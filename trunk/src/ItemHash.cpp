#include "ItemHash.h"
#include "Item.h"
#include "base/Logger.h"


ItemHash::ItemHash (const uint64 &max_size) : ObjectHash (max_size)
{

}

ItemHash::~ItemHash ()
{
	STLItemHash_cit it;
	STLItemHash_cit itEnd = GetEnd ();

	for (it = GetBegin (); it != itEnd; ++it)
		delete (static_cast<Item *>((*it).second));
}

Item* ItemHash::Get (const string &key)
{
	return static_cast<Item *>(ObjectHash::Get (key));
}
