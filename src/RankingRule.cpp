#include "RankingRule.h"
#include "Class.h"
#include "Pattern.h"

#include "base/Logger.h"

#include <math.h>


RankingRule::RankingRule (const Class *pClass, const Pattern *pPattern, const uint32 &projection_size, const uint32 &num_classes) : Rule (pClass, pPattern)
{
	uint32	correct_trans	= pPattern->GetNumTransactionsOfClass (mpClass->GetValue ());
	uint32	pattern_freq	= pPattern->GetFrequence ();
	uint32	class_freq	= pClass->GetProjectionTransactionListSize ();

	mSupport	= (float32) correct_trans / projection_size;
	mConfidence	= (float32) correct_trans / pattern_freq;
	mGain		= mSupport * (log2 (mConfidence) - log2 ((float32) class_freq / projection_size));
	mJaccard	= (float32) correct_trans / (pattern_freq + class_freq - correct_trans);
	mKulc		= (mSupport / 2) * ((float32) 1.0 / ((float32) pattern_freq / projection_size) + (float32) 1.0 / ((float32) class_freq / projection_size));
	mCosine		= mSupport / sqrt (((float32) pattern_freq / projection_size) * ((float32) class_freq / projection_size));
	mCoherence	= mSupport /((pattern_freq + class_freq - (float32) correct_trans) / projection_size);
	mSensitivity	= (float32) correct_trans / class_freq;
	mSpecificity	= (float32) (projection_size - class_freq - pattern_freq + correct_trans) / (projection_size - class_freq);
	mLaplace	= (float32) (correct_trans + 1) / (pattern_freq + num_classes);
	mCorrelation	= mSupport / ((float32) pattern_freq / projection_size * class_freq / projection_size);

//	mSupport	= mpPattern->GetSupport ();
//	mConfidence	= (float32) correct_transactions / mpPattern->GetFrequence ();
}

RankingRule::~RankingRule ()
{

}

const bool RankingRule::operator< (const Object &rRight) const
{
	bool bRet = false;

	const RankingRule &rRule = static_cast<const RankingRule &>(rRight);

	LOGMSG (NO_DEBUG, "RankingRule::operator< () - p [%p]\n", this);
	LOGMSG (NO_DEBUG, "RankingRule::operator< () - &rRule [%p]\n", &rRule);

	float32 right_confidence = rRule.GetConfidence ();
	LOGMSG (NO_DEBUG, "RankingRule::operator< () - right_confidence [%f]\n", right_confidence);

	if (mConfidence < RANK_FACTOR_LOWER * right_confidence)
		bRet = true;
	else if (right_confidence < RANK_FACTOR_LOWER * mConfidence)
		bRet = false;

	else if (mGain < RANK_FACTOR_LOWER * rRule.GetGain ())
		bRet = true;
	else if (rRule.GetGain () < RANK_FACTOR_LOWER * mGain)
		bRet = false;

	else if (mJaccard < RANK_FACTOR_LOWER * rRule.GetJaccard ())
		bRet = true;
	else if (rRule.GetJaccard () < RANK_FACTOR_LOWER * mJaccard)
		bRet = false;

	else if (mKulc < RANK_FACTOR_LOWER * rRule.GetKulc ())
		bRet = true;
	else if (rRule.GetKulc () < RANK_FACTOR_LOWER * mKulc)
		bRet = false;

	else if (mCosine < RANK_FACTOR_LOWER * rRule.GetCosine ())
		bRet = true;
	else if (rRule.GetCosine () < RANK_FACTOR_LOWER * mCosine)
		bRet = false;

	else if (mCoherence < RANK_FACTOR_LOWER * rRule.GetCoherence ())
		bRet = true;
	else if (rRule.GetCoherence () < RANK_FACTOR_LOWER * mCoherence)
		bRet = false;

	else if (mSensitivity < RANK_FACTOR_LOWER * rRule.GetSensitivity ())
		bRet = true;
	else if (rRule.GetSensitivity () < RANK_FACTOR_LOWER * mSensitivity)
		bRet = false;

	else if (mSpecificity < RANK_FACTOR_LOWER * rRule.GetSpecificity ())
		bRet = true;
	else if (rRule.GetSpecificity () < RANK_FACTOR_LOWER * mSpecificity)
		bRet = false;

	else if (mLaplace < RANK_FACTOR_LOWER * rRule.GetLaplace ())
		bRet = true;
	else if (rRule.GetLaplace () < RANK_FACTOR_LOWER * mLaplace)
		bRet = false;

	else if (mCorrelation < RANK_FACTOR_LOWER * rRule.GetCorrelation ())
		bRet = true;
	else if (rRule.GetCorrelation () < RANK_FACTOR_LOWER * mCorrelation)
		bRet = false;

	else if (mSupport < RANK_FACTOR_LOWER * rRule.GetSupport ())
		bRet = true;
	else if (rRule.GetSupport () < RANK_FACTOR_LOWER * mSupport)
		bRet = false;

	else
	{
		uint32	class_freq		= mpClass->GetProjectionTransactionListSize ();
		uint32	class_freq_right	= static_cast<const RankingRule &>(rRight).GetClass ()->GetProjectionTransactionListSize ();

		if (class_freq < class_freq_right)
			bRet = true;
		else
			bRet = false;
	}

	return bRet;
}

