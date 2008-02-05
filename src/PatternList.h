#ifndef _PATTERN_LIST_H_
#define _PATTERN_LIST_H_


#include "base/ObjectList.h"
#include "Pattern.h"


class TransactionList	;
class RuleList		;
class ClassList		;


class PatternList : public ObjectList
{
	public:
			PatternList		(const uint64 &max_size = ULONG_MAX)	;
		virtual	~PatternList	()					;

	public:
		typedef STLObjectList			STLPatternList		;
		typedef STLPatternList::iterator	STLPatternList_it	;
		typedef STLPatternList::const_iterator	STLPatternList_cit	;

	public:
		void	PushFront	(Object *pObject)	;
		void	PushBack	(Object *pObject)	;

	public:
		PatternList*	GetOrthogonalPatternList		(const TransactionList *pTransactionList)	;
		PatternList*	GetOrthogonalPatternListHeuristical	(const TransactionList *pTransactionList)	;
		PatternList*	GetOrthogonalPatternListPolynomial	(const TransactionList *pTransactionList)	;

	public:
		RuleList*	GetRuleList	(const ClassList *pClassList, const float32 &confidence)	const	;

	public:
		const	uint32&		GetMaxPatternLen	()				const	;
		const	uint32		GetSumPatternLen	()				const	;
			Pattern*	GetMoreSimilar		(const Pattern *pPattern)	const	;
		const	float32		GetSimilarityRate	()					;
		const	float32		GetCoverageRate		(
						const TransactionList *pTransactionList)	const	;
		const	float32		GetClassCoverageRate	(
						const TransactionList *pTransactionList)	const	;
		const	float32		GetRate			(
						const TransactionList *pTransactionList)		;

	private:
		uint32	mMaxPatternLen	;
};


#endif
