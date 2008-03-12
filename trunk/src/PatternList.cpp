#include "PatternList.h"
#include "TransactionList.h"
#include "ItemList.h"
#include "RankingRuleList.h"
#include "RankingRule.h"
#include "ClassList.h"
#include "DataBaseException.h"
#include "base/Rand.h"
#include "base/Logger.h"


bool**		PatternList::mItemPatternCoverageMatrix		= NULL;
bool**		PatternList::mTransactionPatternCoverageMatrix	= NULL;
uint32**	PatternList::mClassPatternCoverageMatrix	= NULL;


PatternList::PatternList (const uint64 &max_size) : ObjectList (max_size)
{

}

PatternList::~PatternList ()
{

}

void PatternList::MakeOrdering (const OrtOrdering &ordering)
{
	switch (ordering)
	{
		case ORDERING_SORTED:
			Sort ();
			break;
		case ORDERING_REVERSE_SORTED:
			ReverseSort ();
			break;
		case ORDERING_SIZE_SORTED:
			SortBySize ();
			break;
		case ORDERING_SIZE_REVERSE_SORTED:
			ReverseSortBySize ();
			break;
		case ORDERING_NONE:
			break;
		case ORDERING_UNKNOWN:
		default:
			LOGMSG (NO_DEBUG, "PatternList::MakeOrdering () - unknown pattern ordering\n");
			throw DataBaseException ("Unknown pattern ordering");
			break;
	}
}

Pattern* PatternList::GetRandomPattern () const
{
	Pattern *pPattern = NULL;

	uint32 size = GetSize ();

	if (size)
		pPattern = static_cast<Pattern *>(GetAt (Rand::GetRandomNumber (0, size)));

	return pPattern;
}

const uint32 PatternList::GetResidue (const PatternList *pPatternList, const Pattern::OrtMetric &rMetric, const float32 &rBeta) const
{
	uint32 residue = 0;

	STLPatternList_cit itEnd = GetEnd ();

	for (STLPatternList_cit it = GetBegin (); it != itEnd; ++it)
	{
		const Pattern *pPattern = static_cast<const Pattern *>(*it);

		if (! pPatternList->FindByPtr (pPattern))
		{
			const Pattern *pMoreSimilarPattern = pPatternList->GetMoreSimilar (pPattern, rMetric);

			const float32 similarity = pMoreSimilarPattern->GetSimilarity (pPattern, rMetric);

			if (similarity < rBeta)
				residue ++;
		}
	}

	return residue;
}

PatternList* PatternList::GetOrthogonalPatternList (const TransactionList *pTransactionList, const OrtMode &mode, const OrtMethod &method, const Pattern::OrtMetric &rMetric, const OrtOrdering &ordering, const float32 &rAlpha, const float32 &rBeta, float32 &rRate)
{
	PatternList *pOrthogonalPatternList = NULL;

	switch (rMetric)
	{
		case Pattern::METRIC_SIMILARITY:
			MakeItemPatternCoverageMatrix (this);
			break;
		case Pattern::METRIC_TRANS_COVERAGE:
			MakeTransactionPatternCoverageMatrix (pTransactionList, this);
			break;
		case Pattern::METRIC_CLASS_COVERAGE:
			MakeClassPatternCoverageMatrix (pTransactionList, this);
			break;
		case Pattern::METRIC_ALL:
			MakeItemPatternCoverageMatrix (this);
			MakeTransactionPatternCoverageMatrix (pTransactionList, this);
			MakeClassPatternCoverageMatrix (pTransactionList, this);
			break;
		default:
			LOGMSG (NO_DEBUG, "PatternList::GetOrthogonalPatternList () - unknown orthogonality metric\n");
			throw DataBaseException ("Unknown orthogonality metric");
			break;
	}

	switch (mode)
	{
		case ORTH_HEURISTICAL:
//			if (rMetric == Pattern::METRIC_CLASS_COVERAGE)
//				pOrthogonalPatternList = GetOrthogonalPatternListClassHeuristical (pTransactionList, method, rMetric, ordering, rRate);
//			else
				pOrthogonalPatternList = GetOrthogonalPatternListHeuristical (pTransactionList, method, rMetric, ordering, rRate);
			break;
		case ORTH_POLYNOMIAL:
			pOrthogonalPatternList = GetOrthogonalPatternListPolynomial (pTransactionList, method, rMetric, rRate);
			break;
		case ORTH_ORIGAMI:
			pOrthogonalPatternList = GetOrthogonalPatternListORIGAMI (pTransactionList, rMetric, rAlpha, rBeta);
			rRate = (float32) 1 - rAlpha;
			break;
		default:
			LOGMSG (NO_DEBUG, "PatternList::GetOrthogonalPatternList () - unknown orthogonality mode\n");
			throw DataBaseException ("Unknown orthogonality mode");
			break;
	}

	switch (rMetric)
	{
		case Pattern::METRIC_SIMILARITY:
			DestroyItemPatternCoverageMatrix ();
			break;
		case Pattern::METRIC_TRANS_COVERAGE:
			DestroyTransactionPatternCoverageMatrix ();
			break;
		case Pattern::METRIC_CLASS_COVERAGE:
			DestroyClassPatternCoverageMatrix ();
			break;
		case Pattern::METRIC_ALL:
			DestroyItemPatternCoverageMatrix ();
			DestroyTransactionPatternCoverageMatrix ();
			DestroyClassPatternCoverageMatrix ();
			break;
		default:
			LOGMSG (NO_DEBUG, "PatternList::GetOrthogonalPatternList () - unknown orthogonality metric\n");
			throw DataBaseException ("Unknown orthogonality metric");
			break;
	}

	return pOrthogonalPatternList;
}

