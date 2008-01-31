#include "RuleUIntHash.h"
#include "base/Logger.h"


RuleUIntHash::RuleUIntHash (const uint32 &max_size)
{
	LOGMSG (MEDIUM_LEVEL, "RuleUIntHash::RuleUIntHash () - [%p]\n", this);
}

RuleUIntHash::~RuleUIntHash ()
{
	LOGMSG (MEDIUM_LEVEL, "RuleUIntHash::~RuleUIntHash () - [%p]\n", this);
}
