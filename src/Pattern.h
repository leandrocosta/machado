#ifndef _PATTERN_H_
#define _PATTERN_H_


#include "ItemSet.h"
#include "TransactionList.h"
#include "base/StringUInt32Hash.h"
#include "base/UInt32Float32Hash.h"


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

	private:
		void	InitFields	()	;

	public:
			void	SetPatternID	()		;
		const	uint32&	GetPatternID	()	const	;

	public:
		const	uint32		GetFrequence		()				const	;

			void		SetSupport		(const float32 &support)		;
		const	float32&	GetSupport		()				const	;

	public:
			void			SetGot				(const bool &got)		;
		const	bool&			GetGot				()			const	;
		const	TransactionList&	GetTransactionList		()			const	;
		const	uint32			GetNumTransactionsOfClass	(const uint32 &classID)	const	;
		const	float32			GetSimilarity			(Pattern *pPattern)		;
			void			AddChildPattern			(Pattern *pPattern)		;

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
		uint32*		mClassCoverageArray	;
		bool		mGot			;
};


#endif
