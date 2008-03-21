#ifndef _RANKING_RULE_H_
#define _RANKING_RULE_H_


#include "Rule.h"


class RankingRule : public Rule
{
	public:
			RankingRule	(
				const Class *pClass,
				const Pattern *pPattern,
				const uint32 &rProjectionSize,
				const uint32 &rNumClasses)	;
		virtual	~RankingRule	()			;

	public:
		typedef enum e_rule_measure
		{
			MEASURE_SUPPORT		= 's',
			MEASURE_CONFIDENCE	= 'c',
			MEASURE_GAIN		= 'g',
			MEASURE_JACCARD		= 'j',
			MEASURE_KULC		= 'k',
			MEASURE_COSINE		= 'o',
			MEASURE_CONVICTION	= 'n',
			MEASURE_SENSITIVITY	= 'e',
			MEASURE_SPECIFICITY	= 'p',
			MEASURE_LAPLACE		= 'l',
			MEASURE_LIFT		= 'i',
			MEASURE_LEVERAGE	= 'v',
			MEASURE_UNKNOWN		= 0
		} RuleMeasure;

	public:
		static		void		SetPrioritizedMeasure	(const RuleMeasure &measure)	;
		static	const	RuleMeasure&	GetPrioritizedMeasure	()				;

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
		const	float32&	GetConviction	()	const	;
		const	float32&	GetSensitivity	()	const	;
		const	float32&	GetSpecificity	()	const	;
		const	float32&	GetLaplace	()	const	;
		const	float32&	GetLift		()	const	;
		const	float32&	GetLeverage	()	const	;

	public:
		void	Print	()	const	;

	private:
		float32	mSupport	;
		float32	mConfidence	;
		float32	mGain		;
		float32	mJaccard	;
		float32	mKulc		;
		float32	mCosine		;
		float32	mConviction	;
		float32	mSensitivity	;
		float32	mSpecificity	;
		float32	mLaplace	;
		float32	mLift		;
		float32	mLeverage	;

	private:
		static	RuleMeasure	msPrioritizedMeasure;

	private:
		static	const	float32	RANK_FACTOR_LOWER	= /* 1.00 */ 0.95	;
		static	const	float32	RANK_FACTOR_GREATER	= /* 1.00 */ 1.05	;
};


#endif
