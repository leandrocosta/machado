#include "Transaction.h"
#include "Class.h"
#include "ItemList.h"
#include "base/Logger.h"

#include <iostream>
using std::cout;
using std::endl;


uint32 Transaction::msSeqTransactionID	= 0	;


Transaction::Transaction (const Class *pClass) : ItemSet (), mTransactionID (GetSeqTransactionID ()), mpClass (pClass)
{

}

Transaction::~Transaction ()
{
	LOGMSG (MEDIUM_LEVEL, "Transaction::~Transaction () - p [%p]\n", this);

	RemoveAll ();
}

const uint32 Transaction::GetSeqTransactionID ()
{
	uint32 transactionID = msSeqTransactionID;
	msSeqTransactionID++;

	return transactionID;
}

const Class* Transaction::GetClass () const
{
	return mpClass;
}

const uint32& Transaction::GetTransactionID () const
{
	return mTransactionID;
}

void Transaction::Print () const
{
	LOGMSG (LOW_LEVEL, "Transaction::Print () - mTransactionID [%u], class [%s], items [%s]\n",  mTransactionID, mpClass->GetValue ().c_str (), GetPrintableString ().c_str ());
//	cout << "[" << mpClass->GetValue () << "] [" << GetPrintableString () << "]" << endl;
}
