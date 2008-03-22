#include "RankingRule.h"
#include "Class.h"
#include "Pattern.h"
#include "DataBaseException.h"

#include "base/Logger.h"

#include <math.h>

RankingRule::RuleMeasure RankingRule::msPrioritizedMeasure = RankingRule::MEASURE_CONFIDENCE;

RankingRule::RankingRule (const Class *pClass, const Pattern *pPattern, const uint32 &rProjectionSize, const uint32 &rNumClasses) : Rule (pClass, pPattern)
{
	uint32	patternClassFrequence	= pPattern->GetNumTransactionsOfClass (pClass->GetClassID ());
	uint32	patternFrequence	= pPattern->GetFrequence ();
	uint32	classFrequence		= pClass->GetProjectionTransactionListSize ();

	mSupport	= (float32) patternClassFrequence / rProjectionSize;
	mConfidence	= (float32) patternClassFrequence / patternFrequence;
	mLift		= (float32) mSupport / (((float32) patternFrequence / rProjectionSize) * ((float32) classFrequence / rProjectionSize));
	mLeverage	= (float32) mSupport - (((float32) patternFrequence / rProjectionSize) * ((float32) classFrequence / rProjectionSize));
	mJaccard	= (float32) patternClassFrequence / (patternFrequence + classFrequence - patternClassFrequence);
	mCosine		= (float32) patternClassFrequence / sqrt (patternFrequence * classFrequence);
	mKulc		= (patternClassFrequence / 2.0) * (1.0 / patternFrequence + 1.0 / classFrequence);
	mGain		= (float32) mSupport * (log2 (mConfidence) - log2 ((float32) classFrequence / rProjectionSize));
	mConviction	= (float32) (patternFrequence * (rProjectionSize - classFrequence)) / ((patternFrequence - patternClassFrequence) * rProjectionSize);
	mSensitivity	= (float32) patternClassFrequence / classFrequence;
	mSpecificity	= (float32) (rProjectionSize - classFrequence - patternFrequence + patternClassFrequence) / (rProjectionSize - classFrequence);
	mLaplace	= (patternClassFrequence + 1.0) / (patternFrequence + rNumClasses);
}

RankingRule::~RankingRule ()
{

}

const bool RankingRule::operator< (const Object &rRight) const
{
	bool bRet = false;

	const RankingRule &rRule = static_cast<const RankingRule &>(rRight);

	float32 measure_left	= 0.0;
	float32 measure_right	= 0.0;

	switch (msPrioritizedMeasure)
	{
		case RankingRule::MEASURE_SUPPORT:
			measure_left	= mSupport;
			measure_right	= rRule.GetSupport ();
			break;
		case RankingRule::MEASURE_CONFIDENCE:
			measure_left	= mConfidence;
			measure_right	= rRule.GetConfidence ();
			break;
		case RankingRule::MEASURE_GAIN:
			measure_left	= mGain;
			measure_right	= rRule.GetGain ();
			break;
		case RankingRule::MEASURE_JACCARD:
			measure_left	= mJaccard;
			measure_right	= rRule.GetJaccard ();
			break;
		case RankingRule::MEASURE_KULC:
			measure_left	= mKulc;
			measure_right	= rRule.GetKulc ();
			break;
		case RankingRule::MEASURE_COSINE:
			measure_left	= mCosine;
			measure_right	= rRule.GetCosine ();
			break;
		case RankingRule::MEASURE_CONVICTION:
			measure_left	= mConviction;
			measure_right	= rRule.GetConviction ();
			break;
		case RankingRule::MEASURE_SENSITIVITY:
			measure_left	= mSensitivity;
			measure_right	= rRule.GetSensitivity ();
			break;
		case RankingRule::MEASURE_SPECIFICITY:
			measure_left	= mSpecificity;
			measure_right	= rRule.GetSpecificity ();
			break;
		case RankingRule::MEASURE_LAPLACE:
			measure_left	= mLaplace;
			measure_right	= rRule.GetLaplace ();
			break;
		case RankingRule::MEASURE_LIFT:
			measure_left	= mLift;
			measure_right	= rRule.GetLift ();
			break;
		case RankingRule::MEASURE_LEVERAGE:
			measure_left	= mLeverage;
			measure_right	= rRule.GetLeverage ();
			break;
		case RankingRule::MEASURE_UNKNOWN:
		default:
			LOGMSG (NO_DEBUG, "RankingRule::operator<() - unknown rule measure\n");
			throw DataBaseException ("Unknown rule measure");
			break;
	}

	if (measure_left < measure_right)
		bRet = true;

/*
	if (measure_left < RANK_FACTOR_LOWER * measure_right)
		bRet = true;
	else if (measure_right < RANK_FACTOR_LOWER * measure_left)
		bRet = false;

	else if (mSupport < RANK_FACTOR_LOWER * rRule.GetSupport ())
		bRet = true;
	else if (rRule.GetSupport () < RANK_FACTOR_LOWER * mSupport)
		bRet = false;

	else if (mConfidence < RANK_FACTOR_LOWER * rRule.GetConfidence ())
		bRet = true;
	else if (rRule.GetConfidence () < RANK_FACTOR_LOWER * mConfidence)
		bRet = false;

	else if (mLift < RANK_FACTOR_LOWER * rRule.GetLift ())
		bRet = true;
	else if (rRule.GetLift () < RANK_FACTOR_LOWER * mLift)
		bRet = false;

	else if (mLeverage < RANK_FACTOR_LOWER * rRule.GetLeverage ())
		bRet = true;
	else if (rRule.GetLeverage () < RANK_FACTOR_LOWER * mLeverage)
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

	else if (mConviction < RANK_FACTOR_LOWER * rRule.GetConviction ())
		bRet = true;
	else if (rRule.GetConviction () < RANK_FACTOR_LOWER * mConviction)
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

	else
	{
		uint32	leftClassFrequence	= mpClass->GetProjectionTransactionListSize ();
		uint32	rightClassFrequence	= static_cast<const RankingRule &>(rRight).GetClass ()->GetProjectionTransactionListSize ();

		if (leftClassFrequence < rightClassFrequence)
			bRet = true;
		else
			bRet = false;
	}
	*/

	return bRet;
}

