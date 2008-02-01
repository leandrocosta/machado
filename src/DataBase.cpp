#include "DataBase.h"
#include "PatternList.h"
#include "RuleList.h"
#include "AppOptions.h"
#include "base/Exception.h"
#include "base/Logger.h"

#include <iostream>
using std::cout;
using std::endl;


DataBase::DataBase (
		const float32 &support, const float32 &confidence,
		const uint32 &min_rule_len, const uint32 &max_rule_len) : IOManager (), Tokenizer (", \t\n")
{
	mSupport	= support	;
	mConfidence	= confidence	;
	mMinRuleLen	= min_rule_len	;
	mMaxRuleLen	= max_rule_len	;

	mCorrectGuesses	= 0;
	mWrongGuesses	= 0;
}

DataBase::~DataBase ()
{
	LOGMSG (MEDIUM_LEVEL, "DataBase::~DataBase () - p [%p]\n", this);
}

void DataBase::LoadTrainData (const string &file)
{
	SetFile (file);

	list<string> lines;
	LoadFile (&lines);

	while (! lines.empty ())
	{

		string s = lines.front ();
		list<string> items;

		Tokenize (s, items);

		if (! items.empty ())
			items.pop_front (); // ID

		if (! items.empty ())
		{
			if (! mClassList.GetClassByValue (items.front ()))
				mClassList.PushBack (new Class (items.front ()));

			Class *pClass = mClassList.GetClassByValue (items.front ());

			Transaction *pTransaction = new Transaction (pClass);
			pClass->AddTransaction (pTransaction);

			items.pop_front ();

			while (! items.empty ())
			{
				if (! mItemList.GetItemByValue (items.front ()))
					mItemList.PushBack (new Item (items.front ()));

				Item *pItem = mItemList.GetItemByValue (items.front ());

				pTransaction->PushBack (pItem);
				pItem->AddTransaction (pTransaction);

				items.pop_front ();
			}

			mTrainTransactionList.PushBack (pTransaction);
		}

		lines.pop_front ();
	}
}

void DataBase::LoadTestData (const string &file)
{
	SetFile (file);

	list<string> lines;
	LoadFile (&lines);

	while (! lines.empty ())
	{

		string s = lines.front ();
		list<string> items;

		Tokenize (s, items);

		if (! items.empty ())
			items.pop_front (); // ID

		if (! items.empty ())
		{
			if (! mClassList.GetClassByValue (items.front ()))
				mClassList.PushBack (new Class (items.front ()));

			Class *pClass = mClassList.GetClassByValue (items.front ());

			Transaction *pTransaction = new Transaction (pClass);

			items.pop_front ();

			while (! items.empty ())
			{
				if (! mItemList.GetItemByValue (items.front ()))
					mItemList.PushBack (new Item (items.front ()));

				pTransaction->PushBack (mItemList.GetItemByValue (items.front ()));

				items.pop_front ();
			}

			mTestTransactionList.PushBack (pTransaction);
		}

		lines.pop_front ();
	}
}

void DataBase::ClassifyTestData ()
{
	LOGMSG (MEDIUM_LEVEL, "DataBase::ClassifyTestData ()\n");

	for (uint64 i = 0; i < mTestTransactionList.GetSize (); i++)
	{
		Transaction *pTransaction = static_cast<Transaction *>(mTestTransactionList.GetAt (i));

		try
		{
			ClassifyTransaction (pTransaction);
		}
		catch (Exception &e)
		{
			LOGMSG (NO_DEBUG, "DataBase::ClassifyTestData () - exception [%s]\n", e.GetMessage ().c_str ());
		}
	}

	LOGMSG (NO_DEBUG, "accuracy [%0.2f] (correct [%u], wrong [%u])\n", ((float32) mCorrectGuesses / (mCorrectGuesses + mWrongGuesses)), mCorrectGuesses, mWrongGuesses);
}

