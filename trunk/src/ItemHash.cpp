#include "ItemHash.h"
#include "Item.h"
#include "base/Logger.h"


ItemHash::ItemHash (const uint64 &max_size) : ObjectHash (max_size)
{

}

ItemHash::~ItemHash ()
{
	STLItemHash_cit it;

	for (it = GetBegin (); it != GetEnd (); ++it)
		delete (static_cast<Item *>((*it).second));
}

ItemHash::STLItemHash_it ItemHash::GetBegin ()
{
	return ObjectHash::GetBegin ();
}

const ItemHash::STLItemHash_cit ItemHash::GetBegin () const
{
	return ObjectHash::GetBegin ();
}

ItemHash::STLItemHash_it ItemHash::GetEnd ()
{
	return ObjectHash::GetEnd ();
}

const ItemHash::STLItemHash_cit ItemHash::GetEnd () const
{
	return ObjectHash::GetEnd ();
}

Item* ItemHash::Get (const string &key)
{
	return static_cast<Item *>(ObjectHash::Get (key));
}
