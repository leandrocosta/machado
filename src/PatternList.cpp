#include "PatternList.h"
#include "TransactionList.h"
#include "ItemList.h"
#include "RuleList.h"
#include "ClassList.h"
#include "AppOptions.h"
#include "base/Logger.h"


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

PatternList* PatternList::GetOrthogonalPatternList (const TransactionList *pTransactionList)
{
	PatternList *pOrthogonalPatternList = NULL;

	switch (AppOptions::GetInstance ()->GetOrtMode ())
	{
		case ORTH_HEURISTICAL:
			pOrthogonalPatternList = GetOrthogonalPatternListHeuristical (pTransactionList);
			break;
		case ORTH_POLYNOMIAL:
			pOrthogonalPatternList = GetOrthogonalPatternListPolynomial (pTransactionList);
			break;
		default:
			LOGMSG (NO_DEBUG, "PatternList::GetOrthogonalPatternList () - unknown orthogonality mode\n");
			break;
	}

	return pOrthogonalPatternList;
}

PatternList* PatternList::GetOrthogonalPatternListHeuristical (const TransactionList *pTransactionList)
{
	LOGMSG (LOW_LEVEL, "PatternList::GetOrthogonalPatternListHeuristical () - begin\n");

	PatternList *pOrthogonalPatternList = new PatternList ();

	uint64 size = GetSize ();

	if (size <= 2)
	{
		if (size > 0)
			pOrthogonalPatternList->PushBack (GetAt (0));

		if (size > 1)
			pOrthogonalPatternList->PushBack (GetAt (1));
	}
	else
	{
		ReverseSort ();

		Pattern *pPatternAt0 = static_cast<Pattern *>(GetAt (0));
		Pattern *pPatternAt1 = static_cast<Pattern *>(GetAt (1));

		pOrthogonalPatternList->PushBack (pPatternAt0);
		pOrthogonalPatternList->PushBack (pPatternAt1);

		pPatternAt0->SetGot (true);
		pPatternAt1->SetGot (true);

		float32 rate_prv = pOrthogonalPatternList->GetRate (pTransactionList);

		STLPatternList_cit itEnd = GetEnd ();

		for (STLPatternList_cit it = GetBegin (); it != itEnd; it++)
		{
			Pattern *pCandToGetInPattern = static_cast<Pattern *>(*it);

			if (! pCandToGetInPattern->GetGot ())
			{
				Pattern *pCandToGetOutPattern = pOrthogonalPatternList->GetMoreSimilar (pCandToGetInPattern);

				pOrthogonalPatternList->Remove (pCandToGetOutPattern);
				pOrthogonalPatternList->PushBack (pCandToGetInPattern);

				float32 rate_new = pOrthogonalPatternList->GetRate (pTransactionList);

				if (rate_new > rate_prv)
				{
					pCandToGetInPattern->SetGot (true);
					pCandToGetOutPattern->SetGot (false);

					rate_prv = rate_new;
				}
				else
				{
					pOrthogonalPatternList->Remove (pCandToGetInPattern);
					pOrthogonalPatternList->PushBack (pCandToGetOutPattern);
				}
			}
		}

		float32 rate_result;
		float32 rate_now = rate_prv;

		do
		{
			rate_result = rate_now;

			PatternList *pTryPatternList = new PatternList ();

			STLPatternList_cit itEnd = pOrthogonalPatternList->GetEnd ();

			for (STLPatternList_cit it = pOrthogonalPatternList->GetBegin (); it != itEnd; it++)
				pTryPatternList->PushBack (*it);

			// adiciona um novo elemento ao try
			itEnd = GetEnd ();

			for (STLPatternList_cit it = GetBegin (); it != itEnd; it++)
			{
				Pattern *pCandToGetInPattern = static_cast<Pattern *>(*it);

				if (! pCandToGetInPattern->GetGot ())
				{
					pTryPatternList->PushBack (pCandToGetInPattern);
					break;
				}
			}

			rate_prv = pTryPatternList->GetRate (pTransactionList);

			// tenta obter o melhor conjunto

			itEnd = GetEnd ();

			for (STLPatternList_cit it = GetBegin (); it != itEnd; it++)
			{
				Pattern *pCandToGetInPattern = static_cast<Pattern *>(*it);

				if (! pCandToGetInPattern->GetGot ())
				{
					Pattern *pCandToGetOutPattern = pTryPatternList->GetMoreSimilar (pCandToGetInPattern);

					pTryPatternList->Remove (pCandToGetOutPattern);
					pTryPatternList->PushBack (pCandToGetInPattern);

					float32 rate_new = pTryPatternList->GetRate (pTransactionList);

					if (rate_new > rate_prv)
					{
						pCandToGetInPattern->SetGot (true);
						pCandToGetOutPattern->SetGot (false);

						rate_prv = rate_new;
					}
					else
					{
						pTryPatternList->Remove (pCandToGetInPattern);
						pTryPatternList->PushBack (pCandToGetOutPattern);
					}
				}
			}

			// atualiza o rate_now com a métrica obtida
			rate_now = rate_prv;

			if (rate_now >= rate_result)
			{
				LOGMSG (LOW_LEVEL, "PatternList::GetOrthogonalPatternListHeuristical () - add one more element - rate_result [%f], rate_now [%f]\n", rate_result, rate_now);
				// atualiza resultado com try

				pOrthogonalPatternList->RemoveAll ();
				delete pOrthogonalPatternList;
				pOrthogonalPatternList = pTryPatternList;
			}
		} while (rate_now >= rate_result);
	}

	return pOrthogonalPatternList;
}

