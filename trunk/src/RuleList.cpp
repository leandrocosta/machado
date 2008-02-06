#include "RuleList.h"
#include "base/Logger.h"
#include "base/stl_hash_include.h"

#include <iostream>
using std::cout;
using std::endl;


RuleList::RuleList (const uint64 &max_size) : ObjectList (max_size)
{
	LOGMSG (MEDIUM_LEVEL, "RuleList::RuleList () - p [%p]\n", this);
}

RuleList::~RuleList ()
{
	LOGMSG (MEDIUM_LEVEL, "RuleList::~RuleList () - p [%p]\n", this);
}

const string RuleList::GetClassificationValue () const
{
	hash_map<string, uint32>	rulesHash;
	hash_map<string, float32>	supportHash;
	hash_map<string, float32>	confidenceHash;
	hash_map<string, float32>	rankHash;

//	uint32 total_rules = GetSize ();

	STLRuleList_cit itEnd = GetEnd ();

	for (STLObjectList_cit it = GetBegin (); it != itEnd; it++)
	{
		const Rule *pRule = static_cast<const Rule *>(*it);

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
	}

	string	class_guess	= "";
	float32	rank		= -1;
	float32	rank_try	= -1;

	for (hash_map<string, uint32>::iterator it = rulesHash.begin (); it != rulesHash.end (); it++)
	{
		uint32	rules		= it->second			;
		float32 support		= supportHash [it->first]	;
		float32	confidence	= confidenceHash [it->first]	;
//		float32 support_mean	= support / total_rules		;
		float32 support_mean	= support / rules		;
//		float32 confidence_mean	= confidence / total_rules	;
		float32 confidence_mean	= confidence / rules		;

		LOGMSG (LOW_LEVEL, "RuleList::GetClassificationValue () - class [%s], rules [%u], support abs/mean [%f/%f], confidence abs/mean [%f/%f]\n", it->first.c_str (), rules, support, support_mean, confidence, confidence_mean);

//		rank_try = support_mean + confidence_mean;
		rank_try = rankHash [it->first] / rules;

		if (rank_try > rank)
		{
			class_guess	= it->first;
			rank		= rank_try;
		}
	}

	return class_guess;
}
