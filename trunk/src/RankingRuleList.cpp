#include "RankingRuleList.h"
#include "RankingRule.h"
#include "base/Logger.h"
#include "base/stl_hash_include.h"

#include <iostream>
using std::cout;
using std::endl;


RankingRuleList::RankingRuleList (const uint64 &max_size) : ObjectList (max_size)
{
	LOGMSG (MAX_LEVEL, "RankingRuleList::RankingRuleList () - p [%p]\n", this);
}

RankingRuleList::~RankingRuleList ()
{
	LOGMSG (MAX_LEVEL, "RankingRuleList::~RankingRuleList () - p [%p]\n", this);
}

const string RankingRuleList::GetClassificationValue (const uint32 &rMaxNumRankRules) const
{
	LOGMSG (LOW_LEVEL, "RankingRuleList::GetClassificationValue () - rMaxNumRankRules [%u]\n", rMaxNumRankRules);

	hash_map<string, uint32>	rulesHash;
	hash_map<string, float32>	supportHash;
	hash_map<string, float32>	confidenceHash;
	hash_map<string, float32>	rankHash;

//	uint32 total_rules = GetSize ();

	RankingRuleList *pReverseSortedRuleList = static_cast<const RankingRuleList *>(GetPartialReverseSortCopy (rMaxNumRankRules));

	uint32 rules = 0;

	STLRankingRuleList_cit itEnd = pReverseSortedRuleList->GetEnd ();

	for (STLRankingRuleList_cit it = pReverseSortedRuleList->GetBegin (); it != itEnd; ++it)
	{
		const RankingRule *pRule = static_cast<const RankingRule *>(*it);

		pRule->Print ();

		rulesHash[pRule->GetClassValue ()]++;
		supportHash[pRule->GetClassValue ()] += pRule->GetSupport ();
		confidenceHash[pRule->GetClassValue ()] += pRule->GetConfidence ();

		/*
		rankHash[pRule->GetClassValue ()] +=
			pRule->GetSupport () +
			pRule->GetConfidence () +
			pRule->GetGain () +
			pRule->GetJaccard () +
			pRule->GetKulc () +
			pRule->GetCosine () +
			pRule->GetCoherence () +
			pRule->GetSensitivity () +
			pRule->GetSpecificity () +
			pRule->GetLaplace () +
			pRule->GetCorrelation ();
		*/

		rankHash[pRule->GetClassValue ()] += pRule->GetConfidence ();

		rules++;

		if (rules == rMaxNumRankRules)
			break;
	}

	pReverseSortedRuleList->RemoveAll ();
	delete pReverseSortedRuleList;

	string	class_guess	= "";
	float32	rank		= -1;
	float32	rank_try	= -1;

	for (hash_map<string, uint32>::iterator it = rulesHash.begin (); it != rulesHash.end (); ++it)
	{
		uint32	rules		= it->second			;
		float32 support		= supportHash [it->first]	;
		float32	confidence	= confidenceHash [it->first]	;
//		float32 support_mean	= support / total_rules		;
		float32 support_mean	= support / rules		;
//		float32 confidence_mean	= confidence / total_rules	;
		float32 confidence_mean	= confidence / rules		;

		LOGMSG (MEDIUM_LEVEL, "RankingRuleList::GetClassificationValue () - class [%s], rules [%u], support abs/mean [%f/%f], confidence abs/mean [%f/%f]\n", it->first.c_str (), rules, support, support_mean, confidence, confidence_mean);

//		rank_try = support_mean + confidence_mean;
		rank_try = rankHash [it->first] / rules;

		if (rank_try > rank)
		{
			class_guess	= it->first;
			rank		= rank_try;
		}
	}

	LOGMSG (LOW_LEVEL, "RankingRuleList::GetClassificationValue () - class [%s]\n", class_guess.c_str ());

	return class_guess;
}
