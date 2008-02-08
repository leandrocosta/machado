#ifndef _RANKING_RULE_LIST_H_
#define _RANKING_RULE_LIST_H_


#include "base/ObjectList.h"

#include <string>
using std::string;


class RankingRuleList : public ObjectList
{
	public:
			RankingRuleList		(const uint64 &max_size = UINT_MAX)	;
		virtual	~RankingRuleList	()					;

	public:
		typedef STLObjectList				STLRankingRuleList		;
		typedef STLRankingRuleList::iterator		STLRankingRuleList_it		;
		typedef STLRankingRuleList::const_iterator	STLRankingRuleList_cit		;

	public:
		const	string	GetClassificationValue	(const uint32 &rMaxNumRankRules)	;
};


#endif
