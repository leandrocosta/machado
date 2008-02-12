#include "PatternList.h"
#include "TransactionList.h"
#include "ItemList.h"
#include "RankingRuleList.h"
#include "RankingRule.h"
#include "ClassList.h"
#include "DataBaseException.h"
#include "base/Logger.h"


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

	return pOrthogonalPatternList;
}

PatternList* PatternList::GetOrthogonalPatternListHeuristical (const TransactionList *pTransactionList, const OrtMetric &metric)
{
	LOGMSG (LOW_LEVEL, "PatternList::GetOrthogonalPatternListHeuristical () - begin\n");

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

			LOGMSG (LOW_LEVEL, "PatternList::GetOrthogonalPatternListHeuristical () - elements [%u], rate [%f]\n", orthogonal_size, rate_prv);

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
	STLPatternList_cit it = GetBegin ();

	Pattern *pPatternRet = static_cast<Pattern *>(*it);

	float32 similarity = pPatternRet->GetSimilarity (pPattern);

	STLPatternList_cit itEnd = GetEnd ();

	for (++it; it != itEnd; ++it)
	{
		Pattern *pPattern2 = static_cast<Pattern *>(*it);

		if (pPattern2->GetSimilarity (pPattern) > similarity)
			pPatternRet = pPattern2;
	}

	return pPatternRet;
}

const float32 PatternList::GetSimilarityRate ()
{
	LOGMSG (HIGH_LEVEL, "PatternList::GetSimilarityRate () - begin\n");

	float32 rate	= 0;

	if (GetSize () > 1)
	{
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
	}

	return rate;
}



const float32 PatternList::GetCoverageRate (const TransactionList *pTransactionList) const
{
	LOGMSG (HIGH_LEVEL, "PatternList::GetCoverageRate () - begin\n");

	float32 rate	= 0;

	if (GetSize () > 1)
	{
		uint32	distinct_transactions	= 0		;
		float32 exclusive_factor	= 0.0		;
//		float32 num_coverages		= 0.0		;
		uint32	num_patterns		= GetSize ()	;

		TransactionList::STLTransactionList_cit itTransactionEnd = pTransactionList->GetEnd ();

		for (TransactionList::STLTransactionList_cit itTransaction = pTransactionList->GetBegin (); itTransaction != itTransactionEnd; ++itTransaction)
		{
			Transaction *pTransaction = static_cast<Transaction *>(*itTransaction);

			uint32 patterns_found_in_transaction = 0;

			STLPatternList_cit itPatternEnd = GetEnd ();

			for (STLPatternList_cit itPattern = GetBegin (); itPattern != itPatternEnd; ++itPattern)
				if (pTransaction->IsCoveredBy (static_cast<const Pattern *>(*itPattern)))
					patterns_found_in_transaction++;

			if (patterns_found_in_transaction)
			{
				distinct_transactions++;
				exclusive_factor += float32 (num_patterns - patterns_found_in_transaction) / (num_patterns - 1);
			}

//			num_coverages += patterns_found_in_transaction;
		}

		rate = exclusive_factor / distinct_transactions;
//		rate = (float32) (exclusive_factor / distinct_transactions) * ((float32) num_coverages / (GetSize () * pTransactionList->GetSize ()));
//		rate = (float32) (exclusive_factor / distinct_transactions) * log ((float32) num_coverages / (GetSize () * pTransactionList->GetSize ()));
	}

	return rate;
}

