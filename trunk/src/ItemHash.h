#ifndef _ITEM_OBJECT_HASH_H_
#define _ITEM_OBJECT_HASH_H_


#include "base/ObjectHash.h"
#include "Item.h"


class ItemHash : public ObjectHash
{
	public:
			ItemHash	(const uint64 &max_size = ULONG_MAX)	;
		virtual	~ItemHash	()					;

	public:
		typedef STLObjectHash			STLItemHash	;
		typedef STLItemHash::iterator		STLItemHash_it	;
		typedef STLItemHash::const_iterator	STLItemHash_cit	;

	public:
			STLItemHash_it	GetBegin	()		;
		const	STLItemHash_cit	GetBegin	()	const	;
			STLItemHash_it	GetEnd		()		;
		const	STLItemHash_cit	GetEnd		()	const	;

	public:
		Item*	Get	(const string &key)	;
};


#endif
