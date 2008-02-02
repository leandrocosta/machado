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

	const ItemList& rItemList = static_cast<const ItemList&>(rObject);

	uint32 min_size = GetSize () <= rItemList.GetSize () ? GetSize ():rItemList.GetSize ();

	bool	bRet	= true;
	uint32	i	= 0;

	while (i < min_size)
	{
		if (*(GetAt (i)) < *(rItemList.GetAt (i)))
		{
			bRet = true;
			break;
		}
		if (*(GetAt (i)) > *(rItemList.GetAt (i)))
		{
			bRet = false;
			break;
		}

		i++;
	}

	if (i == min_size)
		bRet = (min_size < rItemList.GetSize ());

	return bRet;
}

const bool ItemList::operator> (const Object &rObject) const
{
	LOGMSG (HIGH_LEVEL, "ItemList::operator> () - p [%p]\n", this);

	const ItemList& rItemList = static_cast<const ItemList&>(rObject);

	uint32 min_size = GetSize () <= rItemList.GetSize () ? GetSize ():rItemList.GetSize ();

	bool	bRet	= true;
	uint32	i	= 0;

	while (i < min_size)
	{
		if (*(GetAt (i)) > *(rItemList.GetAt (i)))
		{
			bRet = true;
			break;
		}
		if (*(GetAt (i)) < *(rItemList.GetAt (i)))
		{
			bRet = false;
			break;
		}

		i++;
	}

	if (i == min_size)
		bRet = (min_size > rItemList.GetSize ());

	return bRet;
}

const bool ItemList::operator<= (const Object &rObject) const
{
	LOGMSG (HIGH_LEVEL, "ItemList::operator<= () - p [%p]\n", this);

	const ItemList& rItemList = static_cast<const ItemList&>(rObject);

	uint32 min_size = GetSize () <= rItemList.GetSize () ? GetSize ():rItemList.GetSize ();

	bool	bRet	= true;
	uint32	i	= 0;

	while (i < min_size)
	{
		if (*(GetAt (i)) < *(rItemList.GetAt (i)))
		{
			bRet = true;
			break;
		}
		if (*(GetAt (i)) > *(rItemList.GetAt (i)))
		{
			bRet = false;
			break;
		}

		i++;
	}

	if (i == min_size)
		bRet = (min_size <= rItemList.GetSize ());

	return bRet;
}

const bool ItemList::operator>= (const Object &rObject) const
{
	LOGMSG (HIGH_LEVEL, "ItemList::operator> () - p [%p]\n", this);

	const ItemList& rItemList = static_cast<const ItemList&>(rObject);

	uint32 min_size = GetSize () <= rItemList.GetSize () ? GetSize ():rItemList.GetSize ();

	bool	bRet	= true;
	uint32	i	= 0;

	while (i < min_size)
	{
		if (*(GetAt (i)) > *(rItemList.GetAt (i)))
		{
			bRet = true;
			break;
		}
		if (*(GetAt (i)) < *(rItemList.GetAt (i)))
		{
			bRet = false;
			break;
		}

		i++;
	}

	if (i == min_size)
		bRet = (min_size >= rItemList.GetSize ());

	return bRet;
}

const bool ItemList::operator== (const Object &rObject) const
{
	LOGMSG (HIGH_LEVEL, "ItemList::operator== () - p [%p]\n", this);

	bool bRet = true;

	const ItemList& rItemList = static_cast<const ItemList&>(rObject);

	const uint64 size = GetSize ();

	if (size != rItemList.GetSize ())
	{
		bRet = false;
	}
	else
	{
		for (uint64 i = 0; i < size; i++)
		{
			if (*(GetAt (i)) != *(rItemList.GetAt (i)))
			{
				bRet = false;
				break;
			}
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
