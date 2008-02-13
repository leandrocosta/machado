#ifndef _PATTERN_H_
#define _PATTERN_H_


#include "ItemSet.h"
#include "TransactionList.h"
#include "base/StringUInt32Hash.h"
#include "base/UInt32Float32Hash.h"


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
		void	InitFields	()		;
//		void	AddItem		(Item *pItem)	;

	public:
			void	SetPatternID	()		;
		const	uint32&	GetPatternID	()	const	;

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
		const	float32	GetSimilarity		(Pattern *pPattern)		;
//			void	IncClassCoverage	(const string &class_name)	;
//		const	uint32	GetClassCoverage	(const string &class_name)	;
//			void	ResetClassCoverage	()				;

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
		bool		mGot			;

	private:
//		StringUInt32Hash	mClassCoverageHsh		;
//		StringUInt32Hash	mNumTransactionsOfClassHsh	;
//		UInt32Float32Hash	mPatternSimilarityHsh		;

//		hash_map<string, uint32>	mClassCoverageHsh		;
		hash_map<string, uint32>	mNumTransactionsOfClassHsh	;
};


#endif
