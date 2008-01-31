#include "Item.h"
#include "TransactionList.h"
#include "base/Logger.h"


uint32 Item::msSeqItemID	= 0	;


Item::Item (const string &value, const uint32 &count) : Object (), mItemID (GetSeqItemID ()), mValue (value), mCount (count)
{
	mpTransactionList = new TransactionList ();
}

Item::~Item ()
{
	mpTransactionList->RemoveAll ();

	delete mpTransactionList;
}

const bool Item::operator< (const Object &rObject) const
{
	bool b = GetItemID () < static_cast<const Item &>(rObject).GetItemID ();

	if (b)
		LOGMSG (MAX_LEVEL, "Item::operator< () - [%s] < [%s]\n", GetValue ().c_str (), static_cast<const Item&>(rObject).GetValue ().c_str ());
	else
		LOGMSG (MAX_LEVEL, "Item::operator< () - [%s] >= [%s]\n", GetValue ().c_str (), static_cast<const Item&>(rObject).GetValue ().c_str ());

	return b;
}

const bool Item::operator> (const Object &rObject) const
{
	bool b = GetItemID () > static_cast<const Item &>(rObject).GetItemID ();

	if (b)
		LOGMSG (MAX_LEVEL, "Item::operator> () - [%s] > [%s]\n", GetValue ().c_str (), static_cast<const Item&>(rObject).GetValue ().c_str ());
	else
		LOGMSG (MAX_LEVEL, "Item::operator> () - [%s] <= [%s]\n", GetValue ().c_str (), static_cast<const Item&>(rObject).GetValue ().c_str ());

	return b;
}

const uint32 Item::GetSeqItemID ()
{
	uint32 itemID = msSeqItemID;
	msSeqItemID++;

	return itemID;
}

const uint32& Item::GetItemID () const
{
	return mItemID;
}

const string& Item::GetValue () const
{
	return mValue;
}

void Item::IncCount ()
{
	LOGMSG (HIGH_LEVEL, "Item::IncCount () - p [%p]\n", this);

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

const TransactionList* Item::GetTransactionList () const
{
	return mpTransactionList;
}

const uint64 Item::GetFrequence () const
{
	LOGMSG (MEDIUM_LEVEL, "Item::GetFrequence () - value [%s], frequence [%llu]\n", mValue.c_str (), mpTransactionList->GetSize ());

	return mpTransactionList->GetSize ();
}

void Item::Print () const
{
	LOGMSG (NO_DEBUG, "Item::Print () - mItemID [%u], mValue [%s]\n", mItemID, mValue.c_str ());
}
