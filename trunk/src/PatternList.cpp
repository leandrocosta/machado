#include "PatternList.h"
#include "TransactionList.h"
#include "ItemList.h"
#include "RankingRuleList.h"
#include "RankingRule.h"
#include "ClassList.h"
#include "DataBaseException.h"
#include "base/Logger.h"


bool**		PatternList::mItemPatternCoverageMatrix		= NULL;
bool**		PatternList::mTransactionPatternCoverageMatrix	= NULL;
uint32**	PatternList::mClassPatternCoverageMatrix	= NULL;


PatternList::PatternList (const uint64 &max_size) : ObjectList (max_size)
{
//	LOGMSG (MAX_LEVEL, "PatternList::PatternList () - p [%p]\n", this);

//	mMaxPatternLen = 0;
}

PatternList::~PatternList ()
{
//	LOGMSG (MAX_LEVEL, "PatternList::~PatternList () - p [%p]\n", this);
}

/*
void PatternList::PushFront (Object *pObject)
{
	ObjectList::PushFront (pObject);

//	if (static_cast<Pattern *>(pObject)->GetSize () > mMaxPatternLen)
//		mMaxPatternLen = static_cast<Pattern *>(pObject)->GetSize ();
}

void PatternList::PushBack (Object *pObject)
{
	ObjectList::PushBack (pObject);

//	if (static_cast<Pattern *>(pObject)->GetSize () > mMaxPatternLen)
//		mMaxPatternLen = static_cast<Pattern *>(pObject)->GetSize ();
}
*/

PatternList* PatternList::GetOrthogonalPatternList (const TransactionList *pTransactionList, const OrtMode &mode, const OrtMetric &metric)
{
	PatternList *pOrthogonalPatternList = NULL;

	switch (metric)
	{
		case METRIC_SET_SIMILARITY:
			MakeItemPatternCoverageMatrix (this);
			break;
		case METRIC_SET_COVERAGE:
			MakeTransactionPatternCoverageMatrix (pTransactionList, this);
			break;
		case METRIC_SET_SIM_COV:
			MakeItemPatternCoverageMatrix (this);
			MakeTransactionPatternCoverageMatrix (pTransactionList, this);
			break;
		case METRIC_SET_CLASS_COVERAGE:
		case METRIC_PAIR_MEAN_CLASS_COVERAGE:
			MakeClassPatternCoverageMatrix (pTransactionList, this);
			break;
		case METRIC_ALL:
			MakeItemPatternCoverageMatrix (this);
			MakeTransactionPatternCoverageMatrix (pTransactionList, this);
			MakeClassPatternCoverageMatrix (pTransactionList, this);
			break;
		default:
			break;
	}

	switch (mode)
	{
		case ORTH_HEURISTICAL:
			pOrthogonalPatternList = GetOrthogonalPatternListHeuristical (pTransactionList, metric);
			break;
		case ORTH_POLYNOMIAL:
			pOrthogonalPatternList = GetOrthogonalPatternListPolynomial (pTransactionList, metric);
			break;
		default:
			LOGMSG (NO_DEBUG, "PatternList::GetOrthogonalPatternList () - unknown orthogonality mode\n");
			throw DataBaseException ("Unknown orthogonality mode");
			break;
	}

	switch (metric)
	{
		case METRIC_SET_SIMILARITY:
			DestroyItemPatternCoverageMatrix ();
			break;
		case METRIC_SET_COVERAGE:
			DestroyTransactionPatternCoverageMatrix ();
			break;
		case METRIC_SET_SIM_COV:
			DestroyItemPatternCoverageMatrix ();
			DestroyTransactionPatternCoverageMatrix ();
			break;
		case METRIC_SET_CLASS_COVERAGE:
		case METRIC_PAIR_MEAN_CLASS_COVERAGE:
			DestroyClassPatternCoverageMatrix ();
			break;
		case METRIC_ALL:
			DestroyItemPatternCoverageMatrix ();
			DestroyTransactionPatternCoverageMatrix ();
			DestroyClassPatternCoverageMatrix ();
			break;
		default:
			break;
	}

	return pOrthogonalPatternList;
}

