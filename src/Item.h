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
		virtual	const	bool	operator<=	(const Object& rObject)	const	;
		virtual	const	bool	operator>=	(const Object& rObject)	const	;
		virtual	const	bool	operator==	(const Object& rObject)	const	;
		virtual	const	bool	operator!=	(const Object& rObject)	const	;

	private:
		static	const	uint32	GetSeqItemID	()	;

	public:
		const	uint32&			GetItemID			()				const	;
		const	string&			GetValue			()				const	;

			void			IncCount			()					;
			void			SetCount			(const uint32 &count)			;
		const	uint32&			GetCount			()				const	;

			void			AddTransaction			(Transaction *pTransaction)		;
			void			AddProjectTransaction		(Transaction *pTransaction)		;
		const	TransactionList*	GetProjectionTransactionList	()				const	;

		const	uint32			GetProjectionFrequence		()				const	;

			void			ClearProjectionTransactionList	()					;

	public:
		void	Print	()	const	;

	private:
		static	uint32	msSeqItemID	;

	private:
		const	uint32			mItemID				;
		const	string			mValue				;

			uint32			mCount				;
			TransactionList*	mpTransactionList		;
			TransactionList*	mpProjectionTransactionList	;
};


#endif
