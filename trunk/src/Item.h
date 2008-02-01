#ifndef _ITEM_H_
#define _ITEM_H_


#include "base/Object.h"

#include <string>
using std::string;

class	Transaction	;
class	TransactionList	;


class Item : public Object
{
	public:
			Item	(const string &value, const uint32 &count = 1)	;
		virtual	~Item 	()						;

	public:
		virtual	const	bool	operator<	(const Object& rObject)	const	;
		virtual	const	bool	operator>	(const Object& rObject)	const	;

	private:
		static	const	uint32	GetSeqItemID	()	;

	public:
		const	uint32&	GetItemID	()	const	;
		const	string&	GetValue	()	const	;

			void	IncCount	()		;
		const	uint32& GetCount	()	const	;

	public:
			void			AddTransaction		(Transaction *pTransaction)		;
		const	TransactionList*	GetTransactionList	()				const	;

	public:
		const	uint64	GetFrequence	()	const	;

	public:
		void	Print	()	const	;

	private:
		static	uint32	msSeqItemID	;

	private:
		const	uint32	mItemID	;
		const	string	mValue	;

	private:
		TransactionList*	mpTransactionList	;
		uint32			mCount			;
};


#endif
