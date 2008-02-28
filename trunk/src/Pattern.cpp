#include "Pattern.h"
#include "PatternList.h"
#include "Class.h"
#include "DataBaseException.h"
#include "base/Logger.h"

#include <iostream>
using std::cout;
using std::endl;


uint32 Pattern::msSeqPatternID = 0;


Pattern::Pattern (const Pattern *pPattern, Item *pItem) : ItemSet ()
{
	InitFields ();

	STLItemList_cit itPatternEnd = pPattern->GetEnd ();

	for (STLItemList_cit it = pPattern->GetBegin (); it != itPatternEnd; ++it)
	{
		Item *pPatternItem = static_cast<Item *>(*it);
		PushBack (pPatternItem);
		mItemArray [pPatternItem->GetItemID ()] = true;
	}

	PushBack (pItem);
	mItemArray [pItem->GetItemID ()] = true;

	const uint32 &rItemID = pItem->GetItemID ();

	const TransactionList &rPatternTransactionList = pPattern->GetTransactionList ();

	TransactionList::STLTransactionList_cit itTransactionEnd = rPatternTransactionList.GetEnd ();

	for (TransactionList::STLTransactionList_cit it = rPatternTransactionList.GetBegin (); it != itTransactionEnd; ++it)
	{
		Transaction *pTransaction = static_cast<Transaction *>(*it);

		if (pTransaction->IsCoveredByItem (rItemID))
		{
			mTransactionList.PushBack (pTransaction);
			mTransactionCoverageArray [pTransaction->GetTransactionID ()] = true;
			mClassCoverageArray [pTransaction->GetClassID ()]++;
		}
	}

	mFrequence = mTransactionList.GetSize ();
}


Pattern::Pattern (Item *pItem) : ItemSet ()
{
	InitFields ();

	PushBack (pItem);
	mItemArray [pItem->GetItemID ()] = true;

	const TransactionList *pItemTransactionList = pItem->GetProjectionTransactionList ();

	TransactionList::STLTransactionList_cit itTransactionEnd = pItemTransactionList->GetEnd ();

	for (TransactionList::STLTransactionList_cit it = pItemTransactionList->GetBegin (); it != itTransactionEnd; ++it)
	{
		Transaction *pTransaction = static_cast<Transaction *>(*it);

		mTransactionList.PushBack (pTransaction);
		mTransactionCoverageArray [pTransaction->GetTransactionID ()] = true;
		mClassCoverageArray [pTransaction->GetClassID ()]++;
	}

	mFrequence = mTransactionList.GetSize ();
}

Pattern::~Pattern ()
{
	mpChildPatternList->RemoveAll ();
	delete mpChildPatternList;

	delete[] mItemArray;
	delete[] mTransactionCoverageArray;
	delete[] mClassCoverageArray;

	mTransactionList.RemoveAll ();

	RemoveAll ();
}

void Pattern::InitFields ()
{
	mPatternID	= 0	;
	mFrequence	= 0	;
	mSupport	= 0.0	;

	mpChildPatternList = new PatternList ();

	const uint32 num_items = Item::GetNumTrainItems ();
	mItemArray = new bool [num_items];
	for (uint32 itemID = 0; itemID < num_items; itemID++)
		mItemArray [itemID] = false;

	const uint32 num_transactions = Transaction::GetNumTrainTransactions ();
	mTransactionCoverageArray = new bool [num_transactions];
	for (uint32 transactionID = 0; transactionID < num_transactions; transactionID++)
		mTransactionCoverageArray [transactionID] = false;

	const uint32 num_classes = Class::GetNumTrainClasses ();
	mClassCoverageArray = new uint32 [num_classes];
	for (uint32 classID = 0; classID < num_classes; classID++)
		mClassCoverageArray [classID] = 0;
}

const uint32 Pattern::GetSeqPatternID ()
{
	uint32 patternID = msSeqPatternID++;

	return patternID;
}

void Pattern::ResetSeqPatternID ()
{
	msSeqPatternID = 0;
}

/*
const uint32 Pattern::GetMaxPatternID ()
{
	return msSeqPatternID - 1;
}
*/