PatternList* PatternList::GetOrthogonalPatternListHeuristical (const TransactionList *pTransactionList, const OrtMetric &metric)
{
	LOGMSG (LOW_LEVEL, "PatternList::GetOrthogonalPatternListHeuristical () - metric [%c]\n", metric);

	PatternList *pOrthogonalPatternList = new PatternList ();

	if (GetSize () > 0)
	{
		float32	rate_prv	= 0;
		float32	rate_new	= 0;

		PatternList *pTryPatternList = new PatternList ();

		ReverseSort ();

		Pattern *pPatternAt0 = static_cast<Pattern *>(GetAt (0));

		pTryPatternList->PushBack (pPatternAt0);
		pPatternAt0->SetGot (true);

		do
		{
			pOrthogonalPatternList->RemoveAll ();

			STLPatternList_cit itEnd = pTryPatternList->GetEnd ();

			for (STLPatternList_cit it = pTryPatternList->GetBegin (); it != itEnd; it++)
				pOrthogonalPatternList->PushBack (*it);

			rate_prv = rate_new;

			uint32 orthogonal_size = pOrthogonalPatternList->GetSize ();

			LOGMSG (MEDIUM_LEVEL, "PatternList::GetOrthogonalPatternListHeuristical () - elements [%u], rate [%f]\n", orthogonal_size, rate_prv);

			itEnd = GetEnd ();

			bool bGot = false;

			for (STLPatternList_cit it = GetBegin (); it != itEnd; it++)
			{
				Pattern *pPattern = static_cast<Pattern *>(*it);

				if (! pPattern->GetGot ())
				{
					pTryPatternList->PushBack (pPattern);
					pPattern->SetGot (true);
					bGot = true;
					rate_new = pTryPatternList->GetRate (pTransactionList, metric);

					break;
				}
			}

			if (bGot)
			{
				for (STLPatternList_cit it = GetBegin (); it != itEnd; it++)
				{
					Pattern *pCandToGetInPattern = static_cast<Pattern *>(*it);

					if (! pCandToGetInPattern->GetGot ())
					{
						Pattern *pCandToGetOutPattern = pTryPatternList->GetMoreSimilar (pCandToGetInPattern);

						pTryPatternList->Remove (pCandToGetOutPattern);
						pTryPatternList->PushBack (pCandToGetInPattern);

						float32 rate_try = pTryPatternList->GetRate (pTransactionList, metric);

						if (rate_try > rate_new)
						{
							pCandToGetInPattern->SetGot (true);
							pCandToGetOutPattern->SetGot (false);

							rate_new = rate_try;
						}
						else
						{
							pTryPatternList->Remove (pCandToGetInPattern);
							pTryPatternList->PushBack (pCandToGetOutPattern);
						}
					}
				}
			}
			else
				break;
		} while (rate_new >= rate_prv);

		pTryPatternList->RemoveAll ();
		delete pTryPatternList;
	}

	uint32 orthogonal_size = pOrthogonalPatternList->GetSize ();

	LOGMSG (LOW_LEVEL, "PatternList::GetOrthogonalPatternListHeuristical () - patterns [%u]\n", orthogonal_size);

	return pOrthogonalPatternList;
}

PatternList* PatternList::GetOrthogonalPatternListPolynomial (const TransactionList *pTransactionList, const OrtMetric &metric)
{
	LOGMSG (LOW_LEVEL, "PatternList::GetOrthogonalPatternListPolynomial () - begin\n");

	PatternList *pOrthogonalPatternList = new PatternList ();

	if (GetSize () > 0)
	{
		PatternList *pTryPatternList = new PatternList ();

		float32	rate_prv	= 0;
		float32	rate_new	= 0;
		uint32	num_patterns	= 1;

		do
		{
			pOrthogonalPatternList->RemoveAll ();
			delete pOrthogonalPatternList;
			pOrthogonalPatternList = pTryPatternList;
			rate_prv = rate_new;

			if (num_patterns <= GetSize ())
			{
				pTryPatternList = GetOrthogonalPatternListPolynomial (pTransactionList, metric, num_patterns++);
				rate_new = pTryPatternList->GetRate (pTransactionList, metric);
			}
			else
				rate_new = 0;

			LOGMSG (LOW_LEVEL, "PatternList::GetOrthogonalPatternListPolynomial () - rate_prv [%f], rate_new [%f]\n", rate_prv, rate_new);
		} while (rate_new >= rate_prv);
	}

	LOGMSG (LOW_LEVEL, "PatternList::GetOrthogonalPatternListPolynomial () - num_patterns [%u]\n", pOrthogonalPatternList->GetSize ());

	return pOrthogonalPatternList;
}

