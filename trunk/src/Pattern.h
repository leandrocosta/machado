#ifndef _PATTERN_H_
#define _PATTERN_H_


#include "ItemSet.h"
#include "TransactionList.h"


class Pattern : public ItemSet
{
	public:
			Pattern		(const Pattern *pPattern)	;
			Pattern		(Item *pItem)			;
			Pattern		()				;
		virtual	~Pattern	()				;

	private:
		void	InitFields	()	;

	public:
			void		MakeTransactionList	()					;
			void		MakeTransactionList	(Item *pItem)				;
		const	uint64		GetFrequence		()				const	;

			void		SetSupport		(const float32 &support)		;
		const	float32&	GetSupport		()				const	;

	public:
			void			AddItem			(Item *pItem)			;
			void			SetGot			(const bool &got)		;
		const	bool&			GetGot			()			const	;
		const	TransactionList&	GetTransactionList	()			const	;

	public:
		const	float32	GetSimilarityL	(const Pattern *pPattern)	;
		const	float32	GetSimilarityH	(const Pattern *pPattern)	;

	public:
		void	Print	()	const	;

	private:
		float32		mSupport		;
		TransactionList	mTransactionList	;
		bool		mGot			;
};


#endif