const bool RankingRule::operator> (const Object &rRight) const
{
	bool bRet = false;

	const RankingRule& rRule = static_cast<const RankingRule&>(rRight);

	float32 measure_left	= 0.0;
	float32 measure_right	= 0.0;

	switch (msPrioritizedMeasure)
	{
		case RankingRule::MEASURE_SUPPORT:
			measure_left	= mSupport;
			measure_right	= rRule.GetSupport ();
			break;
		case RankingRule::MEASURE_CONFIDENCE:
			measure_left	= mConfidence;
			measure_right	= rRule.GetConfidence ();
			break;
		case RankingRule::MEASURE_GAIN:
			measure_left	= mGain;
			measure_right	= rRule.GetGain ();
			break;
		case RankingRule::MEASURE_JACCARD:
			measure_left	= mJaccard;
			measure_right	= rRule.GetJaccard ();
			break;
		case RankingRule::MEASURE_KULC:
			measure_left	= mKulc;
			measure_right	= rRule.GetKulc ();
			break;
		case RankingRule::MEASURE_COSINE:
			measure_left	= mCosine;
			measure_right	= rRule.GetCosine ();
			break;
		case RankingRule::MEASURE_CONVICTION:
			measure_left	= mConviction;
			measure_right	= rRule.GetConviction ();
			break;
		case RankingRule::MEASURE_SENSITIVITY:
			measure_left	= mSensitivity;
			measure_right	= rRule.GetSensitivity ();
			break;
		case RankingRule::MEASURE_SPECIFICITY:
			measure_left	= mSpecificity;
			measure_right	= rRule.GetSpecificity ();
			break;
		case RankingRule::MEASURE_LAPLACE:
			measure_left	= mLaplace;
			measure_right	= rRule.GetLaplace ();
			break;
		case RankingRule::MEASURE_LIFT:
			measure_left	= mLift;
			measure_right	= rRule.GetLift ();
			break;
		case RankingRule::MEASURE_LEVERAGE:
			measure_left	= mLeverage;
			measure_right	= rRule.GetLeverage ();
			break;
		case RankingRule::MEASURE_UNKNOWN:
		default:
			LOGMSG (NO_DEBUG, "RankingRule::operator<() - unknown rule measure\n");
			throw DataBaseException ("Unknown rule measure");
			break;
	}

	if (measure_left > measure_right)
		bRet = true;

/*
	if (measure_left > RANK_FACTOR_GREATER * measure_right)
		bRet = true;
	else if (measure_right > RANK_FACTOR_GREATER * measure_left)
		bRet = false;

	else if (mSupport > RANK_FACTOR_GREATER * rRule.GetSupport ())
		bRet = true;
	else if (rRule.GetSupport () > RANK_FACTOR_GREATER * mSupport)
		bRet = false;

	else if (mConfidence > RANK_FACTOR_GREATER * rRule.GetConfidence ())
		bRet = true;
	else if (rRule.GetConfidence () > RANK_FACTOR_GREATER * mConfidence)
		bRet = false;

	else if (mLift > RANK_FACTOR_GREATER * rRule.GetLift ())
		bRet = true;
	else if (rRule.GetLift () > RANK_FACTOR_GREATER * mLift)
		bRet = false;

	else if (mLeverage > RANK_FACTOR_GREATER * rRule.GetLeverage ())
		bRet = true;
	else if (rRule.GetLeverage () > RANK_FACTOR_GREATER * mLeverage)
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

	else if (mConviction > RANK_FACTOR_GREATER * rRule.GetConviction ())
		bRet = true;
	else if (rRule.GetConviction () > RANK_FACTOR_GREATER * mConviction)
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

	else
	{
		uint32	leftClassFrequence	= mpClass->GetProjectionTransactionListSize ();
		uint32	rightClassFrequence	= static_cast<const RankingRule &>(rRight).GetClass ()->GetProjectionTransactionListSize ();

		if (leftClassFrequence > rightClassFrequence)
			bRet = true;
		else
			bRet = false;
	}
*/

	return bRet;
}

void RankingRule::SetPrioritizedMeasure (const RuleMeasure &measure)
{
	msPrioritizedMeasure = measure;
}

const RankingRule::RuleMeasure& RankingRule::GetPrioritizedMeasure ()
{
	return msPrioritizedMeasure;
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

const float32& RankingRule::GetConviction () const
{
	return mConviction;
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

const float32& RankingRule::GetLift () const
{
	return mLift;
}

const float32& RankingRule::GetLeverage () const
{
	return mLeverage;
}

void RankingRule::Print () const
{
	LOGMSG (MEDIUM_LEVEL, "RankingRule::Print () - p [%p], rule [%s], mSupport [%f], mConfidence [%f], mGain [%f], mJaccard [%f], mKulc [%f], mCosine [%f], mConviction [%f], mSensitivity [%f], mSpecificity [%f], mLaplace [%f], mLift [%f], mLeverage [f]\n", this, GetPrintableString ().c_str (), mSupport, mConfidence, mGain, mJaccard, mKulc, mCosine, mConviction, mSensitivity, mSpecificity, mLaplace, mLift, mLeverage);
}