PatternList* PatternList::GetOrthogonalPatternListPolynomial (const TransactionList *pTransactionList, const OrtMetric &metric, const uint32 &num_patterns)
{
	LOGMSG (LOW_LEVEL, "PatternList::GetOrthogonalPatternListPolynomial () - num_patterns [%u]\n", num_patterns);

	PatternList *pOrthogonalPatternList = new PatternList ();

	if (num_patterns == 1)
	{
		pOrthogonalPatternList->PushBack (GetAt (0));
	}
	else
	{
		uint32* iArray = new uint32 [num_patterns];

		for (uint32 i = 0; i < num_patterns; i++)
			iArray [i] = i;

		float32	rate_prv	= 0	;
		bool	bKeepRunning	= true	;

		while (bKeepRunning)
		{
			PatternList *pTryPatternList = new PatternList ();

			for (uint32 i = 0; i < num_patterns; i++)
				pTryPatternList->PushBack (GetAt (iArray [i]));

			float32 rate_new = pTryPatternList->GetRate (pTransactionList, metric);

			if (rate_new > rate_prv)
			{
				pOrthogonalPatternList->RemoveAll ();
				delete pOrthogonalPatternList;
				pOrthogonalPatternList = pTryPatternList;
				rate_prv = rate_new;
			}
			else
			{
				pTryPatternList->RemoveAll ();
				delete pTryPatternList;
			}

			int32 i = num_patterns-1;

			while (i >= 0)
			{
				if (iArray [i] < GetSize () - (num_patterns-i))
				{
					iArray [i]++;

					i++;

					while ((uint32) i < num_patterns)
					{
						iArray [i] = iArray [i-1]+1;
						i++;
					}

					break;
				}
				else
					i--;
			}

			if (i < 0)
				bKeepRunning = false;
		}
	}

	LOGMSG (LOW_LEVEL, "PatternList::GetOrthogonalPatternListPolynomial () - rate [%f]\n", pOrthogonalPatternList->GetRate (pTransactionList, metric));

	return pOrthogonalPatternList;
}

RankingRuleList* PatternList::GetRuleList (const ClassList *pClassList, const float32 &confidence, const uint32 &projection_size, const uint32 &min_num_rules) const
{
	LOGMSG (LOW_LEVEL, "PatternList::GetRuleList () - confidence [%f]\n", confidence);

	RankingRuleList *pRuleList = new RankingRuleList ();
	RankingRuleList *pLowConfidenceRuleList = new RankingRuleList ();

	ClassList::STLClassList_cit itClassEnd = pClassList->GetEnd ();

	for (ClassList::STLClassList_cit itClass = pClassList->GetBegin (); itClass != itClassEnd; ++itClass)
	{
		Class *pClass = static_cast<Class *>(*itClass);

		uint32 rules = 0;

		STLPatternList_cit itPatternEnd = GetEnd ();

		for (STLPatternList_cit itPattern = GetBegin (); itPattern != itPatternEnd; ++itPattern)
		{
			Pattern *pPattern = static_cast<Pattern *>(*itPattern);

			RankingRule *pRule = new RankingRule (pClass, pPattern, projection_size, pClassList->GetSize ());

			if (pRule->GetConfidence () >= confidence)
			{
				pRuleList->PushBack (pRule);
				rules++;
			}
			else
				pLowConfidenceRuleList->PushBack (pRule);
		}

		LOGMSG (MEDIUM_LEVEL, "PatternList::GetRuleList () - class [%s], rules [%u]\n", pClass->GetValue ().c_str (), rules);
	}

	uint32 rules = pRuleList->GetSize ();

	if (rules < min_num_rules)
	{
		LOGMSG (LOW_LEVEL, "PatternList::GetRuleList () - min_num_rules [%u], try to add %u more rules to result\n", min_num_rules, (min_num_rules - rules));

		RankingRuleList *pAddRuleList = static_cast<const RankingRuleList *>(pLowConfidenceRuleList->GetPartialReverseSortCopy (min_num_rules - rules));

		RankingRuleList::STLRankingRuleList_cit itEnd = pAddRuleList->GetEnd ();

		for (RankingRuleList::STLRankingRuleList_cit it = pAddRuleList->GetBegin (); it != itEnd; ++it)
		{
			pRuleList->PushBack (*it);
			pLowConfidenceRuleList->Remove (*it);
			rules++;
		}

		pAddRuleList->RemoveAll ();
		delete pAddRuleList;
	}

	delete pLowConfidenceRuleList;

	LOGMSG (LOW_LEVEL, "PatternList::GetRuleList () - rules [%u]\n", rules);

	return pRuleList;
}

