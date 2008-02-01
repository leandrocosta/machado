#include "TransactionList.h"
#include "base/Logger.h"


TransactionList::TransactionList (const uint64 &max_size) : ObjectList (max_size)
{
	LOGMSG (HIGH_LEVEL, "TransactionList::TransactionList () - p [%p]\n", this);
}

TransactionList::~TransactionList ()
{
	LOGMSG (MEDIUM_LEVEL, "TransactionList::~TransactionList () - p [%p]\n", this);
}
