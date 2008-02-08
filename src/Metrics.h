#ifndef _METRICS_H_
#define _METRICS_H_


#include "base/Object.h"

class	Class;
class	Pattern;


class Metrics : public Object
{
	public:
			Metrics		(
					const Class *pClass,
					const Pattern *pPattern,
					const uint32 &proj_size,
					const uint32 &num_classes)	;
		virtual	~Metrics	()				;

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
};


#endif
