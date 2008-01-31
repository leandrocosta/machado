#ifndef _ITEM_SET_H_
#define _ITEM_SET_H_


#include "ItemList.h"


class ItemSet : public ItemList
{
	public:
			ItemSet		()	;
		virtual	~ItemSet	()	;

	public:
		virtual	const	bool	operator^	(const ItemSet& rItemSet)	const	;

	public:
		virtual	const	string	GetPrintableString	()	const	;

	public:
		virtual	void	Print	()	const	;
};


#endif
