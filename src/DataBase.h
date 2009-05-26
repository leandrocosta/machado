#ifndef _DATA_BASE_H_
#define _DATA_BASE_H_


#include "ClassList.h"
#include "ItemList.h"
#include "Transaction.h"
#include "TransactionList.h"
#include "PatternList.h"
#include "RankingRule.h"
#include "base/IOManager.h"
#include "base/Tokenizer.h"


class PatternList;


class DataBase : private IOManager, private Tokenizer
{
	public:
			DataBase	(
				const float32 &support = 0.01, const float32 &confidence = 0.01,
				const uint32 &min_rule_len = 1, const uint32 &max_rule_len = 100)	;
		virtual	~DataBase	()								;

	public:
		typedef enum e_run_mode
		{
			MODE_CLASSICAL	= 'c',
			MODE_ORTHOGONAL	= 'o',
			MODE_UNKNOWN	= 0
		} RunMode;

	private:
		void	LoadData		(const bool &train_data)		;
		void	ClassifyTransaction	(
				Transaction *pTransaction,
				const RunMode &rRunMode,
				const PatternList::OrtMode &rOrtMode,
				const PatternList::OrtMethod &rOrtMethod,
				const Pattern::OrtMetric &rOrtMetric,
				const PatternList::OrtOrdering &rOrtOrdering,
				const RankingRule::RuleMeasure &rRuleMeasure,
				const uint32 &rMinNumRules,
				const uint32 &rMaxNumRankRules,
				const Transaction::PatternSet &rPatternSet,
				const float32 &rClassCoverageFactor,
				const float32 &rAlpha,
				const float32 &rBeta)					;
		void	MakeProjection		(Transaction *pTransaction)		;

	public:
		void	SetSupport		(const float32 &support)		;
		void	SetConfidence		(const float32 &confidence)		;
		void	SetMinRuleLen		(const uint32 &min_rul_len)		;
		void	SetMaxRuleLen		(const uint32 &max_rul_len)		;
		void	LoadTrainData		(const string &file)			;
		void	LoadTestData		(const string &file)			;
		void	MakeItemCoverageArrays	()					;
		void	SortTransactions	()					;
		void	ClassifyTestData	(
				const RunMode &rRunMode,
				const PatternList::OrtMode &rOrtMode,
				const PatternList::OrtMethod &rOrtMethod,
				const Pattern::OrtMetric &rOrtMetric,
				const PatternList::OrtOrdering &rOrtOrdering,
				const RankingRule::RuleMeasure &rRuleMeasure,
				const uint32 &rMinNumRules,
				const uint32 &rMaxNumRankRules,
				const Transaction::PatternSet &rPatternSet,
				const float32 &rClassCoverageFactor,
				const float32 &rAlpha,
				const float32 &rBeta)					;
		void	PrintDataInfo		()				const	;

	public:
		void	Print	()	const	;

	private:
		float32	mSupport	;
		float32	mConfidence	;
		uint32	mMinRuleLen	;
		uint32	mMaxRuleLen	;

	private:
		ClassList		mClassList			;
		ItemList		mItemList			;

		TransactionList		mTrainTransactionList		;
		TransactionList		mTestTransactionList		;
		TransactionList*	mpProjectionTransactionList	;

		uint32			mCorrectGuesses			;
		uint32			mTotalGuesses			;
		uint32			mPatterns			;
		uint32			mRules				;
		float32			mAccuracy			;
		float32			mTime				;
};


#endif
