#ifndef _RULE_LIST_H_
#define _RULE_LIST_H_


#include "base/ObjectList.h"
#include "Rule.h"

#include <string>
using std::string;


class RuleList : public ObjectList
{
	public:
			RuleList	(const uint64 &max_size = ULONG_MAX)	;
		virtual	~RuleList	()					;

	public:
		typedef STLObjectList			STLRuleList	;
		typedef STLRuleList::iterator		STLRuleList_it	;
		typedef STLRuleList::const_iterator	STLRuleList_cit	;

	public:
		const	string	GetClassificationValue	(const uint32 &rMaxNumRankRules)	;
};


#endif
