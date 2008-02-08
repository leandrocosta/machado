#ifndef _PATTERN_H_
#define _PATTERN_H_


#include "ItemSet.h"
#include "TransactionList.h"
#include "base/stl_hash_include.h"


class Pattern : public ItemSet
{
	public:
			Pattern		(const Pattern *pPattern, Item *pItem)	;
			Pattern		(Item *pItem)				;
		virtual	~Pattern	()					;

	private:
		void	InitFields	()		;
		void	AddItem		(Item *pItem)	;

	public:
		const	uint32		GetFrequence		()				const	;

			void		SetSupport		(const float32 &support)		;
		const	float32&	GetSupport		()				const	;

	public:
			void			SetGot				(const bool &got)			;
		const	bool&			GetGot				()				const	;
		const	TransactionList&	GetTransactionList		()				const	;
		const	uint32			GetNumTransactionsOfClass	(const string &class_name)	const	;

	public:
		const	float32	GetSimilarity		(const Pattern *pPattern)	;
			void	IncClassCoverage	(const string &class_name)	;
		const	uint32&	GetClassCoverage	(const string &class_name)	;
			void	ResetClassCoverage	()				;

	public:
		void	Print	()	const	;

	private:
		uint32		mFrequence		;
		float32		mSupport		;
		TransactionList	mTransactionList	;
		bool		mGot			;

	private:
		hash_map<string, uint32>	mClassCoverageHsh		;
		hash_map<string, uint32>	mNumTransactionsOfClassHsh	;
};


#endif
