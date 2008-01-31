#ifndef _TRANSACTION_LIST_H_
#define _TRANSACTION_LIST_H_


#include "base/ObjectList.h"
#include "Transaction.h"

#include <string>
using std::string;


class TransactionList : public ObjectList
{
	public:
			TransactionList		(const uint64 &max_size = ULONG_MAX)	;
		virtual	~TransactionList	()					;

	public:
		typedef STLObjectList				STLTransactionList	;
		typedef STLTransactionList::iterator		STLTransactionList_it	;
		typedef STLTransactionList::const_iterator	STLTransactionList_cit	;

	public:
			STLTransactionList_it	GetBegin	()		;
		const	STLTransactionList_cit	GetBegin	()	const	;
			STLTransactionList_it	GetEnd		()		;
		const	STLTransactionList_cit	GetEnd		()	const	;
};


#endif
