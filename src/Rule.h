#ifndef _RULE_H_
#define _RULE_H_


#include "base/Object.h"
#include "TransactionList.h"

#include <string>
using std::string;

class	Class		;
class	Pattern		;


class Rule : public Object
{
	public:
		Rule		(
				const Class *pClass,
				const Pattern *pPattern,
				const uint32 &projection_size,
				const uint32 &num_classes)	;
		virtual	~Rule	()				;

	public:
		virtual	const	bool	operator<	(const Object& rObject)	const	;
		virtual	const	bool	operator>	(const Object& rObject)	const	;

	public:
		const	float32&	GetSupport	()	const	;
		const	float32&	GetConfidence	()	const	;
		const	float32&	GetGain		()	const	;
		const	float32&	GetJaccard	()	const	;
		const	float32&	GetKulc		()	const	;
		const	float32&	GetCosine	()	const	;
		const	float32&	GetCoherence	()	const	;
		const	float32&	GetSensitivity	()	const	;
		const	float32&	GetSpecificity	()	const	;
		const	float32&	GetLaplace	()	const	;
		const	float32&	GetCorrelation	()	const	;

		const	string&		GetClassValue	()	const	;

	public:
		void	Print		()	const	;
		void	PrintRank	()	const	;

	private:
			float32		mSupport	;
			float32		mConfidence	;
			float32		mGain		;
			float32		mJaccard	;
			float32		mKulc		;
			float32		mCosine		;
			float32		mCoherence	;
			float32		mSensitivity	;
			float32		mSpecificity	;
			float32		mLaplace	;
			float32		mCorrelation	;

		const	Class*		mpClass		;
		const	Pattern*	mpPattern	;

	private:
		static	const	float32	RANK_FACTOR_LOWER	= 0.95	;
		static	const	float32	RANK_FACTOR_GREATER	= 1.05	;
};


#endif
