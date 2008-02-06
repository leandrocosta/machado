#include "ItemList.h"
#include "ItemHash.h"
#include "base/Logger.h"

#include <iostream>
#include <sstream>

using std::cout;
using std::endl;
using std::stringstream;


ItemList::ItemList (const uint64 &max_size) : ObjectList (max_size)
{
	LOGMSG (HIGH_LEVEL, "ItemList::ItemList () - p [%p]\n", this);
}

ItemList::~ItemList ()
{
	LOGMSG (HIGH_LEVEL, "ItemList::~ItemList () - p [%p]\n", this);
}

const bool ItemList::operator< (const Object &rObject) const
{
	LOGMSG (HIGH_LEVEL, "ItemList::operator< () - p [%p]\n", this);

	bool bRet = false;

	const ItemList& rItemList = static_cast<const ItemList&>(rObject);

	STLItemList_cit itLeft		= GetBegin ();
	STLItemList_cit itLeftEnd	= GetEnd ();

	STLItemList_cit itRight		= rItemList.GetBegin ();
	STLItemList_cit itRightEnd	= rItemList.GetEnd ();

	while (itLeft != itLeftEnd && itRight != itRightEnd)
	{
		if (*itLeft < *itRight)
		{
			bRet = true;
			break;
		}
		if (*itLeft > *itRight)
		{
			bRet = false;
			break;
		}

		++itLeft;
		++itRight;
	}

	if (itLeft == itLeftEnd && itRight != itRightEnd)
		bRet = true;

	return bRet;
}

const bool ItemList::operator> (const Object &rObject) const
{
	LOGMSG (HIGH_LEVEL, "ItemList::operator> () - p [%p]\n", this);

	bool bRet = false;

	const ItemList& rItemList = static_cast<const ItemList&>(rObject);

	STLItemList_cit itLeft		= GetBegin ();
	STLItemList_cit itLeftEnd	= GetEnd ();

	STLItemList_cit itRight		= rItemList.GetBegin ();
	STLItemList_cit itRightEnd	= rItemList.GetEnd ();

	while (itLeft != itLeftEnd && itRight != itRightEnd)
	{
		if (*itLeft > *itRight)
		{
			bRet = true;
			break;
		}
		if (*itLeft < *itRight)
		{
			bRet = false;
			break;
		}

		++itLeft;
		++itRight;
	}

	if (itLeft != itLeftEnd && itRight == itRightEnd)
		bRet = true;

	return bRet;
}

const bool ItemList::operator<= (const Object &rObject) const
{
	LOGMSG (HIGH_LEVEL, "ItemList::operator<= () - p [%p]\n", this);

	bool bRet = false;

	const ItemList& rItemList = static_cast<const ItemList&>(rObject);

	STLItemList_cit itLeft		= GetBegin ();
	STLItemList_cit itLeftEnd	= GetEnd ();

	STLItemList_cit itRight		= rItemList.GetBegin ();
	STLItemList_cit itRightEnd	= rItemList.GetEnd ();

	while (itLeft != itLeftEnd && itRight != itRightEnd)
	{
		if (*itLeft < *itRight)
		{
			bRet = true;
			break;
		}
		if (*itLeft > *itRight)
		{
			bRet = false;
			break;
		}

		++itLeft;
		++itRight;
	}

	if (itLeft == itLeftEnd)
		bRet = true;

	return bRet;
}

const bool ItemList::operator>= (const Object &rObject) const
{
	LOGMSG (HIGH_LEVEL, "ItemList::operator>= () - p [%p]\n", this);

	bool bRet = false;

	const ItemList& rItemList = static_cast<const ItemList&>(rObject);

	STLItemList_cit itLeft		= GetBegin ();
	STLItemList_cit itLeftEnd	= GetEnd ();

	STLItemList_cit itRight		= rItemList.GetBegin ();
	STLItemList_cit itRightEnd	= rItemList.GetEnd ();

	while (itLeft != itLeftEnd && itRight != itRightEnd)
	{
		if (*itLeft > *itRight)
		{
			bRet = true;
			break;
		}
		if (*itLeft < *itRight)
		{
			bRet = false;
			break;
		}

		++itLeft;
		++itRight;
	}

	if (itRight == itRightEnd)
		bRet = true;

	return bRet;
}

const bool ItemList::operator== (const Object &rObject) const
{
	LOGMSG (HIGH_LEVEL, "ItemList::operator== () - p [%p]\n", this);

	bool bRet = true;

	const ItemList& rItemList = static_cast<const ItemList&>(rObject);

	if (GetSize () != rItemList.GetSize ())
	{
		bRet = false;
	}
	else
	{
		STLItemList_cit itLeft		= GetBegin ();
		STLItemList_cit itLeftEnd	= GetEnd ();

		STLItemList_cit itRight		= rItemList.GetBegin ();
		STLItemList_cit itRightEnd	= rItemList.GetEnd ();

		while (itLeft != itLeftEnd && itRight != itRightEnd)
		{
			if (*itLeft != *itRight)
			{
				bRet = false;
				break;
			}

			++itLeft;
			++itRight;
		}
	}

	return bRet;
}

Item* ItemList::GetItemByValue (const string &value) const
{
	LOGMSG (HIGH_LEVEL, "ItemList::GetItemByValue () - value [%s]\n", value.c_str ());

	Item*		pItem = NULL		;
	STLItemList_cit	it			;
	STLItemList_cit itEnd = GetEnd ()	;

	for (it = GetBegin (); it != itEnd; ++it)
	{
		if ((static_cast<Item *>(*it))->GetValue () == value)
		{
			pItem = static_cast<Item *>(*it);
			break;
		}
	}

	return pItem;
}

void ItemList::ClearItemProjectionTransactionLists ()
{
	STLItemList_cit itEnd = GetEnd ();

	for (STLItemList_cit it = GetBegin (); it != itEnd; it++)
		static_cast<Item *>(*it)->ClearProjectionTransactionList ();
}

const string ItemList::GetPrintableString () const
{
	stringstream	sstream			;
	Item*		pItem	= NULL		;
	STLItemList_cit	it	= GetBegin ()	;
	STLItemList_cit itEnd	= GetEnd ()	;

	if (it != itEnd)
	{
		pItem = static_cast<Item *>(*it);
		sstream << pItem->GetValue ();
		it++;
	}

	while (it != itEnd)
	{
		pItem = static_cast<Item *>(*it);
		sstream << " " << pItem->GetValue ();
		it++;
	}

	return sstream.str ();
}

void ItemList::Print () const
{
	LOGMSG (NO_DEBUG, "ItemList::Print () - [%s]\n", GetPrintableString ().c_str ());
}
