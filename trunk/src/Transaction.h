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

	public:
		typedef enum e_pattern_set
		{
			PATTERN_FREQUENT	= 'f',
			PATTERN_MAXIMAL		= 'm',
			PATTERN_RAND_MAXIMAL	= 'r'
		} PatternSet;

	private:
		static	const	uint32	GetSeqTransactionID	()	;

	public:
		static	const	uint32	GetMaxTransactionID	()	;
		static	const	uint32	GetNumTransactions	()	;

	public:
		const	uint32&		GetTransactionID		()					const	;
			Class*		GetClass			()						;
		const	uint32&		GetClassID			()					const	;
		const	string&		GetClassValue			()					const	;
		const	bool		IsCoveredBy			(const Pattern *pPattern)		const	;
		const	bool		IsCoveredBy			(const Item *pItem)			const	;
		const	bool		IsCoveredByItem			(const uint32 &rItemID)			const	;
			Item*		GetRandomItem			()					const	;
			PatternList*	GetPatternList			(
										const float32 &support,
										const uint32 &projection_size,
										const uint32 &min_rule_len,
										const uint32 &max_rule_len,
										const PatternSet &rPatternSet)	const	;
			PatternList*	GetFrequentPatternList		(
										const float32 &support,
										const uint32 &projection_size,
										const uint32 &rMaxPatternSize)	const	;
			PatternList*	GetMaximalFrequentPatternList	(const PatternList *pFrequentPatternList)	const	;
			Pattern*	GetRandomMaximalFrequentPattern		(
											const float32 &support,
											const uint32 &projection_size)	const	;
			PatternList*	GetRandomMaximalFrequentPatternList	(
											const float32 &support,
											const uint32 &projection_size)	const	;

	public:
			void	MakeItemCoverageArray				()	;
			void	AddTransactionToItemsProjectionTransactionLists ()	;

	public:
		void	Print	()	const	;

	private:
		const	uint32			mTransactionID		;
			Class*			mpClass			;
			bool*			mItemArray		;

	private:
		static	uint32	msSeqTransactionID	;
};


#endif
