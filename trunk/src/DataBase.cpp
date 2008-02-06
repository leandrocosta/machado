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
	mSupport			= support	;
	mConfidence			= confidence	;
	mMinRuleLen			= min_rule_len	;
	mMaxRuleLen			= max_rule_len	;

	mpProjectionTransactionList	= NULL		;

	mCorrectGuesses			= 0		;
	mWrongGuesses			= 0		;
}

DataBase::~DataBase ()
{
	LOGMSG (MEDIUM_LEVEL, "DataBase::~DataBase () - p [%p]\n", this);

	if (mpProjectionTransactionList)
	{
		mpProjectionTransactionList->RemoveAll ();
		delete mpProjectionTransactionList;
	}
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
			items.pop_front (); // transaction ID

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
			items.pop_front (); // transaction ID

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

void DataBase::SortTransactions ()
{
	mTrainTransactionList.SortTransactions ();
	mTestTransactionList.SortTransactions ();
}

void DataBase::ClassifyTestData ()
{
	LOGMSG (MEDIUM_LEVEL, "DataBase::ClassifyTestData ()\n");

	TransactionList::STLTransactionList_cit itEnd = mTestTransactionList.GetEnd ();

	for (TransactionList::STLTransactionList_cit it = mTestTransactionList.GetBegin (); it != itEnd; it++)
	{
		Transaction *pTransaction = static_cast<Transaction *>(*it);

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

	pTransaction->Print ();

	MakeProjection (pTransaction);

	PatternList *pFrequentPatternList = pTransaction->GetFrequentPatternList (mSupport, mpProjectionTransactionList->GetSize (), mMinRuleLen, mMaxRuleLen);
//	pFrequentPatternList->Print ();
//	exit (1);

	string class_guess = "";

	if (AppOptions::GetInstance ()->GetRunMode () == MODE_CLASSICAL)
	{
		LOGMSG (LOW_LEVEL, "DataBase::ClassifyTransaction () - [MODE_CLASSICAL]\n");

		RuleList *pRuleList = pFrequentPatternList->GetRuleList (&mClassList, mConfidence, mpProjectionTransactionList->GetSize ());
		pRuleList->Print ();
		class_guess = pRuleList->GetClassificationValue ();

		delete pRuleList;
	}
	else if (AppOptions::GetInstance ()->GetRunMode () == MODE_ORTHOGONAL)
	{
		LOGMSG (LOW_LEVEL, "DataBase::ClassifyTransaction () - [MODE_ORTHOGONAL]\n");

		PatternList *pOrthogonalFrequentPatternList = pFrequentPatternList->GetOrthogonalPatternList (mpProjectionTransactionList);
		pOrthogonalFrequentPatternList->Print ();

		RuleList *pRuleList = pOrthogonalFrequentPatternList->GetRuleList (&mClassList, mConfidence, mpProjectionTransactionList->GetSize ());
		pRuleList->Print ();
		class_guess = pRuleList->GetClassificationValue ();

		delete pRuleList;

		pOrthogonalFrequentPatternList->RemoveAll ();

		delete pOrthogonalFrequentPatternList;
	}
	else
	{
		LOGMSG (NO_DEBUG, "DataBase::ClassifyTransaction () - unknown run mode\n");
	}

	delete pFrequentPatternList;

	if (class_guess.empty ())
	{
		uint32 num_transactions = 0;
		Class	*pClassGuess = NULL;

		ClassList::STLClassList_cit itEnd = mClassList.GetEnd ();

		for (ClassList::STLClassList_cit it = mClassList.GetBegin (); it != itEnd; it++)
		{
			Class *pClass = static_cast<Class *>(*it);

			if (pClass->GetProjectionTransactionListSize () > num_transactions)
			{
				pClassGuess = pClass;
				num_transactions = pClass->GetProjectionTransactionListSize ();
			}
		}

		if (pClassGuess)
			class_guess = pClassGuess->GetValue ();
	}

	if (class_guess == pTransaction->GetClass ()->GetValue ())
		mCorrectGuesses++;
	else
		mWrongGuesses++;

	cout << class_guess << endl;
}

void DataBase::MakeProjection (Transaction *pTransaction)
{
	if (mpProjectionTransactionList)
	{
		mpProjectionTransactionList->RemoveAll ();
		delete mpProjectionTransactionList;
	}

	mItemList.ClearItemProjectionTransactionLists ();
	mClassList.ClearClassProjectionTransactionLists ();

	mpProjectionTransactionList = mTrainTransactionList.GetProjection (pTransaction);
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