/*
const uint32 PatternList::GetSumPatternLen () const
{
	uint32 sumPatternLen = 0;

	STLPatternList_cit	it			;
	STLPatternList_cit	itEnd = GetEnd ()	;

	for (it = GetBegin (); it != itEnd; ++it)
	{
		sumPatternLen += static_cast<const Pattern *>(*it)->GetSize ();
	}

	return sumPatternLen;
}

const uint32& PatternList::GetMaxPatternLen () const
{
	return mMaxPatternLen;
}
*/

Pattern* PatternList::GetMoreSimilar (Pattern *pPattern) const
{
	STLPatternList_cit it		= GetBegin ();
	STLPatternList_cit itEnd	= GetEnd ();

	Pattern *pPatternRet = static_cast<Pattern *>(*(it++));

	float32 similarity = pPatternRet->GetSimilarity (pPattern);

	do
	{
		Pattern *pPattern2 = static_cast<Pattern *>(*(it++));

		if (pPattern2->GetSimilarity (pPattern) > similarity)
			pPatternRet = pPattern2;
	} while (it != itEnd);

	return pPatternRet;
}

const float32 PatternList::GetSetSimilarityRate ()
{
	LOGMSG (HIGH_LEVEL, "PatternList::GetSetSimilarityRate () - begin\n");

	float32 rate		= 0		;
	uint32	num_patterns	= GetSize ()	;

	if (num_patterns > 1)
	{
		/*
		ItemList	totalItemList		;
		uint32		num_items	= 0	;

		STLPatternList_cit itEnd = GetEnd ();

		for (STLPatternList_cit it = GetBegin (); it != itEnd; ++it)
		{
			const Pattern *pPattern = static_cast<const Pattern *>(*it);

			STLPatternList_cit itPatternEnd = pPattern->GetEnd ();

			for (ItemList::STLItemList_cit itItem = pPattern->GetBegin (); itItem != itPatternEnd; ++itItem)
			{
				Item *pItem = static_cast<Item *>(*itItem);

				if (totalItemList.FindByPtr (pItem))
					pItem->IncCount ();
				else
				{
					pItem->SetCount (1);
					totalItemList.PushBack (pItem);
				}

				num_items++;
			}
		}

		uint32	distinct_items	= totalItemList.GetSize ()	;
		float32 exclusive_items = 0				;

		ItemList::STLItemList_cit itList				;
		ItemList::STLItemList_cit itListEnd = totalItemList.GetEnd ()	;

		for (itList = totalItemList.GetBegin (); itList != itListEnd; itList++)
		{
			const Item *pItem = static_cast<const Item *>(*itList);

			exclusive_items += (GetSize () - pItem->GetCount ()) / (GetSize () - 1);
		}

		uint32 size = GetSize ();

//		LOGMSG (HIGH_LEVEL, "PatternList::PatternList () - max_pattern_len [%u], num_patterns [%u], num_items [%u], distinct_items [%u], exclusive_items [%f]\n", mMaxPatternLen, size, num_items, distinct_items, exclusive_items);
		LOGMSG (HIGH_LEVEL, "PatternList::PatternList () - num_patterns [%u], num_items [%u], distinct_items [%u], exclusive_items [%f]\n", size, num_items, distinct_items, exclusive_items);

		rate = (float32) (exclusive_items / distinct_items);
//      	rate = (float32) (exclusive_items / distinct_items) * ((float32) num_items / (mMaxPatternLen * GetSize ()));
//		rate = (float32) (exclusive_items / distinct_items) * log ((float32) num_items / (mMaxPatternLen * GetSize ()));

		totalItemList.RemoveAll ();
		*/

		uint32* patternIDArray	= new uint32 [num_patterns]	;
		uint32	i		= 0				;

		STLPatternList_cit itPatternEnd = GetEnd ();

		for (STLPatternList_cit itPattern = GetBegin (); itPattern != itPatternEnd; ++itPattern)
			patternIDArray [i++] = static_cast<const Pattern *>(*itPattern)->GetPatternID ();

		uint32	num_items		= 0	;
		float32 exclusive_factor	= 0.0	;

		for (uint32 itemID = 0; itemID < Item::GetMaxItemID () + 1; itemID++)
		{
			uint32 patterns_found_with_item = 0;

			/*
			STLPatternList_cit itPatternEnd = GetEnd ();

			for (STLPatternList_cit itPattern = GetBegin (); itPattern != itPatternEnd; ++itPattern)
				if (mItemPatternCoverageMatrix [itemID][static_cast<const Pattern *>(*itPattern)->GetPatternID ()])
					patterns_found_with_item++;
			*/

			for (uint32 i = 0; i < num_patterns; i++)
				if (mItemPatternCoverageMatrix [itemID][patternIDArray [i]])
					patterns_found_with_item++;

			if (patterns_found_with_item)
			{
				num_items++;
				exclusive_factor += float32 (num_patterns - patterns_found_with_item) / (num_patterns - 1);
			}
		}

		delete[] patternIDArray;

		rate = exclusive_factor / num_items;
	}

	return rate;
}

