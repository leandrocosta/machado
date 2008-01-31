#include "ItemSet.h"
#include "base/Logger.h"

#include <sstream>
using std::stringstream;


ItemSet::ItemSet () : ItemList ()
{

};

ItemSet::~ItemSet ()
{

}

const bool ItemSet::operator^ (const ItemSet &rItemSet) const
{
	bool bRet = false;

	for (STLItemList_cit it = GetBegin (); it != GetEnd (); it++)
	{
		for (uint64 i = 0; i < rItemSet.GetSize (); i++)
		{
			if (static_cast<Item *>(*it)->GetItemID () == static_cast<Item *>(rItemSet.GetAt (i))->GetItemID ())
			{
				bRet = true;
				break;
			}
		}

		if (bRet)
			break;
	}

	return bRet;
}

const string ItemSet::GetPrintableString () const
{
	stringstream	sstream			;
	Item*		pItem	= NULL		;
	STLItemList_cit	it	= GetBegin ()	;

	if (it != GetEnd ())
	{
		pItem = static_cast<Item *>(*it);
		sstream << pItem->GetValue ();
		it++;
	}

	while (it != GetEnd ())
	{
		pItem = static_cast<Item *>(*it);
		sstream << " " << pItem->GetValue ();
		it++;
	}

	return sstream.str ();
}

void ItemSet::Print () const
{
	LOGMSG (NO_DEBUG, "ItemSet::Print () - [%s]\n", GetPrintableString ().c_str ());
}
