#ifndef _ITEM_OBJECT_HASH_H_
#define _ITEM_OBJECT_HASH_H_


#include "base/ObjectHash.h"
#include "Item.h"


class ItemHash : public ObjectHash
{
	public:
			ItemHash	(const uint64 &max_size = UINT_MAX)	;
		virtual	~ItemHash	()					;

	public:
		typedef STLObjectHash			STLItemHash	;
		typedef STLItemHash::iterator		STLItemHash_it	;
		typedef STLItemHash::const_iterator	STLItemHash_cit	;

	public:
		Item*	Get	(const string &key)	;
};


#endif
