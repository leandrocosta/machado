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
			METRIC_SIMILARITY	= 's',
			METRIC_TRANS_COVERAGE	= 'c',
			METRIC_CLASS_COVERAGE	= 'l',
			METRIC_ALL		= 'a',
			METRIC_UNKNOWN		= 0
		} OrtMetric;

		typedef enum e_orthogonality_method
		{
			METHOD_SET		= 's',
			METHOD_PAIR_AVERAGE	= 'p',
			METHOD_ALL		= 'a',
			METHOD_UNKNOWN		= 0
		} OrtMethod;

		typedef enum e_orthogonality_pattern_ordering
		{
			ORDERING_SORTED			= 's',
			ORDERING_REVERSE_SORTED		= 'r',
			ORDERING_SIZE_SORTED		= 'i',
			ORDERING_SIZE_REVERSE_SORTED	= 'z',
			ORDERING_NONE			= 'n',
			ORDERING_UNKNOWN		= 0
		} OrtOrdering;

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
		const	uint32*	GetPatternIDArray	()				const	;
			void	MakeOrdering		(const OrtOrdering &ordering)		;

	public:
		PatternList*	GetOrthogonalPatternList			(
					const TransactionList *pTransactionList,
					const OrtMode &mode,
					const OrtMethod &method,
					const OrtMetric &metric,
					const OrtOrdering &ordering)			;
		PatternList*	GetOrthogonalPatternListHeuristical		(
					const TransactionList *pTransactionList,
					const OrtMethod &method,
					const OrtMetric &metric,
					const OrtOrdering &ordering)			;
		PatternList*	GetOrthogonalPatternListClassHeuristical	(
					const TransactionList *pTransactionList,
					const OrtMethod &method,
					const OrtMetric &metric,
					const OrtOrdering &ordering)			;
		PatternList*	GetOrthogonalPatternListPolynomial		(
					const TransactionList *pTransactionList,
					const OrtMethod &method,
					const OrtMetric &metric)			;
		PatternList*	GetOrthogonalPatternListPolynomial		(
					const TransactionList *pTransactionList,
					const OrtMethod &method,
					const OrtMetric &metric,
					const uint32 &num_patterns)			;

	public:
		RankingRuleList*	GetRuleList	(
					const ClassList *pClassList,
					const float32 &confidence,
					const uint32 &projection_size,
					const uint32 &min_num_rules)		const	;

	public:
			Pattern*	GetMoreSimilar			(Pattern *pPattern)		const	;
		const	float32		GetSetSimilarityRate		()				const	;
		const	float32		GetSetCoverageRate		(
							const TransactionList *pTransactionList)	const	;
		const	float32		GetSetClassCoverageRate		(
							const TransactionList *pTransactionList)	const	;
		const	float32		GetPairAverageSimilarityRate	()				const	;
		const	float32		GetPairAverageCoverageRate	(
							const TransactionList *pTransactionList)	const	;
		const	float32		GetPairAverageClassCoverageRate	(
							const TransactionList *pTransactionList)	const	;
		const	float32		GetRate				(
							const TransactionList *pTransactionList,
							const OrtMethod &method,
							const OrtMetric &metric)			const	;
		const	float32		GetSetRate			(
							const TransactionList *pTransactionList,
							const OrtMetric &metric)			const	;
		const	float32		GetPairAverageRate		(
							const TransactionList *pTransactionList,
							const OrtMetric &metric)			const	;
		const	bool		FindSuperPatternOf		(const Pattern *pPattern)	const	;

	private:
		static	bool**		mItemPatternCoverageMatrix		;
		static	bool**		mTransactionPatternCoverageMatrix	;
		static	uint32**	mClassPatternCoverageMatrix		;
			uint32*		mPatternIDArray				;
};


#endif
