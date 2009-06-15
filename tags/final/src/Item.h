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
			Item	(const string &value)	;
		virtual	~Item 	()			;

	public:
		virtual	const	bool	operator<	(const Object& rRight)	const	;
		virtual	const	bool	operator>	(const Object& rRight)	const	;
		virtual	const	bool	operator<=	(const Object& rRight)	const	;
		virtual	const	bool	operator>=	(const Object& rRight)	const	;
		virtual	const	bool	operator==	(const Object& rRight)	const	;
		virtual	const	bool	operator!=	(const Object& rRight)	const	;

	private:
		static	const	uint32	GetSeqItemID	()	;

	public:
		static		void	SetNumTrainItems	()	;
//		static	const	uint32	GetMaxItemID		()	;
		static	const	uint32	GetNumTrainItems	()	;

	public:
		const	uint32&			GetItemID			()				const	;
		const	string&			GetValue			()				const	;

			void			AddTransaction			(Transaction *pTransaction)		;
			void			AddProjectionTransaction	(Transaction *pTransaction)		;
		const	TransactionList*	GetProjectionTransactionList	()				const	;

		const	uint32			GetProjectionFrequence		()				const	;

			void			ClearProjectionTransactionList	()					;

	public:
		void	Print	()	const	;

	private:
		static	uint32	msSeqItemID	;
		static	uint32	msNumTrainItems	;

	private:
		const	uint32			mItemID				;
		const	string			mValue				;

			TransactionList*	mpTransactionList		;
			TransactionList*	mpProjectionTransactionList	;
};


#endif
