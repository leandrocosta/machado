#include "Pattern.h"
#include "PatternList.h"
#include "Class.h"
#include "base/Logger.h"

#include <iostream>
using std::cout;
using std::endl;


uint32 Pattern::msSeqPatternID	= 0	;


Pattern::Pattern (const Pattern *pPattern, Item *pItem) : ItemSet ()
{
	InitFields ();

	STLItemList_cit itPatternEnd = pPattern->GetEnd ();

	for (STLItemList_cit it = pPattern->GetBegin (); it != itPatternEnd; ++it)
		PushBack (*it);

	PushBack (pItem);

	const uint32 &rItemID = pItem->GetItemID ();

	const TransactionList &rPatternTransactionList = pPattern->GetTransactionList ();

	TransactionList::STLTransactionList_cit itTransactionEnd = rPatternTransactionList.GetEnd ();

	for (TransactionList::STLTransactionList_cit it = rPatternTransactionList.GetBegin (); it != itTransactionEnd; ++it)
	{
		Transaction *pTransaction = static_cast<Transaction *>(*it);

		if (pTransaction->IsCoveredByItem (rItemID))
		{
			mClassCoverageArray [pTransaction->GetClassID ()]++;
			mTransactionList.PushBack (pTransaction);
		}
	}

	mFrequence = mTransactionList.GetSize ();
}


Pattern::Pattern (Item *pItem) : ItemSet ()
{
	InitFields ();

	PushBack (pItem);

	const TransactionList *pItemTransactionList = pItem->GetProjectionTransactionList ();

	TransactionList::STLTransactionList_cit itTransactionEnd = pItemTransactionList->GetEnd ();

	for (TransactionList::STLTransactionList_cit it = pItemTransactionList->GetBegin (); it != itTransactionEnd; ++it)
	{
		Transaction *pTransaction = static_cast<Transaction *>(*it);

		mClassCoverageArray [pTransaction->GetClassID ()]++;
		mTransactionList.PushBack (pTransaction);
	}

	mFrequence = mTransactionList.GetSize ();
}

Pattern::~Pattern ()
{
	mpChildPatternList->RemoveAll ();
	delete mpChildPatternList;

	delete[] mClassCoverageArray;

	mTransactionList.RemoveAll ();

	RemoveAll ();
}

const uint32 Pattern::GetSeqPatternID ()
{
	uint32 patternID = msSeqPatternID++;

	return patternID;
}

const uint32 Pattern::GetMaxPatternID ()
{
	return msSeqPatternID - 1;
}

void Pattern::ResetSeqPatternID ()
{
	msSeqPatternID = 0;
}

void Pattern::SetPatternID ()
{
	mPatternID = GetSeqPatternID ();
}

const uint32& Pattern::GetPatternID () const
{
	return mPatternID;
}

void Pattern::InitFields ()
{
	mPatternID	= 0	;
	mFrequence	= 0	;
	mSupport	= 0.0	;

	mpChildPatternList = new PatternList ();

	const uint32 num_classes = Class::GetMaxClassID () + 1;

	mClassCoverageArray = new uint32 [num_classes];

	for (uint32 classID = 0; classID < num_classes; classID++)
		mClassCoverageArray [classID] = 0;
}

const uint32 Pattern::GetFrequence () const
{
	return mFrequence;
}

const float32 Pattern::GetAmbiguity () const
{
	const uint32 num_classes = Class::GetMaxClassID () + 1;
	uint32 my_classes	= 0;
	uint32 my_classes_total = 0;
	uint32 my_classes_max	= 0;

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

const float32 Pattern::GetSimilarity (Pattern *pPattern)
{
	float32 similarity = -1;

	ItemList	totalItemList		;
	STLItemList_cit itEnd = GetEnd ()	;

	for (STLItemList_cit it = GetBegin (); it != itEnd; ++it)
	{
		Item *pItem = static_cast<Item *>(*it);

		pItem->SetCount (1);
		totalItemList.PushBack (pItem);
	}

	itEnd = pPattern->GetEnd ();

	for (STLItemList_cit it = pPattern->GetBegin (); it != itEnd; ++it)
	{
		Item *pItem = static_cast<Item *>(*it);

		if (totalItemList.FindByPtr (pItem))
			pItem->IncCount ();
		else
		{
			pItem->SetCount (1);
			totalItemList.PushBack (pItem);
		}
	}

	uint32 num	= 0	;
	uint32 den	= 0	;

	itEnd = totalItemList.GetEnd ();

	for (STLItemList_cit it = totalItemList.GetBegin (); it != itEnd; ++it)
	{
		const Item *pItem = static_cast<const Item *>(*it);

		den += pItem->GetCount ();

		if (pItem->GetCount () > 1)
			num += pItem->GetCount ();
	}

	totalItemList.RemoveAll ();

	similarity = (float32) num / den;

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

	ItemList::STLItemList_cit itMyItem	= GetBegin ();
	ItemList::STLItemList_cit itMyItemEnd	= GetEnd ();

	ItemList::STLItemList_cit itPatternItem		= pPattern->GetBegin ();
	ItemList::STLItemList_cit itPatternItemEnd	= pPattern->GetEnd ();

	while (itMyItem != itMyItemEnd && itPatternItem != itPatternItemEnd)
	{
		if (*itMyItem == *itPatternItem)
			++itPatternItem;
		else if (*itMyItem > *itPatternItem)
			break;

		++itMyItem;
	}

	if (itPatternItem == itPatternItemEnd)
		bRet = true;

	return bRet;
}

void Pattern::Print () const
{
	LOGMSG (HIGH_LEVEL, "Pattern::Print () - support [%0.6f] - [%s]\n", mSupport, GetPrintableString ().c_str ());
}
