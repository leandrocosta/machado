#include "Class.h"
#include "TransactionList.h"
#include "base/Logger.h"


uint32 Class::msSeqClassID	= 0	;


Class::Class (const string &value) : Object (), mClassID (GetSeqClassID ()), mValue (value)
{
	LOGMSG (MAX_LEVEL, "Class::Class () - value [%s]\n", value.c_str ());

	mpTransactionList = new TransactionList ();
}

Class::~Class ()
{
	mpTransactionList->RemoveAll ();

	delete mpTransactionList;
}

const uint32 Class::GetSeqClassID ()
{
	uint32 itemID = msSeqClassID;
	msSeqClassID++;

	return itemID;
}

const uint32& Class::GetClassID () const
{
	return mClassID;
}

const string& Class::GetValue () const
{
	return mValue;
}

void Class::AddTransaction (Transaction *pTransaction)
{
	mpTransactionList->PushBack (pTransaction);
}

void Class::Print () const
{
	LOGMSG (NO_DEBUG, "Class::Print () - mClassID [%u], mValue [%s]\n", mClassID, mValue.c_str ());
}