void DataBase::ClassifyTransaction (Transaction *pTransaction)
{
	LOGMSG (MEDIUM_LEVEL, "DataBase::ClassifyTransaction () - pTransaction\n");
//	cout << endl;
//
	pTransaction->Print ();

	MakeProjectionTransactionList (pTransaction);

	PatternList *pFrequentPatternList = GetFrequentPatternList (pTransaction, mProjectionTransactionList.GetSize ());

//	cout << "frequent patterns:" << endl;
//	pFrequentPatternList->Print ();
//	PatternList *pOrthogonalFrequentPatternList = GetOrthogonalPatternList (pFrequentPatternList);
//	cout << "orthogonal frequent patterns:" << endl;
//	pOrthogonalFrequentPatternList->Print ();
//
//	return;

	string class_guess = "";

	if (AppOptions::GetInstance ()->GetRunMode () == MODE_CLASSICAL)
	{
		LOGMSG (LOW_LEVEL, "DataBase::ClassifyTransaction () - [MODE_CLASSICAL]\n");

//		cout << "padrões frequentes [" << pFrequentPatternList->GetSize () << "]" << endl;
//		pFrequentPatternList->Print ();

		RuleList *pRuleList = pFrequentPatternList->GetRuleList (&mClassList, mConfidence);

//		LOGMSG (LOW_LEVEL, "DataBase::ClassifyTransaction () - pRuleList (frequent patterns)\n");
//		cout << "classificação (padrões frequentes):" << endl;

//		cout << "regras:" << endl;
//		pRuleList->Print ();

//		pRuleList->ReverseSort ();

/*
		if (pRuleList->GetSize () > 0)
		{
			const Rule *pBestRule = pRuleList->GetAt (0);
			cout << "MELHOR REGRA:" << endl;
			cout << "classe [" << pBestRule->GetClass () << "], suporte [" << pBestRule->GetSupport () << "], confiança [" << pBestRule->GetConfidence () << "]" << endl;
			cout << "---" << endl;
			const Rule *pWorstRule = pRuleList->GetAt (pRuleList->GetSize () - 1);
			cout << "PIOR REGRA:" << endl;
			cout << "classe [" << pWorstRule->GetClass () << "], suporte [" << pWorstRule->GetSupport () << "], confiança [" << pWorstRule->GetConfidence () << "]" << endl;
			cout << "---" << endl;
		}
*/

//		cout << "classificação:" << endl;
//		pRuleList->PrintClassification ();

		class_guess = pRuleList->GetClassificationValue ();

		delete pRuleList;
	}
	else if (AppOptions::GetInstance ()->GetRunMode () == MODE_ORTHOGONAL)
	{
		LOGMSG (LOW_LEVEL, "DataBase::ClassifyTransaction () - [MODE_ORTHOGONAL]\n");

//		PatternList *pOrthogonalFrequentPatternList = GetOrthogonalPatternList (pFrequentPatternList);
		PatternList *pOrthogonalFrequentPatternList = pFrequentPatternList->GetOrthogonalPatternList (&mProjectionTransactionList);

//		cout << "padrões frequentes ortogonais:" << endl;
		pOrthogonalFrequentPatternList->Print ();

		RuleList *pRuleList = pOrthogonalFrequentPatternList->GetRuleList (&mClassList, mConfidence);

//		LOGMSG (LOW_LEVEL, "DataBase::ClassifyTransaction () - pRuleList (orthogonal frequent patterns)\n");
//		cout << "classificação (padrões frequentes ortogonais):" << endl;

//		cout << "regras:" << endl;
//		pRuleList->Print ();

//		pRuleList->ReverseSort ();

/*
		if (pRuleList->GetSize () > 0)
		{
			Rule *pBestRule = pRuleList->GetAt (0);
			cout << "MELHOR REGRA:" << endl;
			cout << "classe [" << pBestRule->GetClass () << "], suporte [" << pBestRule->GetSupport () << "], confiança [" << pBestRule->GetConfidence () << "]" << endl;
			cout << "---" << endl;
			Rule *pWorstRule = pRuleList->GetAt (pRuleList->GetSize () - 1);
			cout << "PIOR REGRA:" << endl;
			cout << "classe [" << pWorstRule->GetClass () << "], suporte [" << pWorstRule->GetSupport () << "], confiança [" << pWorstRule->GetConfidence () << "]" << endl;
			cout << "---" << endl;
		}
*/

//		cout << "classificação:" << endl;
//		pRuleList->PrintClassification ();

		class_guess = pRuleList->GetClassificationValue ();

		delete pRuleList;

		pOrthogonalFrequentPatternList->RemoveAll ();

		delete pOrthogonalFrequentPatternList;
	}
	else
	{
		LOGMSG (NO_DEBUG, "DataBase::ClassifyTransaction () - unknown run mode\n");
	}

	mProjectionTransactionList.RemoveAll ();

	delete pFrequentPatternList;

	if (class_guess == pTransaction->GetClass ()->GetValue ())
		mCorrectGuesses++;
	else
		mWrongGuesses++;

	cout << class_guess << endl;
}

