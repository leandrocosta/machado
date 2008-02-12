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
		const	uint32&		GetTransactionID	()				const	;
			Class*		GetClass		()					;
		const	string&		GetClassValue		()				const	;
		const	bool		IsCoveredBy		(const Pattern *pPattern)		;
		const	bool		IsCoveredBy		(const Item *pItem)			;
			PatternList*	GetFrequentPatternList	(
								const float32 &support,
								const uint32 &projection_size,
								const uint32 &min_rule_len,
								const uint32 &max_rule_len)	const	;
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
