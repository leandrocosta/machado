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

void RuleList::PrintClassification () const
{
	hash_map<string, uint32>	classHash;
	hash_map<string, float32>	supportHash;
	hash_map<string, float32>	confidenceHash;

	uint32 total_rules = GetSize ();

	STLRuleList_cit itEnd = GetEnd ();

	for (STLObjectList_cit it = GetBegin (); it != itEnd; it++)
	{
		const Rule *pRule = static_cast<const Rule *>(*it);

//		pRule->Print ();

		classHash[pRule->GetClassValue ()]++;
		supportHash[pRule->GetClassValue ()] += pRule->GetSupport ();
		confidenceHash[pRule->GetClassValue ()] += pRule->GetConfidence ();
	}

	string	class_guess	= "";
	float32	rank		= -1;
	float32	rank_try	= -1;

	for (hash_map<string, uint32>::iterator it = classHash.begin (); it != classHash.end (); it++)
	{
		uint32	rules		= it->second			;
		float32 support		= supportHash [it->first]	;
		float32	confidence	= confidenceHash [it->first]	;
		float32 support_mean	= support / total_rules		;
		float32 confidence_mean	= confidence / total_rules	;

		LOGMSG (LOW_LEVEL, "RuleList::PrintClassification () - class [%s], rules [%u], support abs/mean [%f/%f], confidence abs/mean [%f/%f]\n", it->first.c_str (), rules, support, support_mean, confidence, confidence_mean);
//		cout << "classe [" << it->first << "], regras [" << rules << "], suporte absoluto/média [" << support << "/" << support_mean << "], confiança absoluto/média [" << confidence << "/" << confidence_mean << "]" << endl;

		rank_try = support_mean + confidence_mean;

		if (rank_try > rank)
		{
			rank		= rank_try;
			class_guess	= it->first;
		}
	}

//	cout << "classificação: " << class_guess << endl;
	cout << class_guess << endl;
}

const string RuleList::GetClassificationValue () const
{
	hash_map<string, uint32>	classHash;
	hash_map<string, float32>	supportHash;
	hash_map<string, float32>	confidenceHash;

	uint32 total_rules = GetSize ();

	STLRuleList_cit itEnd = GetEnd ();

	for (STLObjectList_cit it = GetBegin (); it != itEnd; it++)
	{
		const Rule *pRule = static_cast<const Rule *>(*it);

		classHash[pRule->GetClassValue ()]++;
		supportHash[pRule->GetClassValue ()] += pRule->GetSupport ();
		confidenceHash[pRule->GetClassValue ()] += pRule->GetConfidence ();
	}

	string	class_guess	= "";
	float32	rank		= -1;
	float32	rank_try	= -1;

	for (hash_map<string, uint32>::iterator it = classHash.begin (); it != classHash.end (); it++)
	{
		uint32	rules		= it->second			;
		float32 support		= supportHash [it->first]	;
		float32	confidence	= confidenceHash [it->first]	;
		float32 support_mean	= support / total_rules		;
		float32 confidence_mean	= confidence / total_rules	;

		LOGMSG (LOW_LEVEL, "RuleList::GetClassificationValue () - class [%s], rules [%u], support abs/mean [%f/%f], confidence abs/mean [%f/%f]\n", it->first.c_str (), rules, support, support_mean, confidence, confidence_mean);

		rank_try = support_mean + confidence_mean;

		if (rank_try > rank)
		{
			rank		= rank_try;
			class_guess	= it->first;
		}
	}

	return class_guess;
}