PatternList* DataBase::GetFrequentPatternList (const Transaction *pTransaction, const uint64 &projection_size) const
{
	LOGMSG (LOW_LEVEL, "DataBase::GetFrequentPatternList () - pTransaction [%p], items [%llu]\n", pTransaction, pTransaction ? pTransaction->GetSize ():0);

	PatternList *pFrequentPatternList = new PatternList ();

	LOGMSG (MEDIUM_LEVEL, "DataBase::GetFrequentPatternList () - add single-item patterns\n");

	for (uint64 i = 0; i < pTransaction->GetSize (); i++)
	{
		Item *pItem = static_cast<Item *>(pTransaction->GetAt (i));

		if ((float32) pItem->GetFrequence () / projection_size >= mSupport)
		{
			Pattern *pPattern = new Pattern (pItem);
			pPattern->SetSupport ((float32) pPattern->GetFrequence () / projection_size);
			LOGMSG (MEDIUM_LEVEL, "DataBase::GetFrequentPatternList () - add pattern [%s], frequence [%llu]\n", pPattern->GetPrintableString ().c_str (), pPattern->GetFrequence ());

			pFrequentPatternList->PushBack (pPattern);
		}
		else
			LOGMSG (MAX_LEVEL, "DataBase::GetFrequentPatternList () - item frequence [%llu], projection size [%llu], support [%f]\n", pItem->GetFrequence (), projection_size, mSupport);
	}

	pFrequentPatternList->Sort ();

	uint32 numFrequentOneItemPatterns = pFrequentPatternList->GetSize ();
	uint32 iTryPattern = 0;

	LOGMSG (MEDIUM_LEVEL, "DataBase::GetFrequentPatternList () - add combined-item patterns\n");

	while (iTryPattern < pFrequentPatternList->GetSize ())
	{
		Pattern *pPattern = pFrequentPatternList->GetAt (iTryPattern);

		if (pPattern->GetSize () < mMaxRuleLen)
		{
			Item *pItemBack = static_cast<Item *>(pPattern->GetBack ());

			for (uint32 i = 0; i < numFrequentOneItemPatterns; i++)
			{
				Item *pItem = static_cast<Item *>(static_cast<Pattern *>(pFrequentPatternList->GetAt (i))->GetAt (0));

				if (pItemBack->GetItemID () < pItem->GetItemID ())
				{
					Pattern *pNewPattern = new Pattern (pPattern);
					pNewPattern->AddItem (pItem);

					pNewPattern->SetSupport ((float32) pNewPattern->GetFrequence () / projection_size);

					if ((float32) pNewPattern->GetFrequence () / projection_size >= mSupport)
					{
						LOGMSG (MEDIUM_LEVEL, "DataBase::GetFrequentPatternList () - add pattern [%s], frequence [%llu]\n", pNewPattern->GetPrintableString ().c_str (), pNewPattern->GetFrequence ());

						pFrequentPatternList->PushBack (pNewPattern);
					}
					else
					{
						LOGMSG (MEDIUM_LEVEL, "DataBase::GetFrequentPatternList () - doesn't add pattern [%s], frequence [%llu]\n", pNewPattern->GetPrintableString ().c_str (), pNewPattern->GetFrequence ());

						delete pNewPattern;
					}
				}
			}
		}

		iTryPattern++;
	}

	LOGMSG (MEDIUM_LEVEL, "DataBase::GetFrequentPatternList () - remove short patterns\n");

	PatternList::STLPatternList_it it	= pFrequentPatternList->GetBegin ();
	PatternList::STLPatternList_it itEnd	= pFrequentPatternList->GetEnd ();

	while (it != itEnd)
	{
		if (static_cast<const Pattern*>(*it)->GetSize () < mMinRuleLen)
		{
			LOGMSG (MEDIUM_LEVEL, "DataBase::GetFrequentPatternList () - delete pattern [%s], frequence [%llu]\n", static_cast<const Pattern*>(*it)->GetPrintableString ().c_str (), static_cast<const Pattern*>(*it)->GetFrequence ());
			delete *it;
			it = pFrequentPatternList->Erase (it);
			itEnd = pFrequentPatternList->GetEnd ();
		}
		else
			it++;
	}

	LOGMSG (LOW_LEVEL, "DataBase::GetFrequentPatternList () - return pFrequentPatternList [%p], patterns [%llu]\n", pFrequentPatternList, pFrequentPatternList->GetSize ());

	return pFrequentPatternList;
}

void DataBase::MakeProjectionTransactionList (const Transaction *pTransaction)
{
	TransactionList::STLTransactionList_cit itEnd = mTrainTransactionList.GetEnd ();

	for (TransactionList::STLTransactionList_cit it = mTrainTransactionList.GetBegin (); it != itEnd; it++)
		if (Transaction::HasIntersectionByPtr (pTransaction, static_cast<const Transaction *>(*it)))
			mProjectionTransactionList.PushBack (static_cast<Transaction *>(*it));
}

void DataBase::Print () const
{
	LOGMSG (NO_DEBUG, "DataBase::Print () - mClassList\n");
	mClassList.Print ();

	LOGMSG (NO_DEBUG, "DataBase::Print () - mItemList\n");
	mItemList.Print ();

	LOGMSG (NO_DEBUG, "DataBase::Print () - mTrainTransactionList\n");
	mTrainTransactionList.Print ();

	LOGMSG (NO_DEBUG, "DataBase::Print () - mTestTransactionList\n");
	mTestTransactionList.Print ();
}
