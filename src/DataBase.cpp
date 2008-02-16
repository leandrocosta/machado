#include "DataBase.h"
#include "PatternList.h"
#include "RankingRuleList.h"
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
	mTotalGuesses			= 0		;
	mPatterns			= 0		;
	mRules				= 0		;
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

void DataBase::MakeItemCoverageArrays ()
{
	mTrainTransactionList.MakeItemCoverageArrays (mItemList.GetSize ());
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

		LOGMSG (NO_DEBUG, "accuracy [%0.6f] (correct [%u], wrong [%u])\n", mAccuracy, mCorrectGuesses, mTotalGuesses - mCorrectGuesses);

//		cout << mAccuracy << endl;

//		cout << "average patterns [" << (float32) mPatterns / mTestTransactionList.GetSize () << "], average rules [" << (float32) mRules / mTestTransactionList.GetSize () << "], accuracy [" << mAccuracy << "]" << endl;

		cout << "accuracy [" << mAccuracy << "], average patterns [" << (float32) mPatterns / mTestTransactionList.GetSize () << "], average rules [" << (float32) mRules / mTestTransactionList.GetSize () << "]" << endl;
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
	LOGMSG (HIGH_LEVEL, "DataBase::ClassifyTranscation () - frequent patterns:\n");
	pFrequentPatternList->Print ();

	string	class_guess	= "";
	uint32	patterns	= 0;
	uint32	rules		= 0;

	if (rRunMode == MODE_CLASSICAL)
	{
		LOGMSG (MEDIUM_LEVEL, "DataBase::ClassifyTransaction () - [MODE_CLASSICAL]\n");

		RankingRuleList *pRuleList = pFrequentPatternList->GetRuleList (&mClassList, mConfidence, mpProjectionTransactionList->GetSize (), rMinNumRules);

		patterns	= pFrequentPatternList->GetSize ();
		rules		= pRuleList->GetSize ();

		LOGMSG (MEDIUM_LEVEL, "DataBase::ClassifyTranscation () - rule list:\n");
		pRuleList->Print ();

		class_guess = pRuleList->GetClassificationValue (rMaxNumRankRules);

		delete pRuleList;
	}
	else if (rRunMode == MODE_ORTHOGONAL)
	{
		LOGMSG (MEDIUM_LEVEL, "DataBase::ClassifyTransaction () - [MODE_ORTHOGONAL]\n");

		PatternList *pOrthogonalFrequentPatternList = pFrequentPatternList->GetOrthogonalPatternList (mpProjectionTransactionList, rOrtMode, rOrtMetric);
		LOGMSG (HIGH_LEVEL, "DataBase::ClassifyTranscation () - orthogonal patterns:\n");
		pOrthogonalFrequentPatternList->Print ();

		RankingRuleList *pRuleList = pOrthogonalFrequentPatternList->GetRuleList (&mClassList, mConfidence, mpProjectionTransactionList->GetSize (), rMinNumRules);

		patterns	= pOrthogonalFrequentPatternList->GetSize ();
		rules		= pRuleList->GetSize ();

		LOGMSG (MEDIUM_LEVEL, "DataBase::ClassifyTranscation () - rule list:\n");
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
		LOGMSG (LOW_LEVEL, "DataBase::ClassifyTransaction () - no rule, using more frequent class\n");

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

	mTotalGuesses++;

	if (class_guess == pTransaction->GetClass ()->GetValue ())
		mCorrectGuesses++;

	uint32 train_size = mTrainTransactionList.GetSize ();
	uint32 test_size = mTestTransactionList.GetSize ();

	mAccuracy = (mTotalGuesses > 0 ? (float32) mCorrectGuesses / (mTotalGuesses) : 0);

	LOGMSG (NO_DEBUG, "DataBase::ClassifyTransaction () - transaction [%u/%u], class [%s], guess [%s], correct [%s], accuracy [%f] (patterns [%u], rules [%u])\n", pTransaction->GetTransactionID () - train_size + 1, test_size, pTransaction->GetClass ()->GetValue ().c_str (), class_guess.c_str (), (class_guess == pTransaction->GetClass ()->GetValue () ? "yes":"no"), mAccuracy, patterns, rules);

	cout << "transaction [" << (pTransaction->GetTransactionID () - train_size + 1) << "/" << test_size << "], class [" << pTransaction->GetClass ()->GetValue () << "], guess [" << class_guess << "], correct [" << (class_guess == pTransaction->GetClass ()->GetValue () ? "yes":"no") << "], patterns [" << patterns << "], rules [" << rules << "], accuracy [" << mAccuracy << "]" << endl;

	mPatterns	+= patterns	;
	mRules		+= rules	;

//	cout << class_guess << endl;
}

void DataBase::MakeProjection (Transaction *pTransaction)
{
	LOGMSG (LOW_LEVEL, "DataBase::MakeProjection () - begin\n");

	if (mpProjectionTransactionList)
	{
		mpProjectionTransactionList->RemoveAll ();
		delete mpProjectionTransactionList;
	}

	mItemList.ClearItemProjectionTransactionLists ();
	mClassList.ClearClassProjectionTransactionLists ();

	mpProjectionTransactionList = mTrainTransactionList.GetProjection (pTransaction);

	uint32 projection_size = mpProjectionTransactionList->GetSize ();

	LOGMSG (LOW_LEVEL, "DataBase::MakeProjection () - transactions [%u]\n", projection_size);
}

void DataBase::PrintDataInfo () const
{
	uint32 class_list_size	= mClassList.GetSize ();
	uint32 item_list_size	= mItemList.GetSize ();
	uint32 train_list_size	= mTrainTransactionList.GetSize ();
	uint32 test_list_size	= mTestTransactionList.GetSize ();

	LOGMSG (NO_DEBUG, "DataBase::PrintDataInfo () - classes [%u], items [%u], train transactions [%u], test transactions [%u]\n", class_list_size, item_list_size, train_list_size, test_list_size);
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
