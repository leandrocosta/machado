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

void TransactionList::SortTransactions ()
{
	STLTransactionList_cit itEnd = GetEnd ();

	for (STLTransactionList_cit it = GetBegin (); it != itEnd; it++)
		static_cast<Transaction *>(*it)->Sort ();
}

TransactionList* TransactionList::GetProjection (const Transaction *pTransaction) const
{
	TransactionList *pTransactionList = new TransactionList ();

	STLTransactionList_cit itEnd = GetEnd ();

	for (STLTransactionList_cit it = GetBegin (); it != itEnd; it++)
		if (Transaction::HasIntersectionByPtr (pTransaction, static_cast<const Transaction *>(*it)))
			pTransactionList->PushBack (static_cast<Transaction *>(*it));

	return pTransactionList;
}