const float32 PatternList::GetSetCoverageRate (const TransactionList *pTransactionList) const
{
	LOGMSG (HIGH_LEVEL, "PatternList::GetSetCoverageRate () - begin\n");

	float32 rate		= 0		;
	uint32	num_patterns	= GetSize ()	;

	if (num_patterns > 1)
	{
		uint32* patternIDArray	= new uint32 [num_patterns]	;
		uint32	i		= 0				;

		STLPatternList_cit itPatternEnd = GetEnd ();

		for (STLPatternList_cit itPattern = GetBegin (); itPattern != itPatternEnd; ++itPattern)
			patternIDArray [i++] = static_cast<const Pattern *>(*itPattern)->GetPatternID ();

		uint32	num_transactions	= 0		;
		float32 exclusive_factor	= 0.0		;
//		float32 num_coverages		= 0.0		;

		for (uint32 transactionID = 0; transactionID < Transaction::GetMaxTransactionID () + 1; transactionID++)
		{
			uint32 patterns_found_in_transaction = 0;

			/*
			STLPatternList_cit itPatternEnd = GetEnd ();

			for (STLPatternList_cit itPattern = GetBegin (); itPattern != itPatternEnd; ++itPattern)
				if (mTransactionPatternCoverageMatrix [transactionID][static_cast<const Pattern *>(*itPattern)->GetPatternID ()])
					patterns_found_in_transaction++;
			*/

			for (uint32 i = 0; i < num_patterns; i++)
				if (mTransactionPatternCoverageMatrix [transactionID][patternIDArray [i]])
					patterns_found_in_transaction++;

			if (patterns_found_in_transaction)
			{
				num_transactions++;
				exclusive_factor += float32 (num_patterns - patterns_found_in_transaction) / (num_patterns - 1);
			}

//			num_coverages += patterns_found_in_transaction;
		}

		delete[] patternIDArray;

		rate = exclusive_factor / num_transactions;
//		rate = (float32) (exclusive_factor / distinct_transactions) * ((float32) num_coverages / (GetSize () * pTransactionList->GetSize ()));
//		rate = (float32) (exclusive_factor / distinct_transactions) * log ((float32) num_coverages / (GetSize () * pTransactionList->GetSize ()));
	}

	return rate;
}