const float32 PatternList::GetClassCoverageRate (const TransactionList *pTransactionList) const
{
	LOGMSG (HIGH_LEVEL, "PatternList::GetClassCoverageRate () - begin\n");

	float32 rate	= 0;

	if (GetSize () > 1)
	{
		STLPatternList_cit itPatternEnd = GetEnd ();

		for (STLPatternList_cit itPattern = GetBegin (); itPattern != itPatternEnd; ++itPattern)
		{
			Pattern *pPattern = static_cast<Pattern *>(*itPattern);

			pPattern->ResetClassCoverage ();
		}

		hash_map<string, uint32>	coveragesHash;

		TransactionList::STLTransactionList_cit itTransactionEnd = pTransactionList->GetEnd ();

		for (TransactionList::STLTransactionList_cit itTransaction = pTransactionList->GetBegin (); itTransaction != itTransactionEnd; ++itTransaction)
		{
			Transaction *pTransaction = static_cast<Transaction *>(*itTransaction);

			STLPatternList_cit itPatternEnd = GetEnd ();

			for (STLPatternList_cit itPattern = GetBegin (); itPattern != itPatternEnd; ++itPattern)
			{
				Pattern *pPattern = static_cast<Pattern *>(*itPattern);

				if (pTransaction->IsCoveredBy (pPattern))
				{
					pPattern->IncClassCoverage (pTransaction->GetClassValue ());
					coveragesHash [pTransaction->GetClassValue ()]++;
				}
			}
		}

		hash_map<string, uint32>::const_iterator it;

		for (it = coveragesHash.begin (); it != coveragesHash.end (); ++it)
		{
			const string class_name = it->first;

			uint32 coverage_max = 0;
			uint32 coverage_min = 0;

			STLPatternList_cit itPatternEnd = GetEnd ();

			for (STLPatternList_cit itPattern = GetBegin (); itPattern != itPatternEnd; ++itPattern)
			{
				Pattern *pPattern = static_cast<Pattern *>(*itPattern);

				uint32 coverage = pPattern->GetClassCoverage (class_name);

				if (coverage >= coverage_max)
				{
					coverage_min = coverage_max;
					coverage_max = coverage;
				}
				else if (coverage > coverage_min)
					coverage_min = coverage;
			}

			LOGMSG (HIGH_LEVEL, "PatternList::GetClassCoverageRate () - class [%s], coverages [%u], coverage_max [%u], coverage_min [%u]\n", class_name.c_str (), it->second, coverage_max, coverage_min);

			if (coverage_max)
				rate += ((float32) (coverage_max - coverage_min) / it->second);
		}

		rate /= coveragesHash.size ();
	}

	return rate;
}

const float32 PatternList::GetClassCoverageMeanRate (const TransactionList *pTransactionList) const
{
	LOGMSG (HIGH_LEVEL, "PatternList::GetClassCoverageMeanRate () - begin\n");

	float32 rate = 0;

	if (GetSize () > 1)
	{
		STLPatternList_cit itPatternEnd = GetEnd ();

		for (STLPatternList_cit itLeftPattern = GetBegin (); itLeftPattern != itPatternEnd; ++itLeftPattern)
		{
			const Pattern *pLeftPattern = static_cast<const Pattern *>(*itLeftPattern);

			for (STLPatternList_cit itRightPattern = itLeftPattern+1; itRightPattern != itPatternEnd; ++itRightPattern)
			{
				const Pattern *pRightPattern = static_cast<const Pattern *>(*itRightPattern);

				uint32	num	= 0;
				uint32	den	= 0;

				TransactionList::STLTransactionList_cit itTransactionEnd = pTransactionList->GetEnd ();

				for (TransactionList::STLTransactionList_cit itTransaction = pTransactionList->GetBegin (); itTransaction != itTransactionEnd; ++itTransaction)
				{
					Transaction *pTransaction = static_cast<Transaction *>(*itTransaction);

					bool bLeftCoverage	= pTransaction->IsCoveredBy (pLeftPattern);
					bool bRightCoverage	= pTransaction->IsCoveredBy (pRightPattern);

					if (bLeftCoverage || bRightCoverage)
					{
						den++;

						if (! bLeftCoverage || ! bRightCoverage)
							num++;
					}
				}

				rate += (float32) num / den;
			}
		}

		rate /= (GetSize () * (GetSize () - 1) / 2);
	}

	return rate;
}

const float32 PatternList::GetRate (const TransactionList *pTransactionList, const OrtMetric &metric)
{
	float32 rate = -1;

	switch (metric)
	{
		case METRIC_SIMILARITY:
			rate = GetSimilarityRate ();
			break;
		case METRIC_COVERAGE:
			rate = GetCoverageRate (pTransactionList);
			break;
		case METRIC_SIM_COV:
			rate = GetSimilarityRate () * GetCoverageRate (pTransactionList);
			break;
		case METRIC_CLASS_COVERAGE:
			rate = GetClassCoverageRate (pTransactionList);
			break;
		case METRIC_CLASS_MEAN_COVERAGE:
			rate = GetClassCoverageMeanRate (pTransactionList);
			break;
		case METRIC_ALL:
			rate = GetSimilarityRate () * GetCoverageRate (pTransactionList) * GetClassCoverageMeanRate (pTransactionList);
			break;
		case METRIC_UNKNOWN:
		default:
			LOGMSG (NO_DEBUG, "PatternList::GetRate () - unknown orthogonality metric\n");
			break;
	}

	return rate;
}
