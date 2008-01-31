#ifndef _ITEM_LIST_H_
#define _ITEM_LIST_H_


#include "base/ObjectList.h"
#include "Item.h"

#include <string>
using std::string;


class ItemList : public ObjectList
{
	public:
			ItemList	(const uint64 &max_size = ULONG_MAX)	;
		virtual	~ItemList	()					;

	public:
		typedef STLObjectList			STLItemList	;
		typedef STLItemList::iterator		STLItemList_it	;
		typedef STLItemList::const_iterator	STLItemList_cit	;

	public:
			STLItemList_it	GetBegin	()		;
		const	STLItemList_cit	GetBegin	()	const	;
			STLItemList_it	GetEnd		()		;
		const	STLItemList_cit	GetEnd		()	const	;

	public:
		virtual	const	bool	operator==	(const Object& rObject)	const	;
		virtual	const	bool	operator<	(const Object& rObject)	const	;
		virtual	const	bool	operator>	(const Object& rObject)	const	;

	public:
			Item*	GetItemByValue	(const string &value)	const	;
		const	float32	GetSimilarity	(const ItemList *pItemList)	const	;

//	public:
//		void	Print	()	const	;
};


#endif
