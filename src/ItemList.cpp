#include "ItemList.h"
#include "ItemHash.h"
#include "base/Logger.h"

#include <iostream>
using std::cout;
using std::endl;


ItemList::ItemList (const uint64 &max_size) : ObjectList (max_size)
{
	LOGMSG (HIGH_LEVEL, "ItemList::ItemList () - p [%p]\n", this);
}

ItemList::~ItemList ()
{
	LOGMSG (HIGH_LEVEL, "ItemList::~ItemList () - p [%p]\n", this);
}

ItemList::STLItemList_it ItemList::GetBegin ()
{
	return ObjectList::GetBegin ();
}

const ItemList::STLItemList_cit ItemList::GetBegin () const
{
	return ObjectList::GetBegin ();
}

ItemList::STLItemList_it ItemList::GetEnd ()
{
	return ObjectList::GetEnd ();
}

const ItemList::STLItemList_cit ItemList::GetEnd () const
{
	return ObjectList::GetEnd ();
}

const bool ItemList::operator== (const Object &rObject) const
{
	LOGMSG (HIGH_LEVEL, "ItemList::operator== () - p [%p]\n", this);

	bool ret = true;

	const ItemList& rItemList = static_cast<const ItemList&>(rObject);

	if (GetSize () != rItemList.GetSize ())
	{
		ret = false;
	}
	else
	{
		for (uint64 i = 0; i < GetSize (); i++)
		{
			Item*	pItem1 = static_cast<Item *>(GetAt (i));
			Item*	pItem2 = static_cast<Item *>(rItemList.GetAt (i));

			if (pItem1->GetValue () != pItem2->GetValue ())
			{
				ret = false;
				break;
			}
		}
	}

	return ret;
}

const bool ItemList::operator< (const Object &rObject) const
{
	LOGMSG (HIGH_LEVEL, "ItemList::operator< () - p [%p]\n", this);

	bool ret = true;

	const ItemList& rItemList = static_cast<const ItemList&>(rObject);

	if (GetSize () != rItemList.GetSize ())
	{
		ret = GetSize () < rItemList.GetSize ();
	}
	else
	{
		for (uint64 i = 0; i < GetSize (); i++)
		{
			Item*	pItem1 = static_cast<Item *>(GetAt (i));
			Item*	pItem2 = static_cast<Item *>(rItemList.GetAt (i));

			if (pItem1->GetValue () > pItem2->GetValue ())
			{
				ret = false;
				break;
			}
		}
	}

	return ret;
}

const bool ItemList::operator> (const Object &rObject) const
{
	LOGMSG (HIGH_LEVEL, "ItemList::operator> () - p [%p]\n", this);

	bool ret = true;

	const ItemList& rItemList = static_cast<const ItemList&>(rObject);

	if (GetSize () != rItemList.GetSize ())
	{
		ret = GetSize () > rItemList.GetSize ();
	}
	else
	{
		uint64		index = 0	;

		for (index = 0; index < GetSize (); index++)
		{
			Item*	pItem1 = static_cast<Item *>(GetAt (index));
			Item*	pItem2 = static_cast<Item *>(rItemList.GetAt (index));

			if (pItem1->GetValue () < pItem2->GetValue ())
			{
				ret = false;
				break;
			}
		}
	}

	return ret;
}

Item* ItemList::GetItemByValue (const string &value) const
{
	LOGMSG (HIGH_LEVEL, "ItemList::GetItemByValue () - value [%s]\n", value.c_str ());

	Item*		pItem = NULL	;
	STLItemList_cit	it		;

	for (it = GetBegin (); it != GetEnd (); ++it)
	{
		if ((static_cast<Item *>(*it))->GetValue () == value)
		{
			pItem = static_cast<Item *>(*it);
			break;
		}
	}

	return pItem;
}

const float32 ItemList::GetSimilarity (const ItemList *pItemList) const
{
	LOGMSG (MEDIUM_LEVEL, "ItemList::GetSimilarity () - begin [%p]\n", this);

	float32		similarity	= 0.0	;
	const Item*	pItem		= NULL	;
	STLItemList_cit	itList			;
	ItemHash	Hash			;

	for (itList = GetBegin (); itList != GetEnd (); ++itList)
	{
		pItem = static_cast<const Item *>(*itList);

		LOGMSG (HIGH_LEVEL, "ItemList::GetSimilarity () - key [%s]\n", pItem->GetValue ().c_str ());

		if (! Hash.Find (pItem->GetValue ()))
			Hash.Add (pItem->GetValue (), new Item (pItem->GetValue ()));
		else
			Hash.Get (pItem->GetValue ())->IncCount ();
	}

	for (itList = pItemList->GetBegin (); itList != pItemList->GetEnd (); ++itList)
	{
		pItem = static_cast<const Item *>(*itList);

		LOGMSG (HIGH_LEVEL, "ItemList::GetSimilarity () - key [%s]\n", pItem->GetValue ().c_str ());

		if (! Hash.Find (pItem->GetValue ()))
			Hash.Add (pItem->GetValue (), new Item (pItem->GetValue ()));
		else
			Hash.Get (pItem->GetValue ())->IncCount ();
	}

	uint32 num	= 0	;
	uint32 den	= 0	;

	ItemHash::STLItemHash_cit itHash	;

	for (itHash = Hash.GetBegin (); itHash != Hash.GetEnd (); itHash++)
	{
		pItem = static_cast<const Item *>(itHash->second);

		den += pItem->GetCount ();

		if (pItem->GetCount () > 1)
			num += pItem->GetCount ();

		LOGMSG (HIGH_LEVEL, "ItemList::GetSimilarity () - key [%s], count [%u]\n", itHash->first.c_str (), pItem->GetCount ());
	}

	similarity = (float32) num / den;

	return similarity;
}
