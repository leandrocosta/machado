#ifndef _RANKING_RULE_H_
#define _RANKING_RULE_H_


#include "Rule.h"


class RankingRule : public Rule
{
	public:
			RankingRule	(
				const Class *pClass,
				const Pattern *pPattern,
				const uint32 &projection_size,
				const uint32 &num_classes)	;
		virtual	~RankingRule	()			;

	public:
		virtual	const	bool	operator<	(const Object& rRight)	const	;
		virtual	const	bool	operator>	(const Object& rRight)	const	;

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

	public:
		void	Print	()	const	;

	private:
		float32	mSupport	;
		float32	mConfidence	;
		float32	mGain		;
		float32	mJaccard	;
		float32	mKulc		;
		float32	mCosine		;
		float32	mCoherence	;
		float32	mSensitivity	;
		float32	mSpecificity	;
		float32	mLaplace	;
		float32	mCorrelation	;
	private:
		static	const	float32	RANK_FACTOR_LOWER	= /* 1.00 */ 0.95	;
		static	const	float32	RANK_FACTOR_GREATER	= /* 1.00 */ 1.05	;
};


#endif