const float32 PatternList::GetSetClassCoverageRate (const TransactionList *pTransactionList) const
{
	LOGMSG (HIGH_LEVEL, "PatternList::GetSetClassCoverageRate () - begin\n");

	float32 rate		= 0		;
	uint32	num_patterns	= GetSize ()	;

	if (num_patterns > 1)
	{
		uint32 num_total_classes = Class::GetMaxClassID () + 1;

		float32* classCoverageMeanArray = new float32 [num_total_classes];

		for (uint32 classID = 0; classID < num_total_classes; classID++)
			classCoverageMeanArray [classID] = 0;

		STLPatternList_cit itPatternEnd = GetEnd ();

		for (STLPatternList_cit itPattern = GetBegin (); itPattern != itPatternEnd; ++itPattern)
		{
			const uint32 patternID = static_cast<const Pattern *>(*itPattern)->GetPatternID ();

			for (uint32 classID = 0; classID < num_total_classes; classID++)
				classCoverageMeanArray [classID] += mClassPatternCoverageMatrix [classID][patternID];
		}

		for (uint32 classID = 0; classID < num_total_classes; classID++)
			classCoverageMeanArray [classID] /= num_patterns;

		uint32* patternIDArray	= new uint32 [num_patterns]	;
		uint32	i		= 0				;

		itPatternEnd = GetEnd ();

		for (STLPatternList_cit itPattern = GetBegin (); itPattern != itPatternEnd; ++itPattern)
			patternIDArray [i++] = static_cast<const Pattern *>(*itPattern)->GetPatternID ();

		uint32	num_classes		= 0	;
		float32 exclusive_factor	= 0.0	;

		for (uint32 classID = 0; classID < Class::GetMaxClassID () + 1; classID++)
		{
			uint32 patterns_found_with_class = 0;

			/*
			STLPatternList_cit itPatternEnd = GetEnd ();

			for (STLPatternList_cit itPattern = GetBegin (); itPattern != itPatternEnd; ++itPattern)
				if (mClassPatternCoverageMatrix [classID][static_cast<const Pattern *>(*itPattern)->GetPatternID ()] >= 0.9 * classCoverageMeanArray [classID])
					patterns_found_with_class++;
			*/

			for (uint32 i = 0; i < num_patterns; i++)
				if (mClassPatternCoverageMatrix [classID][patternIDArray [i]] >= 0.9 * classCoverageMeanArray [classID])
					patterns_found_with_class++;

			if (patterns_found_with_class)
			{
				num_classes++;
				exclusive_factor += float32 (num_patterns - patterns_found_with_class) / (num_patterns - 1);
			}
		}

		delete[] classCoverageMeanArray;
		delete[] patternIDArray;

		rate = exclusive_factor / num_classes;
	}

	return rate;
}

const float32 PatternList::GetPairMeanClassCoverageRate (const TransactionList *pTransactionList) const
{
	LOGMSG (HIGH_LEVEL, "PatternList::GetPairMeanClassCoverageRate () - begin\n");

	float32	rate		= 0		;
	uint32	num_patterns	= GetSize ()	;

	if (num_patterns > 1)
	{
		STLPatternList_cit itPatternEnd = GetEnd ();

		for (STLPatternList_cit itLeftPattern = GetBegin (); itLeftPattern != itPatternEnd; ++itLeftPattern)
		{
			const Pattern *pLeftPattern = static_cast<const Pattern *>(*itLeftPattern);

			for (STLPatternList_cit itRightPattern = itLeftPattern+1; itRightPattern != itPatternEnd; ++itRightPattern)
			{
				const Pattern *pRightPattern = static_cast<const Pattern *>(*itRightPattern);

				uint32	num_classes	= 0;
				float32	pattern_rate	= 0;

				for (uint32 classID = 0; classID < Class::GetMaxClassID () + 1; classID++)
				{
					uint32	leftCoverage	= mClassPatternCoverageMatrix [classID][pLeftPattern->GetPatternID ()];
					uint32	rightCoverage	= mClassPatternCoverageMatrix [classID][pRightPattern->GetPatternID ()];

					if (leftCoverage || rightCoverage)
					{
						num_classes++;

						if (leftCoverage > rightCoverage)
							pattern_rate += (float32) (leftCoverage - rightCoverage) / leftCoverage;
						else if (rightCoverage > leftCoverage)
							pattern_rate += (float32) (rightCoverage - leftCoverage) / rightCoverage;
					}
				}

				pattern_rate /= num_classes;

				rate += pattern_rate;
			}
		}

		rate /= (num_patterns * (num_patterns - 1) / 2);
	}

	return rate;
}

