#include "TransactionList.h"
#include "Class.h"
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

TransactionList* TransactionList::GetProjection (const Transaction *pBaseTransaction)
{
	TransactionList *pTransactionList = new TransactionList ();

	STLTransactionList_cit itEnd = GetEnd ();

	for (STLTransactionList_cit it = GetBegin (); it != itEnd; it++)
	{
		if (Transaction::HasIntersectionByPtr (pBaseTransaction, static_cast<const Transaction *>(*it)))
		{
			Transaction *pTransaction = static_cast<Transaction *>(*it);

			pTransaction->GetClass ()->AddTransaction (pTransaction);

			pTransactionList->PushBack (pTransaction);
		}
	}

	return pTransactionList;
}
