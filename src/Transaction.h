#ifndef _TRANSACTION_H_
#define _TRANSACTION_H_


#include "ItemSet.h"

#include <string>
using std::string;

class Class	;


class Transaction : public ItemSet
{
	public:
			Transaction	(const Class *pClass)	;
		virtual	~Transaction	()			;

	private:
		static	const	uint32	GetSeqTransactionID	()	;

	public:
		const	uint32&	GetTransactionID	()	const	;
		const	Class*	GetClass		()	const	;

	public:
		void	Print	()	const	;

	private:
		static	uint32	msSeqTransactionID	;

	private:
		const	uint32	mTransactionID	;
		const	Class*	mpClass		;
};


#endif
