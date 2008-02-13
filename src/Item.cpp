#include "Item.h"
#include "TransactionList.h"
#include "base/Logger.h"


uint32 Item::msSeqItemID	= 0	;


Item::Item (const string &value, const uint32 &count) : Object (), mItemID (GetSeqItemID ()), mValue (value), mCount (count)
{
	mpTransactionList		= new TransactionList ();
	mpProjectionTransactionList	= new TransactionList ();
}

Item::~Item ()
{
	mpTransactionList->RemoveAll ();
	mpProjectionTransactionList->RemoveAll ();

	delete mpTransactionList;
	delete mpProjectionTransactionList;
}

const bool Item::operator< (const Object &rObject) const
{
	return GetItemID () < static_cast<const Item &>(rObject).GetItemID ();
}

const bool Item::operator> (const Object &rObject) const
{
	return GetItemID () > static_cast<const Item &>(rObject).GetItemID ();
}

const bool Item::operator<= (const Object &rObject) const
{
	return GetItemID () <= static_cast<const Item &>(rObject).GetItemID ();
}

const bool Item::operator>= (const Object &rObject) const
{
	return GetItemID () >= static_cast<const Item &>(rObject).GetItemID ();
}

const bool Item::operator== (const Object &rObject) const
{
	return GetItemID () == static_cast<const Item &>(rObject).GetItemID ();
}

const bool Item::operator!= (const Object &rObject) const
{
	return GetItemID () != static_cast<const Item &>(rObject).GetItemID ();
}

const uint32 Item::GetSeqItemID ()
{
	uint32 itemID = msSeqItemID++;

	return itemID;
}

const uint32 Item::GetMaxItemID ()
{
	return msSeqItemID - 1;
}

const uint32& Item::GetItemID () const
{
	return mItemID;
}

const string& Item::GetValue () const
{
	return mValue;
}

void Item::SetCount (const uint32 &count)
{
	mCount = count;
}

void Item::IncCount ()
{
//	LOGMSG (HIGH_LEVEL, "Item::IncCount () - p [%p]\n", this);

	mCount++;
}

const uint32& Item::GetCount () const
{
	return mCount;
}

void Item::AddTransaction (Transaction *pTransaction)
{
	mpTransactionList->PushBack (pTransaction);
}

void Item::AddProjectionTransaction (Transaction *pTransaction)
{
	mpProjectionTransactionList->PushBack (pTransaction);
}

const TransactionList* Item::GetProjectionTransactionList () const
{
	return mpProjectionTransactionList;
}

const uint32 Item::GetProjectionFrequence () const
{
	return mpProjectionTransactionList->GetSize ();
}

void Item::ClearProjectionTransactionList ()
{
	mpProjectionTransactionList->RemoveAll ();
}

void Item::Print () const
{
	LOGMSG (NO_DEBUG, "Item::Print () - mItemID [%u], mValue [%s]\n", mItemID, mValue.c_str ());
}
