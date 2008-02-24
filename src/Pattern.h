#ifndef _PATTERN_H_
#define _PATTERN_H_


#include "ItemSet.h"
#include "TransactionList.h"


class PatternList;


class Pattern : public ItemSet
{
	public:
			Pattern		(const Pattern *pPattern, Item *pItem)	;
			Pattern		(Item *pItem)				;
		virtual	~Pattern	()					;

	private:
		static	const	uint32	GetSeqPatternID	()	;

	public:
		static	const	uint32	GetMaxPatternID	()	;
		static		void	ResetSeqPatternID ()	;

	private:
		void	InitFields	()	;

	public:
			void	SetPatternID	()		;
		const	uint32&	GetPatternID	()	const	;

	public:
		const	uint32		GetFrequence		()				const	;
		const	float32		GetAmbiguity		()				const	;

			void		SetSupport		(const float32 &support)		;
		const	float32&	GetSupport		()				const	;

	public:
		const	TransactionList&	GetTransactionList		()				const	;
		const	uint32			GetNumTransactionsOfClass	(const uint32 &classID)		const	;
		const	float32			GetSimilarity			(Pattern *pPattern)			;
			void			AddChildPattern			(Pattern *pPattern)			;
			PatternList*		GetMaximalPatternList		()					;
		const	bool			IsSuperPatternOf		(const Pattern *pPattern)	const	;
		const	bool*			GetItemArray			()				const	;

	public:
		void	SetSimilarityHsh	(const uint32 &rPatternID, const float32 &similarity)	;

	public:
		void	Print	()	const	;

	private:
		static	uint32	msSeqPatternID	;

	private:
		uint32	mPatternID	;

	private:
		uint32		mFrequence		;
		float32		mSupport		;
		TransactionList	mTransactionList	;
		PatternList*	mpChildPatternList	;
		bool*		mItemArray		;
		uint32*		mClassCoverageArray	;
};


#endif
