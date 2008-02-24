#include "Item.h"
#include "TransactionList.h"
#include "base/Logger.h"


uint32 Item::msSeqItemID	= 0	;


Item::Item (const string &value) : Object (), mItemID (GetSeqItemID ()), mValue (value)
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

const bool Item::operator< (const Object &rRight) const
{
	return mItemID < static_cast<const Item &>(rRight).GetItemID ();
}

const bool Item::operator> (const Object &rRight) const
{
	return mItemID > static_cast<const Item &>(rRight).GetItemID ();
}

const bool Item::operator<= (const Object &rRight) const
{
	return mItemID <= static_cast<const Item &>(rRight).GetItemID ();
}

const bool Item::operator>= (const Object &rRight) const
{
	return mItemID >= static_cast<const Item &>(rRight).GetItemID ();
}

const bool Item::operator== (const Object &rRight) const
{
	return GetItemID () == static_cast<const Item &>(rRight).GetItemID ();
}

const bool Item::operator!= (const Object &rRight) const
{
	return GetItemID () != static_cast<const Item &>(rRight).GetItemID ();
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

const uint32 Item::GetNumItems ()
{
	return msSeqItemID;
}

const uint32& Item::GetItemID () const
{
	return mItemID;
}

const string& Item::GetValue () const
{
	return mValue;
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
