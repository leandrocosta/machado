#include "Metrics.h"
#include "Class.h"
#include "Pattern.h"

#include <math.h>


Metrics::Metrics (
		const Class *pClass,
		const Pattern *pPattern,
		const uint64 &proj_size,
		const uint64 &num_classes)
{
	uint64	correct_trans	= pPattern->GetNumTransactionsOfClass (pClass);
	float32	pattern_support	= pPattern->GetSupport ();
	uint64	pattern_freq	= pPattern->GetFrequence ();
	uint64	class_proj_freq	= pClass->GetProjectionTransactionListSize ();

	mSupport	= (float32) correct_trans / proj_size;
	mConfidence	= (float32) correct_trans / pattern_freq;
	mGain		= mSupport * (log2 (mConfidence) - log2 (class_proj_freq / proj_size));
	mJaccard	= (float32) correct_trans / (pattern_freq + class_proj_freq - correct_trans);
	mKulc		= (mSupport / 2) * ((float32) 1.0 / ((float32) pattern_freq / proj_size) + (float32) 1.0 / ((float32) class_proj_freq / proj_size));
	mCosine		= mSupport / sqrt (((float32) pattern_freq / proj_size) * ((float32) class_proj_freq / proj_size));
	mCoherence	= mSupport /((pattern_freq + class_proj_freq - (float32) correct_trans) / proj_size);
	mSensitivity	= (float32) correct_trans / class_proj_freq;
	mSpecificity	= (float32) (proj_size - class_proj_freq - pattern_freq + correct_trans) / (proj_size - class_proj_freq);
	mLaplace	= (float32) (correct_trans + 1) / (pattern_freq + num_classes);
	mCorrelation	= mSupport / ((float32) pattern_freq / proj_size * class_proj_freq / proj_size);
}

Metrics::~Metrics ()
{

}

const float32& Metrics::GetSupport () const
{
	return mSupport;
}

const float32& Metrics::GetConfidence () const
{
	return mConfidence;
}

const float32& Metrics::GetGain () const
{
	return mGain;
}

const float32& Metrics::GetJaccard () const
{
	return mJaccard;
}

const float32& Metrics::GetKulc () const
{
	return mKulc;
}

const float32& Metrics::GetCosine () const
{
	return mCosine;
}

const float32& Metrics::GetCoherence () const
{
	return mCoherence;
}

const float32& Metrics::GetSensitivity () const
{
	return mSensitivity;
}

const float32& Metrics::GetSpecificity () const
{
	return mSpecificity;
}

const float32& Metrics::GetLaplace () const
{
	return mLaplace;
}

const float32& Metrics::GetCorrelation () const
{
	return mCorrelation;
}
