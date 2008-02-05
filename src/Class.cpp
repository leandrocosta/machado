#include "Class.h"
#include "base/Logger.h"


uint32 Class::msSeqClassID	= 0	;


Class::Class (const string &value) : Object (), mClassID (GetSeqClassID ()), mValue (value)
{
	LOGMSG (MAX_LEVEL, "Class::Class () - value [%s]\n", value.c_str ());
}

Class::~Class ()
{
	mTransactionList.RemoveAll ();
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
	mTransactionList.PushBack (pTransaction);
}

void Class::ClearTransactionList ()
{
	mTransactionList.RemoveAll ();
}

const uint64 Class::GetTransactionListSize () const
{
	return mTransactionList.GetSize ();
}

void Class::Print () const
{
	LOGMSG (NO_DEBUG, "Class::Print () - mClassID [%u], mValue [%s]\n", mClassID, mValue.c_str ());
}
