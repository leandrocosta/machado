#include "DataBase.h"
#include "PatternList.h"
#include "RuleList.h"
#include "DataBaseException.h"
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
	LOGMSG (MAX_LEVEL, "DataBase::~DataBase () - p [%p]\n", this);

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

void DataBase::ClassifyTestData (const RunMode &rRunMode, const PatternList::OrtMode &rOrtMode, const PatternList::OrtMetric &rOrtMetric, const uint32 &rMinNumRules, const uint32 &rMaxNumRankRules)
{
	LOGMSG (MEDIUM_LEVEL, "DataBase::ClassifyTestData ()\n");

	TransactionList::STLTransactionList_cit itEnd = mTestTransactionList.GetEnd ();

	try
	{
		for (TransactionList::STLTransactionList_cit it = mTestTransactionList.GetBegin (); it != itEnd; ++it)
			ClassifyTransaction (static_cast<Transaction *>(*it), rRunMode, rOrtMode, rOrtMetric, rMinNumRules, rMaxNumRankRules);

		LOGMSG (NO_DEBUG, "accuracy [%0.6f] (correct [%u], wrong [%u])\n", ((float32) mCorrectGuesses / (mCorrectGuesses + mWrongGuesses)), mCorrectGuesses, mWrongGuesses);
	}
	catch (DataBaseException &e)
	{
		LOGMSG (NO_DEBUG, "DataBaseException: %s\n", e.GetMessage ().c_str ());
	}
	catch (Exception &e)
	{
		LOGMSG (NO_DEBUG, "Exception: %s\n", e.GetMessage ().c_str ());
	}
}

void DataBase::ClassifyTransaction (Transaction *pTransaction, const RunMode &rRunMode, const PatternList::OrtMode &rOrtMode, const PatternList::OrtMetric &rOrtMetric, const uint32 &rMinNumRules, const uint32 &rMaxNumRankRules)
{
	LOGMSG (MEDIUM_LEVEL, "DataBase::ClassifyTransaction () - pTransaction\n");

	pTransaction->Print ();

	MakeProjection (pTransaction);

	PatternList *pFrequentPatternList = pTransaction->GetFrequentPatternList (mSupport, mpProjectionTransactionList->GetSize (), mMinRuleLen, mMaxRuleLen);
	LOGMSG (MEDIUM_LEVEL, "DataBase::ClassifyTranscation () - frequent patterns:\n");
	pFrequentPatternList->Print ();

	string class_guess = "";

	if (rRunMode == MODE_CLASSICAL)
	{
		LOGMSG (LOW_LEVEL, "DataBase::ClassifyTransaction () - [MODE_CLASSICAL]\n");

		RuleList *pRuleList = NULL;
		float32 confidence = mConfidence;
		
		do
		{
			if (pRuleList)
				delete pRuleList;

			pRuleList = pFrequentPatternList->GetRuleList (&mClassList, confidence, mpProjectionTransactionList->GetSize ());

			if (pRuleList->GetSize () < rMinNumRules)
				confidence *= 0.9;
		} while (pRuleList->GetSize () < rMinNumRules && confidence > 0.001);

		pRuleList->Print ();
		class_guess = pRuleList->GetClassificationValue (rMaxNumRankRules);
		delete pRuleList;
	}
	else if (rRunMode == MODE_ORTHOGONAL)
	{
		LOGMSG (LOW_LEVEL, "DataBase::ClassifyTransaction () - [MODE_ORTHOGONAL]\n");

		PatternList *pOrthogonalFrequentPatternList = pFrequentPatternList->GetOrthogonalPatternList (mpProjectionTransactionList, rOrtMode, rOrtMetric);
		LOGMSG (MEDIUM_LEVEL, "DataBase::ClassifyTranscation () - orthogonal patterns:\n");
		pOrthogonalFrequentPatternList->Print ();

		RuleList *pRuleList = NULL;
		float32 confidence = mConfidence;
		
		do
		{
			if (pRuleList)
				delete pRuleList;

			pRuleList = pOrthogonalFrequentPatternList->GetRuleList (&mClassList, confidence, mpProjectionTransactionList->GetSize ());

			if (pRuleList->GetSize () < rMinNumRules)
				confidence *= 0.9;
		} while (pRuleList->GetSize () < rMinNumRules && confidence > 0.001);

		pRuleList->Print ();
		class_guess = pRuleList->GetClassificationValue (rMaxNumRankRules);
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
		LOGMSG (NO_DEBUG, "DataBase::ClassifyTransaction () - no rule, using more frequent class\n");

		uint32 num_transactions = 0;
		Class	*pClassGuess = NULL;

		ClassList::STLClassList_cit itEnd = mClassList.GetEnd ();

		for (ClassList::STLClassList_cit it = mClassList.GetBegin (); it != itEnd; ++it)
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

	LOGMSG (NO_DEBUG, "DataBase::ClassifyTransaction () - class [%s], guess [%s], correct [%s]\n", pTransaction->GetClass ()->GetValue ().c_str (), class_guess.c_str (), (class_guess == pTransaction->GetClass ()->GetValue () ? "yes":"no"));

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

	LOGMSG (LOW_LEVEL, "DataBase::MakeProjection () - transactions [%llu]\n", mpProjectionTransactionList->GetSize ());
}

void DataBase::PrintDataInfo () const
{
	LOGMSG (NO_DEBUG, "DataBase::PrintDataInfo () - items [%llu], train transactions [%llu], test transactions [%llu]\n", mItemList.GetSize (), mTrainTransactionList.GetSize (), mTestTransactionList.GetSize ());
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
