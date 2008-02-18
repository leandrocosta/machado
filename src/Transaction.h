#ifndef _TRANSACTION_H_
#define _TRANSACTION_H_


#include "ItemSet.h"
#include "base/stl_hash_include.h"

#include <string>
using std::string;

class Class		;
class Pattern		;
class PatternList	;


class Transaction : public ItemSet
{
	public:
			Transaction	(Class *pClass)	;
		virtual	~Transaction	()		;

	private:
		static	const	uint32	GetSeqTransactionID	()	;

	public:
		static	const	uint32	GetMaxTransactionID	()	;

	public:
		const	uint32&		GetTransactionID		()					const	;
			Class*		GetClass			()						;
		const	uint32&		GetClassID			()					const	;
		const	string&		GetClassValue			()					const	;
		const	bool		IsCoveredBy			(const Pattern *pPattern)		const	;
		const	bool		IsCoveredBy			(const Item *pItem)			const	;
		const	bool		IsCoveredByItem			(const uint32 &rItemID)			const	;
			PatternList*	GetPatternList			(
										const float32 &support,
										const uint32 &projection_size,
										const uint32 &min_rule_len,
										const uint32 &max_rule_len,
										const bool &rUseMaximalPatterns)const	;
			PatternList*	GetFrequentPatternList		(
										const float32 &support,
										const uint32 &projection_size,
										const uint32 &rMaxPatternSize)	const	;
			PatternList*	GetMaximalFrequentPatternList	(const PatternList *pFrequentPatternList)	const	;

	public:
			void	MakeItemCoverageArray				(const uint32 &num_items)	;
			void	AddTransactionToItemsProjectionTransactionLists ()				;

	public:
		void	Print	()	const	;

	private:
		const	uint32			mTransactionID		;
			Class*			mpClass			;
			bool*			mItemCoverageArray	;
			hash_map<uint32, bool>	mPatternCoverageHsh	;

	private:
		static	uint32	msSeqTransactionID	;
};


#endif
