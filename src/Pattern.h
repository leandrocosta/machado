#ifndef _PATTERN_H_
#define _PATTERN_H_


#include "ItemSet.h"

class TransactionList;


class Pattern : public ItemSet
{
	public:
			Pattern		()	;
		virtual	~Pattern	()	;

//	public:
//		virtual	const	bool	operator<	(const Object& rObject)	const	;

	public:
			void		MakeTransactionList	()				const	;
		const	uint64		GetFrequence		()				const	;

			void		SetSupport		(const float32 &support)		;
		const	float32&	GetSupport		()				const	;

	public:
		const	TransactionList*	GetTransactionList	()	const	;

	public:
			void	SetGot	(const bool &got)		;
		const	bool&	GetGot	()			const	;

	public:
		void	Print	()	const	;

	private:
		float32			mSupport		;
		TransactionList*	mpTransactionList	;
		bool			mGot			;
};


#endif
