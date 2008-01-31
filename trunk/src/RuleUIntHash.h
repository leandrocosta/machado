#ifndef _RULE_UINT_HASH_H_
#define _RULE_UINT_HASH_H_


#include "base/ObjectUIntHash.h"


class RuleUIntHash : public ObjectUIntHash
{
	public:
			RuleUIntHash	(const uint32 &max_size = UINT_MAX)	;
		virtual	~RuleUIntHash	()					;

};


#endif