PatternList* PatternList::GetOrthogonalPatternListHeuristical (const TransactionList *pTransactionList, const OrtMethod &method, const Pattern::OrtMetric &rMetric, const OrtOrdering &ordering, float32 &rRate)
{
	uint32 num_patterns = GetSize ();

	LOGMSG (LOW_LEVEL, "PatternList::GetOrthogonalPatternListHeuristical () - num_patterns [%u], rMetric [%c]\n", num_patterns, rMetric);

	PatternList *pOrthogonalPatternList = new PatternList ();

	if (num_patterns > 0)
	{
		float32	rate		= 0;
		float32	rate_new	= 0;

		PatternList *pTryPatternList = new PatternList ();

		MakeOrdering (ordering);

		Pattern *pPatternAt0 = static_cast<Pattern *>(GetAt (0));

		uint32 max_patterns = Pattern::GetNumPatterns ();
		bool *orthogonalPatternMatrix = new bool [max_patterns];
		for (uint32 i = 0; i < max_patterns; i++)
			orthogonalPatternMatrix [i] = false;

		pTryPatternList->PushBack (pPatternAt0);
		orthogonalPatternMatrix [pPatternAt0->GetPatternID ()] = true;

		do
		{
			pOrthogonalPatternList->RemoveAll ();

			STLPatternList_cit itEnd = pTryPatternList->GetEnd ();

			for (STLPatternList_cit it = pTryPatternList->GetBegin (); it != itEnd; ++it)
				pOrthogonalPatternList->PushBack (*it);

			rate = rate_new;

			uint32 orthogonal_size = pOrthogonalPatternList->GetSize ();

			LOGMSG (MEDIUM_LEVEL, "PatternList::GetOrthogonalPatternListHeuristical () - elements [%u], rate [%f]\n", orthogonal_size, rate);

			itEnd = GetEnd ();

			bool bGot = false;

			for (STLPatternList_cit it = GetBegin (); it != itEnd; ++it)
			{
				Pattern *pPattern = static_cast<Pattern *>(*it);

				if (! orthogonalPatternMatrix [pPattern->GetPatternID ()])
				{
					pTryPatternList->PushBack (pPattern);
					orthogonalPatternMatrix [pPattern->GetPatternID ()] = true;
					bGot = true;
					rate_new = pTryPatternList->GetRate (pTransactionList, method, rMetric);

					break;
				}
			}

			if (bGot)
			{
				for (STLPatternList_cit it = GetBegin (); it != itEnd; ++it)
				{
					Pattern *pCandToGetInPattern = static_cast<Pattern *>(*it);

					if (! orthogonalPatternMatrix [pCandToGetInPattern->GetPatternID ()])
					{
						Pattern *pCandToGetOutPattern = pTryPatternList->GetMoreSimilar (pCandToGetInPattern, rMetric);

						pTryPatternList->Remove (pCandToGetOutPattern);
						pTryPatternList->PushBack (pCandToGetInPattern);

						float32 rate_try = pTryPatternList->GetRate (pTransactionList, method, rMetric);

						if (rate_try > rate_new)
						{
							orthogonalPatternMatrix [pCandToGetInPattern->GetPatternID ()] = true;
							orthogonalPatternMatrix [pCandToGetOutPattern->GetPatternID ()] = false;

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
		} while (rate_new >= rate);

		pTryPatternList->RemoveAll ();
		delete pTryPatternList;
	
		delete[] orthogonalPatternMatrix;

		LOGMSG (NO_DEBUG, "PatternList::GetOrthogonalPatternListHeuristical () - rate [%f]\n", rate);
		rRate = rate;
	}

	uint32 orthogonal_size = pOrthogonalPatternList->GetSize ();

	LOGMSG (NO_DEBUG, "PatternList::GetOrthogonalPatternListHeuristical () - patterns [%u]\n", orthogonal_size);

	return pOrthogonalPatternList;
}

PatternList* PatternList::GetOrthogonalPatternListClassHeuristical (const TransactionList *pTransactionList, const OrtMethod &method, const Pattern::OrtMetric &rMetric, const OrtOrdering &ordering, float32 &rRate)
{
	uint32 num_patterns = GetSize ();

	LOGMSG (LOW_LEVEL, "PatternList::GetOrthogonalPatternListClassHeuristical () - num_patterns [%u], rMetric [%c]\n", num_patterns, rMetric);

	PatternList *pOrthogonalPatternList = new PatternList ();

	if (num_patterns > 0)
	{
		float32	rate		= 0;
		float32	rate_new	= 0;

		PatternList *pTryPatternList = new PatternList ();

		MakeOrdering (ordering);

		uint32 max_patterns = Pattern::GetNumPatterns ();
		bool *orthogonalPatternMatrix = new bool [max_patterns];
		for (uint32 i = 0; i < max_patterns; i++)
			orthogonalPatternMatrix [i] = false;

		STLPatternList_cit it = GetBegin ();
		STLPatternList_cit itEnd = GetEnd ();

		Pattern *pPattern0 = static_cast<Pattern *>(*(it++));
		float32 ambiguity = pPattern0->GetAmbiguity ();

		while (it != itEnd)
		{
			Pattern *pPattern = static_cast<Pattern *>(*(it++));

			float32 ambiguity_new = pPattern->GetAmbiguity ();

			if (ambiguity > ambiguity_new)
			{
				pPattern0 = pPattern;
				ambiguity = ambiguity_new;
			}
		}

		pTryPatternList->PushBack (pPattern0);
		orthogonalPatternMatrix [pPattern0->GetPatternID ()] = true;

		do
		{
			pOrthogonalPatternList->RemoveAll ();

			STLPatternList_cit itEnd = pTryPatternList->GetEnd ();

			for (STLPatternList_cit it = pTryPatternList->GetBegin (); it != itEnd; ++it)
				pOrthogonalPatternList->PushBack (*it);

			rate = rate_new;

			uint32 orthogonal_size = pOrthogonalPatternList->GetSize ();

			LOGMSG (MEDIUM_LEVEL, "PatternList::GetOrthogonalPatternListClassHeuristical () - elements [%u], rate [%f]\n", orthogonal_size, rate);

			itEnd = GetEnd ();

			bool bGot = false;

			for (STLPatternList_cit it = GetBegin (); it != itEnd; ++it)
			{
				Pattern *pPattern = static_cast<Pattern *>(*it);

				if (! orthogonalPatternMatrix [pPattern->GetPatternID ()])
				{
					pTryPatternList->PushBack (pPattern);
					orthogonalPatternMatrix [pPattern->GetPatternID ()] = true;
					bGot = true;
					rate_new = pTryPatternList->GetRate (pTransactionList, method, rMetric);

					break;
				}
			}

			if (bGot)
			{
				for (STLPatternList_cit it = GetBegin (); it != itEnd; ++it)
				{
					Pattern *pCandToGetInPattern = static_cast<Pattern *>(*it);

					if (! orthogonalPatternMatrix [pCandToGetInPattern->GetPatternID ()])
					{
						Pattern *pCandToGetOutPattern = pTryPatternList->GetMoreSimilar (pCandToGetInPattern, rMetric);

						pTryPatternList->Remove (pCandToGetOutPattern);
						pTryPatternList->PushBack (pCandToGetInPattern);

						float32 rate_try = pTryPatternList->GetRate (pTransactionList, method, rMetric);

						if (rate_try > rate_new)	// 1 best for iris and lymph
//						if (rate_try >= rate_new)	// 2
						{
							orthogonalPatternMatrix [pCandToGetInPattern->GetPatternID ()] = true;
							orthogonalPatternMatrix [pCandToGetOutPattern->GetPatternID ()] = false;

							rate_new = rate_try;
						}
						else if (rate_try == rate_new)
						{
							float32 ambiguity_in = pCandToGetInPattern->GetAmbiguity ();
							float32 ambiguity_out = pCandToGetOutPattern->GetAmbiguity ();

							if (ambiguity_in < ambiguity_out)
							{
								orthogonalPatternMatrix [pCandToGetInPattern->GetPatternID ()] = true;
								orthogonalPatternMatrix [pCandToGetOutPattern->GetPatternID ()] = false;
							}
							else
							{
								pTryPatternList->Remove (pCandToGetInPattern);
								pTryPatternList->PushBack (pCandToGetOutPattern);
							}
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
		} while (rate_new >= rate);

		pTryPatternList->RemoveAll ();
		delete pTryPatternList;
	
		delete[] orthogonalPatternMatrix;

		LOGMSG (NO_DEBUG, "PatternList::GetOrthogonalPatternListClassHeuristical () - rate [%f]\n", rate);
		rRate = rate;
	}

	uint32 orthogonal_size = pOrthogonalPatternList->GetSize ();

	LOGMSG (NO_DEBUG, "PatternList::GetOrthogonalPatternListClassHeuristical () - patterns [%u]\n", orthogonal_size);

	return pOrthogonalPatternList;
}

PatternList* PatternList::GetOrthogonalPatternListPolynomial (const TransactionList *pTransactionList, const OrtMethod &method, const Pattern::OrtMetric &rMetric, float32 &rRate)
{
	LOGMSG (LOW_LEVEL, "PatternList::GetOrthogonalPatternListPolynomial () - begin\n");

	PatternList *pOrthogonalPatternList = new PatternList ();

	if (GetSize () > 0)
	{
		PatternList *pTryPatternList = new PatternList ();

		float32	rate		= 0;
		float32	rate_try	= 0;
		uint32	num_patterns	= 1;

		do
		{
			pOrthogonalPatternList->RemoveAll ();
			delete pOrthogonalPatternList;
			pOrthogonalPatternList = pTryPatternList;
			rate = rate_try;

			if (num_patterns <= GetSize ())
			{
				pTryPatternList = GetOrthogonalPatternListPolynomial (pTransactionList, method, rMetric, num_patterns++, rate_try);
//				rate_try = pTryPatternList->GetRate (pTransactionList, method, rMetric);
			}
			else
				rate_try = 0;

			LOGMSG (LOW_LEVEL, "PatternList::GetOrthogonalPatternListPolynomial () - rate [%f], rate_try [%f]\n", rate, rate_try);
		} while (rate_try >= rate);

		LOGMSG (NO_DEBUG, "PatternList::GetOrthogonalPatternListPolynomial () - rate [%f]\n", rate);
		rRate = rate;
	}

	LOGMSG (LOW_LEVEL, "PatternList::GetOrthogonalPatternListPolynomial () - num_patterns [%u]\n", pOrthogonalPatternList->GetSize ());

	return pOrthogonalPatternList;
}

PatternList* PatternList::GetOrthogonalPatternListPolynomial (const TransactionList *pTransactionList, const OrtMethod &method, const Pattern::OrtMetric &rMetric, const uint32 &num_patterns, float32 &rRate)
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

		float32	rate		= 0	;
		bool	bKeepRunning	= true	;

		while (bKeepRunning)
		{
			PatternList *pTryPatternList = new PatternList ();

			for (uint32 i = 0; i < num_patterns; i++)
				pTryPatternList->PushBack (GetAt (iArray [i]));

			float32 rate_try = pTryPatternList->GetRate (pTransactionList, method, rMetric);

			if (rate_try > rate)
			{
				pOrthogonalPatternList->RemoveAll ();
				delete pOrthogonalPatternList;
				pOrthogonalPatternList = pTryPatternList;
				rate = rate_try;
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
	
		LOGMSG (LOW_LEVEL, "PatternList::GetOrthogonalPatternListPolynomial () - rate [%f]\n", rate);
		rRate = rate;
	}

	return pOrthogonalPatternList;
}

PatternList* PatternList::GetOrthogonalPatternListORIGAMI (const TransactionList *pTransactionList, const Pattern::OrtMetric &rMetric, const float32 &rAlpha, const float32 &rBeta)
{
	uint32 size = GetSize ();

	PatternList *pOrthogonalPatternList = GetOrthogonalPatternListORIGAMIRandomCandidate (pTransactionList, rMetric, rAlpha, rBeta);

	uint32 residue = GetResidue (pOrthogonalPatternList, rMetric, rBeta);

	uint32 runs = 0;

	while (runs < size && pOrthogonalPatternList->GetSize () < size)
	{
		PatternList *pOrthogonalPatternListTry = GetOrthogonalPatternListORIGAMIRandomCandidate (pTransactionList, rMetric, rAlpha, rBeta);
		uint32 residue_try = GetResidue (pOrthogonalPatternListTry, rMetric, rBeta);

		if (residue_try < residue)
		{
			pOrthogonalPatternList->RemoveAll ();
			delete pOrthogonalPatternList;
			pOrthogonalPatternList= pOrthogonalPatternListTry;
		}
		else
		{
			pOrthogonalPatternListTry->RemoveAll ();
			delete pOrthogonalPatternListTry;
		}

		runs++;
	}

	return pOrthogonalPatternList;
}

PatternList* PatternList::GetOrthogonalPatternListORIGAMIRandomCandidate (const TransactionList *pTransactionList, const Pattern::OrtMetric &rMetric, const float32 &rAlpha, const float32 &rBeta)
{
	uint32 size = GetSize ();

//	LOGMSG (NO_DEBUG, "PatternList::GetOrthogonalPatternListORIGAMIRandomCandidate () - size [%u], rAlpha [%f], rBeta [%f]\n", size, rAlpha, rBeta);

	PatternList *pOrthogonalPatternList = new PatternList ();

	const uint32 num_patterns = Pattern::GetNumPatterns ();
	bool *triedPatternArray = new bool [num_patterns];
	for (uint32 patternID = 0; patternID < num_patterns; patternID++)
		triedPatternArray [patternID] = false;

	if (size)
	{
		Pattern *pPattern = GetRandomPattern ();

		pOrthogonalPatternList->PushBack (pPattern);
		triedPatternArray [pPattern->GetPatternID ()] = true;
	}

	uint32 tries = 0;

	while (tries < size) 
	{
		Pattern *pPattern = NULL;
		
		for (uint32 i = 0; i < size; i++)
		{
			pPattern = GetRandomPattern ();

			if (triedPatternArray [pPattern->GetPatternID ()])
				pPattern = NULL;
			else
				break;
		}

		if (pPattern)
		{
			triedPatternArray [pPattern->GetPatternID ()] = true;

			bool bOrtho = true;

			PatternList::STLPatternList_cit itEnd = pOrthogonalPatternList->GetEnd ();

			for (PatternList::STLPatternList_cit it = pOrthogonalPatternList->GetBegin (); it != itEnd; ++it)
			{
				if (static_cast<const Pattern *>(*it)->GetSimilarity (pPattern, rMetric) > rAlpha)
				{
					bOrtho = false;
					break;
				}
			}

			if (bOrtho)
				pOrthogonalPatternList->PushBack (pPattern);
			else
				tries++;
		}
		else
			tries++;
	}

	delete[] triedPatternArray;

	size = pOrthogonalPatternList->GetSize ();

//	LOGMSG (NO_DEBUG, "PatternList::GetOrthogonalPatternListORIGAMIRandomCandidate () - orthogonal patterns [%u]\n", size);

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

Pattern* PatternList::GetMoreSimilar (const Pattern *pPattern, const Pattern::OrtMetric &rMetric) const
{
	STLPatternList_cit it		= GetBegin ();
	STLPatternList_cit itEnd	= GetEnd ();

	Pattern *pRetPattern = static_cast<Pattern *>(*(it++));

	float32 similarity = pRetPattern->GetSimilarity (pPattern, rMetric);

	while (it != itEnd)
	{
		Pattern *pTryPattern = static_cast<Pattern *>(*(it++));

		if (pTryPattern->GetSimilarity (pPattern, rMetric) > similarity)
			pRetPattern = pTryPattern;
	}

	return pRetPattern;
}

const float32 PatternList::GetSetSimilarityRate () const
{
	float32 rate		= 0		;
	uint32	num_patterns	= GetSize ()	;

	if (num_patterns > 1)
	{
		const uint32 *patternIDArray = GetPatternIDArray ();

		uint32	num_cov_items		= 0	;
		float32 exclusive_factor	= 0.0	;
		uint32	num_items		= Item::GetNumTrainItems ();

		for (uint32 itemID = 0; itemID < num_items; itemID++)
		{
			uint32 patterns_found_with_item = 0;

			for (uint32 i = 0; i < num_patterns; i++)
				if (mItemPatternCoverageMatrix [itemID][patternIDArray [i]])
					patterns_found_with_item++;

			if (patterns_found_with_item)
			{
				num_cov_items++;
				exclusive_factor += float32 (num_patterns - patterns_found_with_item) / (num_patterns - 1);
			}
		}

		delete[] patternIDArray;

		rate = exclusive_factor / num_cov_items;
	}

	return rate;
}

const float32 PatternList::GetSetCoverageRate (const TransactionList *pTransactionList) const
{
	float32 rate		= 0		;
	uint32	num_patterns	= GetSize ()	;

	if (num_patterns > 1)
	{
		const uint32 *patternIDArray = GetPatternIDArray ();

		uint32	num_cov_transactions	= 0		;
		float32 exclusive_factor	= 0.0		;
		uint32	num_transactions	= Transaction::GetNumTrainTransactions ();

		for (uint32 transactionID = 0; transactionID < num_transactions; transactionID++)
		{
			uint32 patterns_found_in_transaction = 0;

			for (uint32 i = 0; i < num_patterns; i++)
				if (mTransactionPatternCoverageMatrix [transactionID][patternIDArray [i]])
					patterns_found_in_transaction++;

			if (patterns_found_in_transaction)
			{
				num_cov_transactions++;
				exclusive_factor += float32 (num_patterns - patterns_found_in_transaction) / (num_patterns - 1);
			}
		}

		delete[] patternIDArray;

		rate = exclusive_factor / num_cov_transactions;
	}

	return rate;
}

const float32 PatternList::GetSetClassCoverageRate (const TransactionList *pTransactionList) const
{
	float32 rate		= 0		;
	uint32	num_patterns	= GetSize ()	;

	if (num_patterns > 1)
	{
		uint32 num_total_classes = Class::GetNumTrainClasses ();

		float32* classCoverageAverageArray = new float32 [num_total_classes];

		for (uint32 classID = 0; classID < num_total_classes; classID++)
			classCoverageAverageArray [classID] = 0;

		const uint32 *patternIDArray = GetPatternIDArray ();

		for (uint32 classID = 0; classID < num_total_classes; classID++)
		{
			for (uint32 i = 0; i < num_patterns; i++)
				classCoverageAverageArray [classID] += mClassPatternCoverageMatrix [classID][patternIDArray [i]];

			classCoverageAverageArray [classID] /= num_patterns;
		}

		uint32	num_classes		= 0	;
		float32 exclusive_factor	= 0.0	;

		for (uint32 classID = 0; classID < num_total_classes; classID++)
		{
			uint32 patterns_found_with_class = 0;

			for (uint32 i = 0; i < num_patterns; i++)
				if (mClassPatternCoverageMatrix [classID][patternIDArray [i]] >= 0.9 * classCoverageAverageArray [classID])
					patterns_found_with_class++;

			if (patterns_found_with_class)
			{
				num_classes++;
				exclusive_factor += float32 (num_patterns - patterns_found_with_class) / (num_patterns - 1);
			}
		}

		delete[] patternIDArray;
		delete[] classCoverageAverageArray;

		rate = exclusive_factor / num_classes;
	}

	return rate;
}

const float32 PatternList::GetPairAverageSimilarityRate () const
{
	float32	rate		= 0		;
	uint32	num_patterns	= GetSize ()	;

	if (num_patterns > 1)
	{
		const uint32 *patternIDArray = GetPatternIDArray ();

		for (uint32 leftPatternIndex = 0; leftPatternIndex < num_patterns - 1; leftPatternIndex++)
		{
			for (uint32 rightPatternIndex = leftPatternIndex + 1; rightPatternIndex < num_patterns; rightPatternIndex++)
			{
				uint32 num = 0;
				uint32 den = 0;
				uint32 num_items = Item::GetNumTrainItems ();

				for (uint32 itemID = 0; itemID < num_items; itemID++)
				{
					bool	leftCoverage	= mItemPatternCoverageMatrix [itemID][patternIDArray [leftPatternIndex]];
					bool	rightCoverage	= mItemPatternCoverageMatrix [itemID][patternIDArray [rightPatternIndex]];

					if (leftCoverage || rightCoverage)
					{
						den++;

						if (! leftCoverage || ! rightCoverage)
							num++;
					}
				}

				rate += (float32) num / den;
			}
		}

		delete[] patternIDArray;

		rate /= (num_patterns * (num_patterns - 1) / 2);
	}

	return rate;
}

const float32 PatternList::GetPairAverageCoverageRate (const TransactionList *pTransactionList) const
{
	float32	rate		= 0		;
	uint32	num_patterns	= GetSize ()	;

	if (num_patterns > 1)
	{
		const uint32 *patternIDArray = GetPatternIDArray ();

		for (uint32 leftPatternIndex = 0; leftPatternIndex < num_patterns - 1; leftPatternIndex++)
		{
			for (uint32 rightPatternIndex = leftPatternIndex + 1; rightPatternIndex < num_patterns; rightPatternIndex++)
			{
				uint32 num = 0;
				uint32 den = 0;
				uint32 num_transactions = Transaction::GetNumTrainTransactions ();

				for (uint32 transactionID = 0; transactionID < num_transactions; transactionID++)
				{
					bool	leftCoverage	= mTransactionPatternCoverageMatrix [transactionID][patternIDArray [leftPatternIndex]];
					bool	rightCoverage	= mTransactionPatternCoverageMatrix [transactionID][patternIDArray [rightPatternIndex]];

					if (leftCoverage || rightCoverage)
					{
						den++;

						if (! leftCoverage || ! rightCoverage)
							num++;
					}
				}

				rate += (float32) num / den;
			}
		}

		delete[] patternIDArray;

		rate /= (num_patterns * (num_patterns - 1) / 2);
	}

	return rate;
}

const float32 PatternList::GetPairAverageClassCoverageRate (const TransactionList *pTransactionList) const
{
		float32	rate		= 0				;
	const	uint32	num_patterns	= GetSize ()			;
	const	uint32	num_classes	= Class::GetNumTrainClasses ()	;

	if (num_patterns > 1)
	{
		const uint32 *patternIDArray = GetPatternIDArray ();

		for (uint32 leftPatternIndex = 0; leftPatternIndex < num_patterns - 1; leftPatternIndex++)
		{
			for (uint32 rightPatternIndex = leftPatternIndex + 1; rightPatternIndex < num_patterns; rightPatternIndex++)
			{
				uint32	num_cov_classes	= 0;
				float32	pattern_rate	= 0;

				for (uint32 classID = 0; classID < num_classes; classID++)
				{
					uint32	leftCoverage	= mClassPatternCoverageMatrix [classID][patternIDArray [leftPatternIndex]];
					uint32	rightCoverage	= mClassPatternCoverageMatrix [classID][patternIDArray [rightPatternIndex]];

					if (leftCoverage || rightCoverage)
					{
						num_cov_classes++;

						if (leftCoverage > rightCoverage)
							pattern_rate += (float32) (leftCoverage - rightCoverage) / leftCoverage;
						else if (rightCoverage > leftCoverage)
							pattern_rate += (float32) (rightCoverage - leftCoverage) / rightCoverage;
					}
				}

				pattern_rate /= num_cov_classes;

				rate += pattern_rate;
			}
		}

		delete[] patternIDArray;

		rate /= (num_patterns * (num_patterns - 1) / 2);
	}

	return rate;
}

const float32 PatternList::GetRate (const TransactionList *pTransactionList, const OrtMethod &method, const Pattern::OrtMetric &rMetric) const
{
	float32 rate = -1;

	switch (method)
	{
		case METHOD_SET:
			rate = GetSetRate (pTransactionList, rMetric);
			break;
		case METHOD_PAIR_AVERAGE:
			rate = GetPairAverageRate (pTransactionList, rMetric);
			break;
		case METHOD_ALL:
			rate = GetSetRate (pTransactionList, rMetric) * GetPairAverageRate (pTransactionList, rMetric);
			break;
		case METHOD_UNKNOWN:
		default:
			LOGMSG (NO_DEBUG, "PatternList::GetRate () - unknown orthogonality method\n");
			throw DataBaseException ("Unknown orthogonality method");
			break;
	}

	return rate;
}

const float32 PatternList::GetSetRate (const TransactionList *pTransactionList, const Pattern::OrtMetric &rMetric) const
{
	float32 rate = -1;

	switch (rMetric)
	{
		case Pattern::METRIC_SIMILARITY:
			rate = GetSetSimilarityRate ();
			break;
		case Pattern::METRIC_TRANS_COVERAGE:
			rate = GetSetCoverageRate (pTransactionList);
			break;
		case Pattern::METRIC_CLASS_COVERAGE:
			rate = GetSetClassCoverageRate (pTransactionList);
			break;
		case Pattern::METRIC_ALL:
			rate = GetSetSimilarityRate () * GetSetCoverageRate (pTransactionList) * GetSetClassCoverageRate (pTransactionList);
			break;
		case Pattern::METRIC_UNKNOWN:
		default:
			LOGMSG (NO_DEBUG, "PatternList::GetSetRate () - unknown orthogonality metric\n");
			throw DataBaseException ("Unknown orthogonality metric");
			break;
	}

	return rate;
}

const float32 PatternList::GetPairAverageRate (const TransactionList *pTransactionList, const Pattern::OrtMetric &rMetric) const
{
	float32 rate = -1;

	switch (rMetric)
	{
		case Pattern::METRIC_SIMILARITY:
			rate = GetPairAverageSimilarityRate ();
			break;
		case Pattern::METRIC_TRANS_COVERAGE:
			rate = GetPairAverageCoverageRate (pTransactionList);
			break;
		case Pattern::METRIC_CLASS_COVERAGE:
			rate = GetPairAverageClassCoverageRate (pTransactionList);
			break;
		case Pattern::METRIC_ALL:
			rate = GetPairAverageSimilarityRate () * GetPairAverageCoverageRate (pTransactionList) * GetPairAverageClassCoverageRate (pTransactionList);
			break;
		case Pattern::METRIC_UNKNOWN:
		default:
			LOGMSG (NO_DEBUG, "PatternList::GetPairAverageRate () - unknown orthogonality metric\n");
			throw DataBaseException ("Unknown orthogonality metric");
			break;

	}

	return rate;
}

void PatternList::MakeItemPatternCoverageMatrix (const PatternList *pPatternList)
{
	uint32	num_items 	= Item::GetNumTrainItems ();
	uint32	num_patterns	= Pattern::GetNumPatterns ();

	LOGMSG (LOW_LEVEL, "PatternList::MakeItemPatternCoverageMatrix () - num_items [%u], num_patterns [%u]\n", num_items, num_patterns);

	mItemPatternCoverageMatrix = new bool* [num_items];

	for (uint32 i = 0; i < num_items; i++)
	{
		mItemPatternCoverageMatrix [i] = new bool [num_patterns];

		for (uint32 j = 0; j < num_patterns; j++)
			mItemPatternCoverageMatrix [i][j] = false;
	}

	PatternList::STLPatternList_cit itPatternEnd = pPatternList->GetEnd ();

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
	uint32	num_transactions 	= Transaction::GetNumTrainTransactions ();
	uint32	num_patterns		= Pattern::GetNumPatterns ();

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
		const Transaction *pTransaction = static_cast<const Transaction *>(*itTransaction);

		for (PatternList::STLPatternList_cit itPattern = pPatternList->GetBegin (); itPattern != itPatternEnd; itPattern++)
		{
			const Pattern *pPattern = static_cast<const Pattern*>(*itPattern);

			if (pTransaction->IsCoveredBy (pPattern))
				mTransactionPatternCoverageMatrix [pTransaction->GetTransactionID ()][pPattern->GetPatternID ()] = true;
		}
	}

	LOGMSG (LOW_LEVEL, "PatternList::MakeTransactionPatternCoverageMatrix () - end\n");
}

void PatternList::MakeClassPatternCoverageMatrix (const TransactionList *pTransactionList, const PatternList *pPatternList)
{
	uint32	num_classes 	= Class::GetNumTrainClasses ();
	uint32	num_patterns	= Pattern::GetNumPatterns ();

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
		const Transaction *pTransaction = static_cast<const Transaction *>(*itTransaction);

		for (PatternList::STLPatternList_cit itPattern = pPatternList->GetBegin (); itPattern != itPatternEnd; itPattern++)
		{
			const Pattern *pPattern = static_cast<const Pattern*>(*itPattern);

			if (pTransaction->IsCoveredBy (pPattern))
				mClassPatternCoverageMatrix [pTransaction->GetClassID ()][pPattern->GetPatternID ()]++;
		}
	}

	LOGMSG (LOW_LEVEL, "PatternList::MakeClassPatternCoverageMatrix () - end\n");
}

const uint32* PatternList::GetPatternIDArray () const
{
	uint32	num_patterns	= GetSize ()			;
	uint32* patternIDArray	= new uint32 [num_patterns]	;
	uint32	i		= 0				;

	STLPatternList_cit itPatternEnd = GetEnd ();

	for (STLPatternList_cit itPattern = GetBegin (); itPattern != itPatternEnd; ++itPattern)
		patternIDArray [i++] = static_cast<const Pattern *>(*itPattern)->GetPatternID ();

	return patternIDArray;
}

void PatternList::DestroyItemPatternCoverageMatrix ()
{
	uint32 num_items = Item::GetNumTrainItems ();

	for (uint32 i = 0; i < num_items; i++)
		delete[] mItemPatternCoverageMatrix [i];

	delete[] mItemPatternCoverageMatrix;
}

void PatternList::DestroyTransactionPatternCoverageMatrix ()
{
	uint32 num_transactions = Transaction::GetNumTrainTransactions ();

	for (uint32 i = 0; i < num_transactions; i++)
		delete[] mTransactionPatternCoverageMatrix [i];

	delete[] mTransactionPatternCoverageMatrix;
}

void PatternList::DestroyClassPatternCoverageMatrix ()
{
	uint32 num_classes = Class::GetNumTrainClasses ();

	for (uint32 i = 0; i < num_classes; i++)
		delete[] mClassPatternCoverageMatrix [i];

	delete[] mClassPatternCoverageMatrix;
}

const bool PatternList::FindSuperPatternOf (const Pattern *pPattern) const
{
	bool bRet = false;

	STLPatternList_cit itEnd = GetEnd ();

	for (STLPatternList_cit it = GetBegin (); it != itEnd; ++it)
	{
		if (static_cast<const Pattern *>(*it)->IsSuperPatternOf (pPattern))
		{
			bRet = true;
			break;
		}
	}

	return bRet;
}

void PatternList::RemoveSubPatternsOf (const Pattern *pPattern)
{
//	LOGMSG (NO_DEBUG, "PatternList::RemoveSubPatternsOf () - begin\n");

	STLPatternList_it it	= GetBegin ()	;
	STLPatternList_it itEnd	= GetEnd ()	;

	while (it != itEnd)
	{
		if (pPattern->IsSuperPatternOf (static_cast<const Pattern *>(*it)))
		{
			delete *it;
			it = Erase (it);
			itEnd = GetEnd ();
		}
		else
			++it;
	}

//	LOGMSG (NO_DEBUG, "PatternList::RemoveSubPatternsOf () - end\n");
}
