#ifndef _DATA_BASE_H_
#define _DATA_BASE_H_


#include "IOManager.h"
#include "Tokenizer.h"
#include "ClassList.h"
#include "ItemList.h"
#include "TransactionList.h"

#define	MODE_ORTHOGONAL	'o'
#define	MODE_CLASSICAL	'c'


typedef enum e_run_mode
{
	CLASSICAL	= 'c',
	ORTHOGONAL	= 'o',
	RUNKNOWN		= 0
} e_rmode;

typedef enum e_orthogonality_mode
{
	SIMILARITY	= 's',
	COVERAGE	= 'c',
	BOTH		= 'b',
	OUNKNOWN	= 0
} e_omode;



class PatternList	;
class RuleList		;


class DataBase : private IOManager, private Tokenizer
{
	public:
			DataBase	(
				const float32 &support = 0.01, const float32 &confidence = 0.01,
				const uint32 &min_rule_len = 1, const uint32 &max_rule_len = 100)	;
		virtual	~DataBase	()								;

	public:
		void	SetRunMode	(const e_rmode &mode)	;
		void	SetOrtMode	(const e_omode &mode)	;
		void	SetDataFile	(const string &file)	;
		void	LoadTrainData	()			;
		void	LoadTestData	()			;

	public:
		void	ClassifyTestData	()	;

	private:
		void	ClassifyTransaction	(Transaction *pTransaction)	;

	private:
		uint64		GetProjectionSize		(Transaction *pTransaction)	const	;
		PatternList*	GetFrequentPatternList		(
						const Transaction *pTransaction,
						const uint64 &projection_size)			const	;
		PatternList*	GetOrthogonalPatternList		(PatternList *pPatternList)	const	;
		PatternList*	GetOrthogonalPatternListHeuristical	(PatternList *pPatternList)	const	;
		PatternList*	GetOrthogonalPatternListPolynomial	(PatternList *pPatternList)	const	;
		RuleList*	GetRuleList				(PatternList *pPatternList)	const	;

	public:
		void	Print	()	const	;

	private:
		float32	mSupport	;
		float32	mConfidence	;
		uint32	mMinRuleLen	;
		uint32	mMaxRuleLen	;

		ClassList	mClassList		;
		ItemList	mItemList		;
		TransactionList	mTrainTransactionList	;
		TransactionList	mTestTransactionList	;

		e_rmode	m_rmode;
		e_omode m_omode;

		uint32	mCorrectGuesses;
		uint32	mWrongGuesses;
};


#endif
