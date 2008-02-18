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
			METRIC_SET_SIMILARITY		= 's',
			METRIC_SET_COVERAGE		= 'c',
			METRIC_SET_SIM_COV		= 'b',
			METRIC_SET_CLASS_COVERAGE	= 'l',
			METRIC_PAIR_MEAN_CLASS_COVERAGE	= 'm',
			METRIC_ALL			= 'a',
			METRIC_UNKNOWN			= 0
		} OrtMetric;

	private:
		static	void	MakeItemPatternCoverageMatrix		(
					const PatternList *pPatternList	)	;
		static	void	MakeTransactionPatternCoverageMatrix	(
					const TransactionList *pTransactionList,
					const PatternList *pPatternList	)	;
		static	void	MakeClassPatternCoverageMatrix		(
					const TransactionList *pTransactionList,
					const PatternList *pPatternList	)	;

		static	void	DestroyItemPatternCoverageMatrix	()	;
		static	void	DestroyTransactionPatternCoverageMatrix	()	;
		static	void	DestroyClassPatternCoverageMatrix	()	;

	private:
		const	uint32*	GetPatternIDArray	()	const	;

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
			Pattern*	GetMoreSimilar			(Pattern *pPattern)		const	;
		const	float32		GetSetSimilarityRate		()					;
		const	float32		GetSetCoverageRate		(
							const TransactionList *pTransactionList)	const	;
		const	float32		GetSetClassCoverageRate		(
							const TransactionList *pTransactionList)	const	;
		const	float32		GetPairMeanSimilarityRate	()				const	;
		const	float32		GetPairMeanCoverageRate	(
							const TransactionList *pTransactionList)	const	;
		const	float32		GetPairMeanClassCoverageRate	(
							const TransactionList *pTransactionList)	const	;
		const	float32		GetRate				(
							const TransactionList *pTransactionList,
							const OrtMetric &metric)				;
		const	bool		FindSuperPatternOf		(const Pattern *pPattern)	const	;

	private:
		static	bool**		mItemPatternCoverageMatrix		;
		static	bool**		mTransactionPatternCoverageMatrix	;
		static	uint32**	mClassPatternCoverageMatrix		;
			uint32*		mPatternIDArray				;
};


#endif