const float32 PatternList::GetRate (const TransactionList *pTransactionList, const OrtMetric &metric)
{
	float32 rate = -1;

	switch (metric)
	{
		case METRIC_SET_SIMILARITY:
			rate = GetSetSimilarityRate ();
			break;
		case METRIC_SET_COVERAGE:
			rate = GetSetCoverageRate (pTransactionList);
			break;
		case METRIC_SET_SIM_COV:
			rate = GetSetSimilarityRate () * GetSetCoverageRate (pTransactionList);
			break;
		case METRIC_SET_CLASS_COVERAGE:
			rate = GetSetClassCoverageRate (pTransactionList);
			break;
		case METRIC_PAIR_MEAN_CLASS_COVERAGE:
			rate = GetPairMeanClassCoverageRate (pTransactionList);
			break;
		case METRIC_ALL:
			rate = GetSetSimilarityRate () * GetSetCoverageRate (pTransactionList) * GetSetClassCoverageRate (pTransactionList) * GetPairMeanClassCoverageRate (pTransactionList);
			break;
		case METRIC_UNKNOWN:
		default:
			LOGMSG (NO_DEBUG, "PatternList::GetRate () - unknown orthogonality metric\n");
			break;
	}

	return rate;
}

void PatternList::MakeItemPatternCoverageMatrix (const PatternList *pPatternList)
{
	uint32	num_items 	= Item::GetMaxItemID () + 1;
	uint32	num_patterns	= Pattern::GetMaxPatternID () + 1;

	LOGMSG (LOW_LEVEL, "PatternList::MakeItemPatternCoverageMatrix () - num_items [%u], num_patterns [%u]\n", num_items, num_patterns);

	mItemPatternCoverageMatrix = new bool* [num_items];

	for (uint32 i = 0; i < num_items; i++)
	{
		mItemPatternCoverageMatrix [i] = new bool [num_patterns];

		for (uint32 j = 0; j < num_patterns; j++)
			mItemPatternCoverageMatrix [i][j] = false;
	}

	PatternList::STLPatternList_cit		itPatternEnd		= pPatternList->GetEnd ()	;

	for (PatternList::STLPatternList_cit itPattern = pPatternList->GetBegin (); itPattern != itPatternEnd; itPattern++)
	{
		const Pattern*			pPattern	= static_cast<const Pattern*>(*itPattern);
		const uint32&			patternID	= pPattern->GetPatternID ();
		ItemList::STLItemList_cit	itItemEnd	= pPattern->GetEnd ();

		for (ItemList::STLItemList_cit itItem = pPattern->GetBegin (); itItem != itItemEnd; itItem++)
			mItemPatternCoverageMatrix [static_cast<const Item *>(*itItem)->GetItemID ()][patternID] = true;
	}

	LOGMSG (LOW_LEVEL, "PatternList::MakeItemPatternCoverageMatrix () - end\n");
}

