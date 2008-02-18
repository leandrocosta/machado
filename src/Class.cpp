#include "Class.h"
#include "base/Logger.h"


uint32 Class::msSeqClassID	= 0	;


Class::Class (const string &value) : Object (), mClassID (GetSeqClassID ()), mValue (value)
{
//	LOGMSG (MAX_LEVEL, "Class::Class () - value [%s]\n", value.c_str ());
}

Class::~Class ()
{
	mTransactionList.RemoveAll ();
	mProjectionTransactionList.RemoveAll ();
}

const uint32 Class::GetSeqClassID ()
{
	uint32 itemID = msSeqClassID++;

	return itemID;
}


const uint32 Class::GetMaxClassID ()
{
	return msSeqClassID - 1;
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

void Class::AddProjectionTransaction (Transaction *pTransaction)
{
//	LOGMSG (MAX_LEVEL, "Class::AddProjectionTransaction () - class [%s]\n", mValue.c_str ());

	mProjectionTransactionList.PushBack (pTransaction);
}

void Class::ClearProjectionTransactionList ()
{
	mProjectionTransactionList.RemoveAll ();
}

const uint32 Class::GetProjectionTransactionListSize () const
{
	return mProjectionTransactionList.GetSize ();
}

void Class::Print () const
{
	LOGMSG (NO_DEBUG, "Class::Print () - mClassID [%u], mValue [%s]\n", mClassID, mValue.c_str ());
}