PatternList* PatternList::GetOrthogonalPatternListPolynomial (const TransactionList *pTransactionList)
{
	uint64 size = GetSize ();

	LOGMSG (LOW_LEVEL, "PatternList::GetOrthogonalPatternListPolynomial () - patterns [%llu]\n", size);

	PatternList *pOrthogonalPatternList = new PatternList ();

	Pattern *pPattern1 = NULL;
	Pattern *pPattern2 = NULL;

	if (size <= 2)
	{
		if (size > 0)
			pPattern1 = static_cast<Pattern *>(GetAt (0));

		if (size > 1)
			pPattern2 = static_cast<Pattern *>(GetAt (1));
	}
	else
	{
		float32 rate_prv = -1;

		STLPatternList_cit itEnd = GetEnd ();

		for (STLPatternList_cit it_i = GetBegin (); it_i < itEnd-1; it_i++)
		{
			for (STLPatternList_cit it_j = it_i+1; it_j != itEnd; it_j++)
			{
				pOrthogonalPatternList->PushBack (*it_i);
				pOrthogonalPatternList->PushBack (*it_j);

				float32 rate_new = pOrthogonalPatternList->GetRate (pTransactionList);

				if (rate_new > rate_prv)
				{
					pPattern1 = static_cast<Pattern *>(*it_i);
					pPattern2 = static_cast<Pattern *>(*it_j);

					rate_prv = rate_new;
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

RuleList* PatternList::GetRuleList (const ClassList *pClassList, const float32 &confidence, const uint64 &projection_size) const
{
	RuleList *pRuleList = new RuleList ();

	ClassList::STLClassList_cit itClassEnd = pClassList->GetEnd ();

	for (ClassList::STLClassList_cit itClass = pClassList->GetBegin (); itClass != itClassEnd; itClass++)
	{
		Class *pClass = static_cast<Class *>(*itClass);

		LOGMSG (MEDIUM_LEVEL, "DataBase::GetRuleList () - check class [%s]\n", pClass->GetValue ().c_str ());

		STLPatternList_cit itPatternEnd = GetEnd ();

		for (STLPatternList_cit itPattern = GetBegin (); itPattern != itPatternEnd; itPattern++)
		{
			Pattern *pPattern = static_cast<Pattern *>(*itPattern);

			Rule *pRule = new Rule (pClass, pPattern, projection_size, pClassList->GetSize ());

			if (pRule->GetConfidence () >= confidence)
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

const uint64 PatternList::GetSumPatternLen () const
{
	uint64 sumPatternLen = 0;

	STLPatternList_cit	it			;
	STLPatternList_cit	itEnd = GetEnd ()	;

	for (it = GetBegin (); it != itEnd; it++)
	{
		sumPatternLen += static_cast<const Pattern *>(*it)->GetSize ();
	}

	return sumPatternLen;
}

const uint64& PatternList::GetMaxPatternLen () const
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

const float32 PatternList::GetSimilarityRate ()
{
	LOGMSG (MEDIUM_LEVEL, "PatternList::GetSimilarityRate () - begin\n");

	ItemList	totalItemList		;
	uint32		num_items	= 0	;

	STLPatternList_cit itEnd = GetEnd ();

	for (STLPatternList_cit it = GetBegin (); it != itEnd; ++it)
	{
		const Pattern *pPattern = static_cast<const Pattern *>(*it);

		STLPatternList_cit itPatternEnd = pPattern->GetEnd ();

		for (ItemList::STLItemList_cit itItem = pPattern->GetBegin (); itItem != itPatternEnd; itItem++)
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

	LOGMSG (MEDIUM_LEVEL, "PatternList::PatternList () - max_pattern_len [%u], num_patterns [%llu], num_items [%u], distinct_items [%u], exclusive_items [%f]\n", mMaxPatternLen, GetSize (), num_items, distinct_items, exclusive_items);

	float32 rate = (float32) (exclusive_items / distinct_items);
//      rate = (float32) (exclusive_items / distinct_items) * ((float32) num_items / (mMaxPatternLen * GetSize ()));
//	rate = (float32) (exclusive_items / distinct_items) * log ((float32) num_items / (mMaxPatternLen * GetSize ()));

	totalItemList.RemoveAll ();

	return rate;
}



const float32 PatternList::GetCoverageRate (const TransactionList *pTransactionList) const
{
	LOGMSG (MEDIUM_LEVEL, "PatternList::GetCoverageRate () - begin\n");

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
			if (pTransaction->IsCoveredBy (static_cast<const Pattern *>(*itPattern)))
				patterns_found_in_transaction++;

		if (patterns_found_in_transaction)
		{
			distinct_transactions++;
			exclusive_factor += float32 (num_patterns - patterns_found_in_transaction) / (num_patterns - 1);
		}

//		num_coverages += patterns_found_in_transaction;
	}

	float32 rate = exclusive_factor / distinct_transactions;
 //	rate = (float32) (exclusive_factor / distinct_transactions) * ((float32) num_coverages / (GetSize () * pTransactionList->GetSize ()));
//	rate = (float32) (exclusive_factor / distinct_transactions) * log ((float32) num_coverages / (GetSize () * pTransactionList->GetSize ()));

	return rate;
}

const float32 PatternList::GetClassCoverageRate (const TransactionList *pTransactionList) const
{
	LOGMSG (MEDIUM_LEVEL, "PatternList::GetClassCoverageRate () - begin\n");

	STLPatternList_cit itPatternEnd = GetEnd ();

	for (STLPatternList_cit itPattern = GetBegin (); itPattern != itPatternEnd; itPattern++)
	{
		Pattern *pPattern = static_cast<Pattern *>(*itPattern);

		pPattern->ResetClassCoverage ();
	}

	hash_map<string, uint32>	coveragesHash;

	TransactionList::STLTransactionList_cit itTransactionEnd = pTransactionList->GetEnd ();

	for (TransactionList::STLTransactionList_cit itTransaction = pTransactionList->GetBegin (); itTransaction != itTransactionEnd; itTransaction++)
	{
		const Transaction *pTransaction = static_cast<const Transaction *>(*itTransaction);

		STLPatternList_cit itPatternEnd = GetEnd ();

		for (STLPatternList_cit itPattern = GetBegin (); itPattern != itPatternEnd; itPattern++)
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

	float32 rate = 0.0;

	for (it = coveragesHash.begin (); it != coveragesHash.end (); it++)
	{
		const string class_name = it->first;
	
		uint32 coverage_max = 0;
		uint32 coverage_min = 0;

		STLPatternList_cit itPatternEnd = GetEnd ();

		for (STLPatternList_cit itPattern = GetBegin (); itPattern != itPatternEnd; itPattern++)
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

	return rate;
}

const float32 PatternList::GetRate (const TransactionList *pTransactionList)
{
	float32 rate = -1;

	switch (AppOptions::GetInstance ()->GetOrtMetric ())
	{
		case METRIC_SIMILARITY:
			rate = GetSimilarityRate ();
			break;
		case METRIC_COVERAGE:
			rate = GetCoverageRate (pTransactionList);
			break;
		case METRIC_BOTH:
			rate = GetSimilarityRate () * GetCoverageRate (pTransactionList);
			break;
		case METRIC_CLASS_COVERAGE:
			rate = GetClassCoverageRate (pTransactionList);
			break;
		case METRIC_UNKNOWN:
		default:
			LOGMSG (NO_DEBUG, "PatternList::GetRate () - unknown orthogonality metric\n");
			break;
	}

	return rate;
}