const bool RankingRule::operator> (const Object &rRight) const
{
	bool bRet = false;

	const RankingRule& rRule = static_cast<const RankingRule&>(rRight);

	LOGMSG (NO_DEBUG, "RankingRule::operator> () - p [%p]\n", this);
	LOGMSG (NO_DEBUG, "RankingRule::operator> () - &rRule [%p]\n", &rRule);

	float32 right_confidence = rRule.GetConfidence ();
	LOGMSG (NO_DEBUG, "RankingRule::operator> () - right_confidence [%f]\n", right_confidence);

	if (mConfidence > RANK_FACTOR_GREATER * rRule.GetConfidence ())
		bRet = true;
	else if (rRule.GetConfidence () > RANK_FACTOR_GREATER * mConfidence)
		bRet = false;

	else if (mGain > RANK_FACTOR_GREATER * rRule.GetGain ())
		bRet = true;
	else if (rRule.GetGain () > RANK_FACTOR_GREATER * mGain)
		bRet = false;

	else if (mJaccard > RANK_FACTOR_GREATER * rRule.GetJaccard ())
		bRet = true;
	else if (rRule.GetJaccard () > RANK_FACTOR_GREATER * mJaccard)
		bRet = false;

	else if (mKulc > RANK_FACTOR_GREATER * rRule.GetKulc ())
		bRet = true;
	else if (rRule.GetKulc () > RANK_FACTOR_GREATER * mKulc)
		bRet = false;

	else if (mCosine > RANK_FACTOR_GREATER * rRule.GetCosine ())
		bRet = true;
	else if (rRule.GetCosine () > RANK_FACTOR_GREATER * mCosine)
		bRet = false;

	else if (mCoherence > RANK_FACTOR_GREATER * rRule.GetCoherence ())
		bRet = true;
	else if (rRule.GetCoherence () > RANK_FACTOR_GREATER * mCoherence)
		bRet = false;

	else if (mSensitivity > RANK_FACTOR_GREATER * rRule.GetSensitivity ())
		bRet = true;
	else if (rRule.GetSensitivity () > RANK_FACTOR_GREATER * mSensitivity)
		bRet = false;

	else if (mSpecificity > RANK_FACTOR_GREATER * rRule.GetSpecificity ())
		bRet = true;
	else if (rRule.GetSpecificity () > RANK_FACTOR_GREATER * mSpecificity)
		bRet = false;

	else if (mLaplace > RANK_FACTOR_GREATER * rRule.GetLaplace ())
		bRet = true;
	else if (rRule.GetLaplace () > RANK_FACTOR_GREATER * mLaplace)
		bRet = false;

	else if (mCorrelation > RANK_FACTOR_GREATER * rRule.GetCorrelation ())
		bRet = true;
	else if (rRule.GetCorrelation () > RANK_FACTOR_GREATER * mCorrelation)
		bRet = false;

	else if (mSupport > RANK_FACTOR_GREATER * rRule.GetSupport ())
		bRet = true;
	else if (rRule.GetSupport () > RANK_FACTOR_GREATER * mSupport)
		bRet = false;
	else
	{
		uint32	class_freq		= mpClass->GetProjectionTransactionListSize ();
		uint32	class_freq_right	= static_cast<const RankingRule &>(rRight).GetClass ()->GetProjectionTransactionListSize ();

		if (class_freq > class_freq_right)
			bRet = true;
		else
			bRet = false;
	}



	return bRet;
}

const float32& RankingRule::GetSupport () const
{
	return mSupport;
}

const float32& RankingRule::GetConfidence () const
{
	return mConfidence;
}

const float32& RankingRule::GetGain () const
{
	return mGain;
}

const float32& RankingRule::GetJaccard () const
{
	return mJaccard;
}

const float32& RankingRule::GetKulc () const
{
	return mKulc;
}

const float32& RankingRule::GetCosine () const
{
	return mCosine;
}

const float32& RankingRule::GetCoherence () const
{
	return mCoherence;
}

const float32& RankingRule::GetSensitivity () const
{
	return mSensitivity;
}

const float32& RankingRule::GetSpecificity () const
{
	return mSpecificity;
}

const float32& RankingRule::GetLaplace () const
{
	return mLaplace;
}

const float32& RankingRule::GetCorrelation () const
{
	return mCorrelation;
}

void RankingRule::Print () const
{
	LOGMSG (HIGH_LEVEL, "RankingRule::Print () - p [%p], rule [%s], mSupport [%f], mConfidence [%f], mGain [%f], mJaccard [%f], mKulc [%f], mCosine [%f], mCoherence [%f], mSensitivity [%f], mSpecificity [%f], mLaplace [%f], mCorrelation [%f]\n", this, GetPrintableString ().c_str (), mSupport, mConfidence, mGain, mJaccard, mKulc, mCosine, mCoherence, mSensitivity, mSpecificity, mLaplace, mCorrelation);
}
