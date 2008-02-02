#ifndef _TRANSACTION_H_
#define _TRANSACTION_H_


#include "ItemSet.h"

#include <string>
using std::string;

class Class		;
class Pattern		;
class PatternList	;


class Transaction : public ItemSet
{
	public:
			Transaction	(const Class *pClass)	;
		virtual	~Transaction	()			;

	private:
		static	const	uint32	GetSeqTransactionID	()	;

	public:
		const	uint32&		GetTransactionID	()				const	;
		const	Class*		GetClass		()				const	;
		const	bool		IsCoveredBy		(const Pattern *pPattern)	const	;
			PatternList*	GetFrequentPatternList	(
								const float32 &support,
								const uint64 &projection_size,
								const uint32 &min_rule_len,
								const uint32 &max_rule_len)	const	;

	public:
		void	Print	()	const	;

	private:
		const	uint32	mTransactionID	;
		const	Class*	mpClass		;

	private:
		static	uint32	msSeqTransactionID	;
};


#endif