void PatternList::MakeTransactionPatternCoverageMatrix (const TransactionList *pTransactionList, const PatternList *pPatternList)
{
	uint32	num_transactions 	= Transaction::GetMaxTransactionID () + 1;
	uint32	num_patterns		= Pattern::GetMaxPatternID () + 1;

	LOGMSG (LOW_LEVEL, "PatternList::MakeTransactionPatternCoverageMatrix () - num_transactions [%u], num_patterns [%u]\n", num_transactions, num_patterns);

	mTransactionPatternCoverageMatrix = new bool* [num_transactions];

	for (uint32 i = 0; i < num_transactions; i++)
	{
		mTransactionPatternCoverageMatrix [i] = new bool [num_patterns];

		for (uint32 j = 0; j < num_patterns; j++)
			mTransactionPatternCoverageMatrix [i][j] = false;
	}

	TransactionList::STLTransactionList_cit	itTransactionEnd	= pTransactionList->GetEnd ()	;
	PatternList::STLPatternList_cit		itPatternEnd		= pPatternList->GetEnd ()	;

	for (TransactionList::STLTransactionList_cit itTransaction = pTransactionList->GetBegin (); itTransaction != itTransactionEnd; itTransaction++)
	{
		for (PatternList::STLPatternList_cit itPattern = pPatternList->GetBegin (); itPattern != itPatternEnd; itPattern++)
		{
			const Transaction*	pTransaction	= static_cast<const Transaction *>(*itTransaction);
			const Pattern*		pPattern	= static_cast<const Pattern*>(*itPattern);

			if (pTransaction->IsCoveredBy (pPattern))
				mTransactionPatternCoverageMatrix [pTransaction->GetTransactionID ()][pPattern->GetPatternID ()] = true;
		}
	}

	LOGMSG (LOW_LEVEL, "PatternList::MakeTransactionPatternCoverageMatrix () - end\n");
}

void PatternList::MakeClassPatternCoverageMatrix (const TransactionList *pTransactionList, const PatternList *pPatternList)
{
	uint32	num_classes 	= Class::GetMaxClassID () + 1;
	uint32	num_patterns	= Pattern::GetMaxPatternID () + 1;

	LOGMSG (LOW_LEVEL, "PatternList::MakeClassPatternCoverageMatrix () - num_classes [%u], num_patterns [%u]\n", num_classes, num_patterns);

	mClassPatternCoverageMatrix = new uint32* [num_classes];

	for (uint32 i = 0; i < num_classes; i++)
	{
		mClassPatternCoverageMatrix [i] = new uint32 [num_patterns];

		for (uint32 j = 0; j < num_patterns; j++)
			mClassPatternCoverageMatrix [i][j] = 0;
	}

	TransactionList::STLTransactionList_cit	itTransactionEnd	= pTransactionList->GetEnd ()	;
	PatternList::STLPatternList_cit		itPatternEnd		= pPatternList->GetEnd ()	;

	for (TransactionList::STLTransactionList_cit itTransaction = pTransactionList->GetBegin (); itTransaction != itTransactionEnd; itTransaction++)
	{
		for (PatternList::STLPatternList_cit itPattern = pPatternList->GetBegin (); itPattern != itPatternEnd; itPattern++)
		{
			const Transaction*	pTransaction	= static_cast<const Transaction *>(*itTransaction);
			const Pattern*		pPattern	= static_cast<const Pattern*>(*itPattern);

			if (pTransaction->IsCoveredBy (pPattern))
				mClassPatternCoverageMatrix [pTransaction->GetClassID ()][pPattern->GetPatternID ()]++;
		}
	}

	LOGMSG (LOW_LEVEL, "PatternList::MakeClassPatternCoverageMatrix () - end\n");
}

void PatternList::DestroyItemPatternCoverageMatrix ()
{
	uint32 num_items = Item::GetMaxItemID () + 1;

	for (uint32 i = 0; i < num_items; i++)
		delete[] mItemPatternCoverageMatrix [i];

	delete[] mItemPatternCoverageMatrix;
}

void PatternList::DestroyTransactionPatternCoverageMatrix ()
{
	uint32 num_transactions = Transaction::GetMaxTransactionID () + 1;

	for (uint32 i = 0; i < num_transactions; i++)
		delete[] mTransactionPatternCoverageMatrix [i];

	delete[] mTransactionPatternCoverageMatrix;
}

void PatternList::DestroyClassPatternCoverageMatrix ()
{
	uint32 num_classes = Class::GetMaxClassID () + 1;

	for (uint32 i = 0; i < num_classes; i++)
		delete[] mClassPatternCoverageMatrix [i];

	delete[] mClassPatternCoverageMatrix;
}
