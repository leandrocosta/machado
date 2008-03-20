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
	hash_map<string, float32>	gainHash;
	hash_map<string, float32>	jaccardHash;
	hash_map<string, float32>	kulcHash;
	hash_map<string, float32>	cosineHash;
	hash_map<string, float32>	coherenceHash;
	hash_map<string, float32>	sensitivityHash;
	hash_map<string, float32>	specificityHash;
	hash_map<string, float32>	laplaceHash;
	hash_map<string, float32>	correlationHash;
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
		gainHash[pRule->GetClassValue ()] += pRule->GetGain ();
		jaccardHash[pRule->GetClassValue ()] += pRule->GetJaccard ();
		kulcHash[pRule->GetClassValue ()] += pRule->GetKulc ();
		cosineHash[pRule->GetClassValue ()] += pRule->GetCosine ();
		coherenceHash[pRule->GetClassValue ()] += pRule->GetCoherence ();
		sensitivityHash[pRule->GetClassValue ()] += pRule->GetSensitivity ();
		specificityHash[pRule->GetClassValue ()] += pRule->GetSpecificity ();
		laplaceHash[pRule->GetClassValue ()] += pRule->GetLaplace ();
		correlationHash[pRule->GetClassValue ()] += pRule->GetCorrelation ();

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
		float32	gain		= gainHash [it->first]	;
		float32	jaccard		= jaccardHash [it->first]	;
		float32	kulc		= kulcHash [it->first]	;
		float32	cosine		= cosineHash [it->first]	;
		float32	coherence	= coherenceHash [it->first]	;
		float32	sensitivity	= sensitivityHash [it->first]	;
		float32	specificity	= specificityHash [it->first]	;
		float32	laplace		= laplaceHash [it->first]	;
		float32	correlation	= correlationHash [it->first]	;

		float32 support_mean		= support	/ rules	;
		float32 confidence_mean		= confidence	/ rules	;
		float32	gain_mean		= gain		/ rules	;
		float32	jaccard_mean		= jaccard	/ rules	;
		float32	kulc_mean		= kulc		/ rules	;
		float32	cosine_mean		= cosine	/ rules	;
		float32	coherence_mean		= coherence	/ rules	;
		float32	sensitivity_mean	= sensitivity	/ rules	;
		float32	specificity_mean	= specificity	/ rules	;
		float32	laplace_mean		= laplace	/ rules	;
		float32	correlation_mean	= correlation	/ rules	;


		LOGMSG (MEDIUM_LEVEL, "RankingRuleList::GetClassificationValue () - class [%s], rules [%u], support abs/mean [%f/%f], confidence abs/mean [%f/%f], gain abs/mean [%f/%f], jaccard abs/mean [%f/%f], kulc abs/mean [%f/%f], cosine abs/mean [%f/%f], coherence abs/mean [%f/%f], sensitivity abs/mean [%f/%f], specificity abs/mean [%f/%f], laplace abs/mean [%f/%f], correlation abs/mean [%f/%f]\n", it->first.c_str (), rules, support, support_mean, confidence, confidence_mean, gain, gain_mean, jaccard, jaccard_mean, kulc, kulc_mean, cosine, cosine_mean, coherence, coherence_mean, sensitivity, sensitivity_mean, specificity, specificity_mean, laplace, laplace_mean, correlation, correlation_mean);

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
