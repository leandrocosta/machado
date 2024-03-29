#ifndef _CLASS_H_
#define _CLASS_H_


#include "base/Object.h"
#include "TransactionList.h"

#include <string>
using std::string;


class Class : public Object
{
	public:
			Class	(const string &value)	;
		virtual	~Class 	()			;

	private:
		static	const	uint32	GetSeqClassID	()	;

	public:
		static		void	SetNumTrainClasses	()	;
//		static	const	uint32	GetMaxClassID		()	;
		static	const	uint32	GetNumTrainClasses	()	;

	public:
		const	uint32&	GetClassID	()	const	;
		const	string&	GetValue	()	const	;

	public:
			void	AddTransaction				(Transaction *pTransaction)		;
			void	AddProjectionTransaction		(Transaction *pTransaction)		;
			void	ClearProjectionTransactionList		()					;
		const	uint32	GetProjectionTransactionListSize	()				const	;

	public:
		void	Print	()	const	;

	private:
		static	uint32	msSeqClassID		;
		static	uint32	msNumTrainClasses	;

	private:
		const	uint32	mClassID	;
		const	string	mValue		;

	private:
		TransactionList	mTransactionList		;
		TransactionList	mProjectionTransactionList	;
};


#endif