const uint32 Pattern::GetNumPatterns ()
{
	return msSeqPatternID;
}

void Pattern::SetPatternID ()
{
	mPatternID = GetSeqPatternID ();
}

const uint32& Pattern::GetPatternID () const
{
	return mPatternID;
}

const uint32 Pattern::GetFrequence () const
{
	return mFrequence;
}

const float32 Pattern::GetAmbiguity () const
{
	uint32 my_classes	= 0;
	uint32 my_classes_total = 0;
	uint32 my_classes_max	= 0;

	const uint32 num_classes = Class::GetNumTrainClasses ();

	for (uint32 classID = 0; classID < num_classes; classID++)
	{
		if (mClassCoverageArray [classID])
		{
			my_classes++;
			my_classes_total += mClassCoverageArray [classID];

			if (mClassCoverageArray [classID] > my_classes_max)
				my_classes_max = mClassCoverageArray [classID];
		}
	}

	float32 mean = (float32) my_classes_total / my_classes;

	float32 rate = 0;

	if (my_classes == 1)
		rate = 1;
	else
		rate = mean / my_classes_max;

	return rate;
}

void Pattern::SetSupport (const float32 &support)
{
	mSupport = support;
}

const float32& Pattern::GetSupport () const
{
	return mSupport;
}

const TransactionList& Pattern::GetTransactionList () const
{
	return mTransactionList;
}

const uint32 Pattern::GetNumTransactionsOfClass (const uint32 &classID) const
{
	return mClassCoverageArray [classID];
}

const float32 Pattern::GetSimilarity (const Pattern *pPattern, const OrtMetric &rMetric) const
{
	float32 similarity = 0;

	switch (rMetric)
	{
		case METRIC_SIMILARITY:
			similarity = GetSimilarity (pPattern);
			break;
		case METRIC_TRANS_COVERAGE:
			similarity = GetTransCovSimilarity (pPattern);
			break;
		case METRIC_CLASS_COVERAGE:
			similarity = GetClassCovSimilarity (pPattern);
			break;
		case METRIC_ALL:
			similarity = GetSimilarity (pPattern) * GetTransCovSimilarity (pPattern) * GetClassCovSimilarity (pPattern);
			break;
		case METRIC_UNKNOWN:
		default:
			LOGMSG (NO_DEBUG, "Pattern::GetSimilarity () - unknown orthogonality metric\n");
			throw DataBaseException ("Unknown orthogonality metric");
			break;
	}

	return similarity;
}

const float32 Pattern::GetSimilarity (const Pattern *pPattern) const
{
	float32 similarity = 0;

	const bool *itemArrayLeft	= mItemArray;
	const bool *itemArrayRight	= pPattern->GetItemArray ();

	uint32 num = 0;
	uint32 den = 0;

	const uint32 num_items = Item::GetNumTrainItems ();

	for (uint32 i = 0; i < num_items; i++)
	{
		if (itemArrayLeft [i] || itemArrayRight [i])
		{
			den++;

			if (! itemArrayLeft [i] || ! itemArrayRight [i])
				num++;
		}
	}

	if (den != 0)
		similarity = (float32) num / den;

	return similarity;
}

const float32 Pattern::GetTransCovSimilarity (const Pattern *pPattern) const
{
	float32 similarity = 0;

	const bool *transactionCoverageArrayLeft	= mTransactionCoverageArray;
	const bool *transactionCoverageArrayRight	= pPattern->GetTransactionCoverageArray ();

	uint32 num = 0;
	uint32 den = 0;

	const uint32 num_transactions = Transaction::GetNumTrainTransactions ();

	for (uint32 i = 0; i < num_transactions; i++)
	{
		if (transactionCoverageArrayLeft [i] || transactionCoverageArrayRight [i])
		{
			den++;

			if (! transactionCoverageArrayLeft [i] || ! transactionCoverageArrayRight [i])
				num++;
		}
	}

	if (den != 0)
		similarity = (float32) num / den;

	return similarity;
}

