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

TransactionList::STLTransactionList_it TransactionList::GetBegin ()
{
	return ObjectList::GetBegin ();
}

const TransactionList::STLTransactionList_cit TransactionList::GetBegin () const
{
	return ObjectList::GetBegin ();
}

TransactionList::STLTransactionList_it TransactionList::GetEnd ()
{
	return ObjectList::GetEnd ();
}

const TransactionList::STLTransactionList_cit TransactionList::GetEnd () const
{
	return ObjectList::GetEnd ();
}

/*
void TransactionList::Print () const
{
	STLTransactionList_cit it;

	for (it = GetBegin (); it != GetEnd (); it++)
	{
		it->Print ();
	}
}
*/
