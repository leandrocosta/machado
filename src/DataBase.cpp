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

		RuleList *pRuleList = GetRuleList (pFrequentPatternList);

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

		RuleList *pRuleList = GetRuleList (pOrthogonalFrequentPatternList);

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

PatternList* DataBase::GetOrthogonalPatternList (PatternList *pPatternList) const
{
	LOGMSG (LOW_LEVEL, "DataBase::GetOrthogonalPatternList () - pPatternList [%p], patterns [%llu]\n", pPatternList, pPatternList ? pPatternList->GetSize ():0);

	return GetOrthogonalPatternListHeuristical (pPatternList);
}

PatternList* DataBase::GetOrthogonalPatternListHeuristical (PatternList *pPatternList) const
{
	LOGMSG (LOW_LEVEL, "DataBase::GetOrthogonalPatternListHeuristical () - begin\n");

	PatternList *pOrthogonalPatternList = new PatternList ();

	if (pPatternList->GetSize () <= 2)
	{
		if (pPatternList->GetSize () > 0)
			pOrthogonalPatternList->PushBack (pPatternList->GetAt (0));

		if (pPatternList->GetSize () > 1)
			pOrthogonalPatternList->PushBack (pPatternList->GetAt (1));
	}
	else
	{
		pPatternList->ReverseSort ();

		pOrthogonalPatternList->PushBack (pPatternList->GetAt (0));
		pOrthogonalPatternList->PushBack (pPatternList->GetAt (1));

		pPatternList->GetAt (0)->SetGot (true);
		pPatternList->GetAt (1)->SetGot (true);

		uint64 index = 0;

		// two items
		for (index = 0; index < pPatternList->GetSize (); index++)
		{
			Pattern *pPattern1 = static_cast<Pattern *>(pPatternList->GetAt (index));

			if (! pPattern1->GetGot ())
			{
				Pattern *pPattern2 = pOrthogonalPatternList->GetMoreSimilar (pPattern1);
				float32 rate_prv = pOrthogonalPatternList->GetRate (&mTrainTransactionList);

				pOrthogonalPatternList->Remove (pPattern2);
				pOrthogonalPatternList->PushBack (pPattern1);

				float32 rate_new = pOrthogonalPatternList->GetRate (&mTrainTransactionList);

				if (rate_new > rate_prv)
				{
					pPattern1->SetGot (true);
					pPattern2->SetGot (false);
				}
				else
				{
					pOrthogonalPatternList->Remove (pPattern1);
					pOrthogonalPatternList->PushBack (pPattern2);
				}
			}

		}
	}

	return pOrthogonalPatternList;
}

PatternList* DataBase::GetOrthogonalPatternListPolynomial (PatternList *pPatternList) const
{
	LOGMSG (LOW_LEVEL, "DataBase::GetOrthogonalPatternListPolynomial () - begin\n");

	PatternList *pOrthogonalPatternList = new PatternList ();

	Pattern *pPattern1 = NULL;
	Pattern *pPattern2 = NULL;

	if (pPatternList->GetSize () <= 2)
	{
		if (pPatternList->GetSize () > 0)
			pPattern1 = pPatternList->GetAt (0);

		if (pPatternList->GetSize () > 1)
			pPattern2 = pPatternList->GetAt (1);
	}
	else
	{
		float32 rate = -1;

		for (uint32 i = 0; i < pPatternList->GetSize () - 1; i++)
		{
			LOGMSG (MEDIUM_LEVEL, "DataBase::GetOrthogonalPatternListPolynomial () - try pattern 1 [%s]\n", pPatternList->GetAt (i)->GetPrintableString ().c_str ());

			for (uint32 j = i+1; j < pPatternList->GetSize (); j++)
			{
				LOGMSG (MEDIUM_LEVEL, "DataBase::GetOrthogonalPatternListPolynomial () - try pattern 2 [%s]\n", pPatternList->GetAt (j)->GetPrintableString ().c_str ());

				pOrthogonalPatternList->PushBack (pPatternList->GetAt (i));
				pOrthogonalPatternList->PushBack (pPatternList->GetAt (j));

				float32 rate_new = -1;

				switch (AppOptions::GetInstance ()->GetOrtMode ())
				{
					case ORTH_SIMILARITY:
						rate_new = pOrthogonalPatternList->GetSimilarityRate ();
						break;
					case ORTH_COVERAGE:
						rate_new = pOrthogonalPatternList->GetCoverageRate (&mTrainTransactionList);
						break;
					case ORTH_BOTH:
						rate_new = pOrthogonalPatternList->GetRate (&mTrainTransactionList);
						break;
					case ORTH_UNKNOWN:
					default:
						LOGMSG (NO_DEBUG, "DataBase::GetOrthogonalPatternListPolynomial () - unknown orthogonality mode\n");
						break;
				}

				if (rate_new > rate)
				{
					pPattern1 = pPatternList->GetAt (i);
					pPattern2 = pPatternList->GetAt (j);
				}

				pOrthogonalPatternList->RemoveAll ();
			}
		}
	}

	if (pPattern1)
		pOrthogonalPatternList->PushBack (pPattern1);

	if (pPattern2)
		pOrthogonalPatternList->PushBack (pPattern2);

	return pOrthogonalPatternList;
}

RuleList* DataBase::GetRuleList (PatternList *pPatternList) const
{
	RuleList *pRuleList = new RuleList ();

	for (uint32 i = 0; i < mClassList.GetSize (); i++)
	{
		Class *pClass = static_cast<Class *>(mClassList.GetAt (i));

		LOGMSG (MEDIUM_LEVEL, "DataBase::GetRuleList () - check class [%s]\n", pClass->GetValue ().c_str ());

		for (uint32 j = 0; j < pPatternList->GetSize (); j++)
		{
			Pattern *pPattern = static_cast<Pattern *>(pPatternList->GetAt (j));

			Rule *pRule = new Rule (pClass, pPattern);

			if (pRule->GetConfidence () >= mConfidence)
			{
				LOGMSG (MEDIUM_LEVEL, "DataBase::GetRuleList () - add rule [%s]\n", pPattern->GetPrintableString ().c_str ());
				pRuleList->PushBack (pRule);
			}
			else
				delete pRule;
		}
	}

	return pRuleList;
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
