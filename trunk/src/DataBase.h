#ifndef _DATA_BASE_H_
#define _DATA_BASE_H_


#include "ClassList.h"
#include "ItemList.h"
#include "TransactionList.h"
#include "base/IOManager.h"
#include "base/Tokenizer.h"


typedef enum e_run_mode
{
	MODE_CLASSICAL	= 'c',
	MODE_ORTHOGONAL	= 'o',
	MODE_UNKNOWN	= 0
} e_rmode;

typedef enum e_orthogonality_mode
{
	ORTH_HEURISTICAL	= 'h',
	ORTH_POLYNOMIAL		= 'p'
} e_omode;

typedef enum e_orthogonality_metric
{
	METRIC_SIMILARITY	= 's',
	METRIC_COVERAGE		= 'c',
	METRIC_BOTH		= 'b',
	METRIC_UNKNOWN		= 0
} e_ometric;


class PatternList	;
class RuleList		;


class DataBase : private IOManager, private Tokenizer
{
	public:
			DataBase	(
				const float32 &support = 0.01, const float32 &confidence = 0.01,
				const uint32 &min_rule_len = 1, const uint32 &max_rule_len = 100)	;
		virtual	~DataBase	()								;

	private:
		void	ClassifyTransaction	(Transaction *pTransaction)	;

	public:
		void	SetSupport		(const float32 &support)	;
		void	SetConfidence		(const float32 &confidence)	;
		void	SetMinRuleLen		(const uint32 &min_rul_len)	;
		void	SetMaxRuleLen		(const uint32 &max_rul_len)	;
		void	LoadTrainData		(const string &file)		;
		void	LoadTestData		(const string &file)		;
		void	SortTransactions	()				;
		void	ClassifyTestData	()				;

	public:
		void	Print	()	const	;

	private:
		float32	mSupport	;
		float32	mConfidence	;
		uint32	mMinRuleLen	;
		uint32	mMaxRuleLen	;

	private:
		ClassList	mClassList		;
		ItemList	mItemList		;
		TransactionList	mTrainTransactionList	;
		TransactionList	mTestTransactionList	;

	private:
		uint32	mCorrectGuesses	;
		uint32	mWrongGuesses	;
};


#endif