const float32 Pattern::GetClassCovSimilarity (const Pattern *pPattern) const
{
	float32 similarity = 0;

	const uint32 *classCoverageArrayLeft	= mClassCoverageArray;
	const uint32 *classCoverageArrayRight	= pPattern->GetClassCoverageArray ();

	uint32	num_cov_classes	= 0;

	const uint32 num_classes = Class::GetNumTrainClasses ();

	for (uint32 i = 0; i < num_classes; i++)
	{
		if (classCoverageArrayLeft [i] || classCoverageArrayRight [i])
		{
			num_cov_classes++;

			if (classCoverageArrayLeft [i] > classCoverageArrayRight [i])
				similarity += (float32) (classCoverageArrayLeft [i] - classCoverageArrayRight [i]) / classCoverageArrayLeft [i];
			if (classCoverageArrayRight [i] > classCoverageArrayLeft [i])
				similarity += (float32) (classCoverageArrayRight [i] - classCoverageArrayLeft [i]) / classCoverageArrayRight [i];
		}
	}

	if (num_cov_classes != 0)
		similarity /= num_cov_classes;

	return similarity;
}

void Pattern::AddChildPattern (Pattern *pPattern)
{
	mpChildPatternList->PushBack (pPattern);
}

PatternList* Pattern::GetMaximalPatternList ()
{
	PatternList *pMaximalPatternList = NULL;

	if (mpChildPatternList->GetSize ())
	{
		PatternList::STLPatternList_cit itChild		= mpChildPatternList->GetBegin ();
		PatternList::STLPatternList_cit itChildEnd	= mpChildPatternList->GetEnd ();

		Pattern *pChildPattern = static_cast<Pattern *>(*(itChild++));

		pMaximalPatternList = pChildPattern->GetMaximalPatternList ();

		while (itChild != itChildEnd)
		{
			pChildPattern = static_cast<Pattern *>(*(itChild++));

			PatternList *pCandidatePatternList = pChildPattern->GetMaximalPatternList ();

			PatternList::STLPatternList_cit itCandEnd = pCandidatePatternList->GetEnd ();

			for (PatternList::STLPatternList_cit itCand = pCandidatePatternList->GetBegin (); itCand != itCandEnd; ++itCand)
			{
				/*
				 * Insert candidate into maximal pattern
				 * list if there is no super-pattern of
				 * candidate in the list yet.
				 */

				Pattern *pCandidatePattern = static_cast<Pattern *>(*itCand);

				if (! pMaximalPatternList->FindSuperPatternOf (pCandidatePattern))
					pMaximalPatternList->PushBack (pCandidatePattern);
			}

			pCandidatePatternList->RemoveAll ();
			delete pCandidatePatternList;
		}
	}
	else
	{
		pMaximalPatternList = new PatternList ();

		pMaximalPatternList->PushBack (this);
	}

	return pMaximalPatternList;
}

const bool Pattern::IsSuperPatternOf (const Pattern *pPattern) const
{
	bool bRet = false;

	ItemList::STLItemList_cit itLeftItem	= GetBegin ();
	ItemList::STLItemList_cit itLeftItemEnd	= GetEnd ();

	ItemList::STLItemList_cit itRightItem		= pPattern->GetBegin ();
	ItemList::STLItemList_cit itRightItemEnd	= pPattern->GetEnd ();

	while (itLeftItem != itLeftItemEnd && itRightItem != itRightItemEnd)
	{
		if (*itLeftItem == *itRightItem)
			++itRightItem;
		else if (*itLeftItem > *itRightItem)
			break;

		++itLeftItem;
	}

	if (itRightItem == itRightItemEnd)
		bRet = true;

	return bRet;
}

const bool* Pattern::GetItemArray () const
{
	return mItemArray;
}

const bool* Pattern::GetTransactionCoverageArray () const
{
	return mTransactionCoverageArray;
}

const uint32* Pattern::GetClassCoverageArray () const
{
	return mClassCoverageArray;
}

void Pattern::Print () const
{
	LOGMSG (HIGH_LEVEL, "Pattern::Print () - support [%0.6f] - [%s]\n", mSupport, GetPrintableString ().c_str ());
}
