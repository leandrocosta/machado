#ifndef _PATTERN_LIST_H_
#define _PATTERN_LIST_H_


#include "base/ObjectList.h"
#include "Pattern.h"


class TransactionList	;
class RankingRuleList	;
class ClassList		;


class PatternList : public ObjectList
{
	public:
			PatternList	(const uint64 &max_size = UINT_MAX)	;
		virtual	~PatternList	()					;

	public:
		typedef STLObjectList			STLPatternList		;
		typedef STLPatternList::iterator	STLPatternList_it	;
		typedef STLPatternList::const_iterator	STLPatternList_cit	;

		typedef enum e_orthogonality_mode
		{
			ORTH_HEURISTICAL	= 'h',
			ORTH_POLYNOMIAL		= 'p'
		} OrtMode;

		typedef enum e_orthogonality_metric
		{
			METRIC_SIMILARITY		= 's',
			METRIC_COVERAGE			= 'c',
			METRIC_SIM_COV			= 'b',
			METRIC_CLASS_COVERAGE		= 'l',
			METRIC_CLASS_MEAN_COVERAGE	= 'm',
			METRIC_ALL			= 'a',
			METRIC_UNKNOWN			= 0
		} OrtMetric;

//	public:
//		void	PushFront	(Object *pObject)	;
//		void	PushBack	(Object *pObject)	;

	public:
		PatternList*	GetOrthogonalPatternList		(
					const TransactionList *pTransactionList,
					const OrtMode &mode, const OrtMetric &metric)	;
		PatternList*	GetOrthogonalPatternListHeuristical	(
					const TransactionList *pTransactionList,
					const OrtMetric &metric)			;
		PatternList*	GetOrthogonalPatternListPolynomial	(
					const TransactionList *pTransactionList,
					const OrtMetric &metric)			;
		PatternList*	GetOrthogonalPatternListPolynomial	(
					const TransactionList *pTransactionList,
					const OrtMetric &metric,
					const uint32 &num_patterns)			;

	public:
		RankingRuleList*	GetRuleList	(
					const ClassList *pClassList,
					const float32 &confidence,
					const uint32 &projection_size,
					const uint32 &min_num_rules	)	const	;

	public:
//		const	uint32&		GetMaxPatternLen			()			const	;
		const	uint32		GetSumPatternLen			()			const	;
			Pattern*	GetMoreSimilar				(Pattern *pPattern)	const	;
		const	float32		GetSimilarityRate			()				;
		const	float32		GetCoverageRate				(
						const TransactionList *pTransactionList)		const	;
		const	float32		GetClassCoverageRate			(
						const TransactionList *pTransactionList)		const	;
		const	float32		GetClassCoverageMeanRate		(
						const TransactionList *pTransactionList)		const	;
		const	float32		GetRate					(
						const TransactionList *pTransactionList,
						const OrtMetric &metric)					;

	private:
//		uint32	mMaxPatternLen	;
};


#endif
