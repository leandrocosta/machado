#include "PatternList.h"
#include "TransactionList.h"
#include "ItemHash.h"
#include "base/Logger.h"
//#include "alortho/AlOrtho.h"


PatternList::PatternList (const uint64 &max_size) : ObjectList (max_size)
{
	LOGMSG (MEDIUM_LEVEL, "PatternList::PatternList () - p [%p]\n", this);

	mMaxPatternLen = 0;
}

PatternList::~PatternList ()
{
	LOGMSG (MEDIUM_LEVEL, "PatternList::~PatternList () - p [%p]\n", this);
}

void PatternList::PushFront (Object *pObject)
{
	ObjectList::PushFront (pObject);

	if (static_cast<Pattern *>(pObject)->GetSize () > mMaxPatternLen)
		mMaxPatternLen = static_cast<Pattern *>(pObject)->GetSize ();
}

void PatternList::PushBack (Object *pObject)
{
	ObjectList::PushBack (pObject);

	if (static_cast<Pattern *>(pObject)->GetSize () > mMaxPatternLen)
		mMaxPatternLen = static_cast<Pattern *>(pObject)->GetSize ();
}

Pattern* PatternList::GetAt (const uint64 &index) const
{
	return static_cast<Pattern *>(ObjectList::GetAt (index));
}

PatternList* PatternList::GetOrthogonalPatternList () const
{
//	AlOrtho alortho;

	return NULL;
}

const uint32 PatternList::GetSumPatternLen () const
{
	uint32 sumPatternLen = 0;

	STLPatternList_cit	it			;
	STLPatternList_cit	itEnd = GetEnd ()	;

	for (it = GetBegin (); it != itEnd; it++)
	{
		sumPatternLen += static_cast<const Pattern *>(*it)->GetSize ();
	}

	return sumPatternLen;
}

const uint32& PatternList::GetMaxPatternLen () const
{
	return mMaxPatternLen;
}

Pattern* PatternList::GetMoreSimilar (const Pattern *pPattern) const
{
	STLPatternList_cit it = GetBegin ();

	Pattern *pPatternRet = static_cast<Pattern *>(*it);

	float32 similarity = pPatternRet->GetSimilarity (pPattern);

	STLPatternList_cit itEnd = GetEnd ();

	for (++it; it != itEnd; it++)
	{
		Pattern *pPattern2 = static_cast<Pattern *>(*it);

		if (pPattern2->GetSimilarity (pPattern) > similarity)
			pPatternRet = pPattern2;
	}

	return pPatternRet;
}

const float32 PatternList::GetSimilarityRate () const
{
	float32 rate = 0.0;

	STLPatternList_cit		itPattern		;
	ItemList::STLItemList_cit	itItem			;
	ItemHash			Hash			;
	const Item*			pItem		= NULL	;
	uint32				num_items	= 0	;

	STLPatternList_cit itEnd = GetEnd ();

	for (itPattern = GetBegin (); itPattern != itEnd; ++itPattern)
	{
		const Pattern *pPattern = static_cast<const Pattern *>(*itPattern);

		STLPatternList_cit itPatternEnd = pPattern->GetEnd ();

		for (itItem = pPattern->GetBegin (); itItem != itPatternEnd; itItem++)
		{
			pItem = static_cast<const Item *>(*itItem);

			if (! Hash.Find (pItem->GetValue ()))
				Hash.Add (pItem->GetValue (), new Item (pItem->GetValue ()));
			else
				Hash.Get (pItem->GetValue ())->IncCount ();

			num_items++;
		}
	}

	uint32	distinct_items	= Hash.GetSize ()	;
	float32 exclusive_items = 0			;

	ItemHash::STLItemHash_cit itHash			;
	ItemHash::STLItemHash_cit itHashEnd = Hash.GetEnd ()	;

	for (itHash = Hash.GetBegin (); itHash != itHashEnd; itHash++)
	{
		pItem = static_cast<const Item *>(itHash->second);

		exclusive_items += (GetSize () - pItem->GetCount ()) / (GetSize () - 1);
	}

	LOGMSG (MEDIUM_LEVEL, "PatternList::PatternList () - max_pattern_len [%u], num_patterns [%llu], num_items [%u], distinct_items [%u], exclusive_items [%f]\n", mMaxPatternLen, GetSize (), num_items, distinct_items, exclusive_items);

	rate = (float32) (exclusive_items / distinct_items);
//      rate = (float32) (exclusive_items / distinct_items) * ((float32) num_items / (mMaxPatternLen * GetSize ()));
//	rate = (float32) (exclusive_items / distinct_items) * log ((float32) num_items / (mMaxPatternLen * GetSize ()));

	return rate;
}

const float32 PatternList::GetCoverageRate (const TransactionList *pTransactionList) const
{
	uint32	distinct_transactions	= 0		;
	float32 exclusive_factor	= 0.0		;
//	float32 num_coverages		= 0.0		;
	uint32	num_patterns		= GetSize ()	;

	TransactionList::STLTransactionList_cit itTransactionEnd = pTransactionList->GetEnd ();

	for (TransactionList::STLTransactionList_cit itTransaction = pTransactionList->GetBegin (); itTransaction != itTransactionEnd; itTransaction++)
	{
		const Transaction *pTransaction = static_cast<const Transaction *>(*itTransaction);

		uint32 patterns_found_in_transaction = 0;

		STLPatternList_cit itPatternEnd = GetEnd ();

		for (STLPatternList_cit itPattern = GetBegin (); itPattern != itPatternEnd; itPattern++)
		{
			/*
			const Pattern *pPattern = static_cast<const Pattern *>(*itPattern);

			bool bCovered = true;

			ItemList::STLItemList_cit itItemEnd = pPattern->GetEnd ();

			for (ItemList::STLItemList_cit itItem = pPattern->GetBegin (); itItem != itItemEnd; itItem++)
			{
				const Item *pItem = static_cast<const Item *>(*itItem);

				if (! pTransaction->FindByPtr (pItem))
				{
					bCovered = false;
					break;
				}
			}

			if (bCovered)
				patterns_found_in_transaction++;
			*/

			if (pTransaction->IsCoveredBy (static_cast<const Pattern *>(*itPattern)))
				patterns_found_in_transaction++;
		}

		if (patterns_found_in_transaction)
		{
			distinct_transactions++;
			exclusive_factor += float32 (num_patterns - patterns_found_in_transaction) / (num_patterns - 1);
		}

//		num_coverages += patterns_found_in_transaction;
	}

	float rate = exclusive_factor / distinct_transactions;
 //	rate = (float32) (exclusive_factor / distinct_transactions) * ((float32) num_coverages / (GetSize () * pTransactionList->GetSize ()));
//	rate = (float32) (exclusive_factor / distinct_transactions) * log ((float32) num_coverages / (GetSize () * pTransactionList->GetSize ()));

	return rate;
}

const float32 PatternList::GetRate (const TransactionList *pTransactionList) const
{
	return GetSimilarityRate () * GetCoverageRate (pTransactionList);
}
