#include "Rule.h"
#include "Class.h"
#include "Pattern.h"
#include "base/Logger.h"

#include <math.h>

#include <iostream>
using std::cout;
using std::endl;


Rule::Rule (const Class *pClass, const Pattern *pPattern, const uint32 &projection_size, const uint32 &num_classes) : Object (), mpClass (pClass), mpPattern (pPattern)
{
	uint32 correct_transactions = pPattern->GetNumTransactionsOfClass (mpClass->GetValue ());

//	mSupport	= mpPattern->GetSupport ();
//	mConfidence	= (float32) correct_transactions / mpPattern->GetFrequence ();

	mSupport	= (float32) correct_transactions / projection_size;
	mConfidence	= (float32) correct_transactions / mpPattern->GetFrequence ();
	mJaccard	= (float32) correct_transactions / (mpPattern->GetFrequence () + pClass->GetProjectionTransactionListSize () - correct_transactions);
	mKulc		= (mSupport / 2) * ((float32) 1.0 / ((float32) mpPattern->GetFrequence () / projection_size) + (float32) 1.0 / ((float32) pClass->GetProjectionTransactionListSize () / projection_size));
	mGain		= mSupport * (log2 (mConfidence) - log2 ((float32) pClass->GetProjectionTransactionListSize () / projection_size));
	mLaplace	= (float32) (correct_transactions + 1) / (mpPattern->GetFrequence () + num_classes);
	mCorrelation	= mSupport / ((float32) mpPattern->GetFrequence () / projection_size * pClass->GetProjectionTransactionListSize () / projection_size);
	mCosine		= mSupport / sqrt (((float32) mpPattern->GetFrequence () / projection_size) * ((float32) pClass->GetProjectionTransactionListSize () / projection_size));
	mCoherence	= mSupport /((mpPattern->GetFrequence () + pClass->GetProjectionTransactionListSize () - (float32) correct_transactions) / projection_size);
	mSpecificity	= (float32) (projection_size - pClass->GetProjectionTransactionListSize () - mpPattern->GetFrequence () + correct_transactions) / (projection_size - pClass->GetProjectionTransactionListSize ());
	mSensitivity	= (float32) correct_transactions / pClass->GetProjectionTransactionListSize ();
}

Rule::~Rule ()
{
//	mTransactionList.RemoveAll ();
}

const bool Rule::operator< (const Object &rObject) const
{
	LOGMSG (MAX_LEVEL, "Rule::operator< () - begin \n");

	bool bRet = false;

	const Rule& rRule = static_cast<const Rule&>(rObject);

	if (mConfidence < RANK_FACTOR_LOWER * rRule.GetConfidence ())
		bRet = true;
	else if (rRule.GetConfidence () < RANK_FACTOR_LOWER * mConfidence)
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

	return bRet;
}

const bool Rule::operator> (const Object &rObject) const
{
	LOGMSG (MAX_LEVEL, "Rule::operator> () - begin \n");

	bool bRet = false;

	const Rule& rRule = static_cast<const Rule&>(rObject);

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

	return bRet;
}

const float32& Rule::GetSupport () const
{
	return mSupport;
}

const float32& Rule::GetConfidence () const
{
	return mConfidence;
}

const float32& Rule::GetGain () const
{
	return mGain;
}

const float32& Rule::GetJaccard () const
{
	return mJaccard;
}

const float32& Rule::GetKulc () const
{
	return mKulc;
}

const float32& Rule::GetCosine () const
{
	return mCosine;
}

const float32& Rule::GetCoherence () const
{
	return mCoherence;
}

const float32& Rule::GetSensitivity () const
{
	return mSensitivity;
}

const float32& Rule::GetSpecificity () const
{
	return mSpecificity;
}

const float32& Rule::GetLaplace () const
{
	return mLaplace;
}

const float32& Rule::GetCorrelation () const
{
	return mCorrelation;
}

const string& Rule::GetClassValue () const
{
	return mpClass->GetValue ();
}

void Rule::Print () const
{
//	LOGMSG (LOW_LEVEL, "Rule::Print () - confidence [%0.2f] - pattern [%s] => class [%s]\n", mConfidence, mpPattern->GetPrintableString ().c_str (), mpClass->GetValue ().c_str ());
	LOGMSG (MEDIUM_LEVEL, "Rule::Print () - support [%0.6f], confidence [%0.6f] - class [%s] <= pattern [%s]\n", mSupport, mConfidence, mpClass->GetValue ().c_str (), mpPattern->GetPrintableString ().c_str ());

//	cout << "classe [" << mpClass->GetValue () << "], suporte [" << mSupport << "], confiança [" << mConfidence << "], padrão [" << mpPattern->GetPrintableString () << "]" << endl;
}

void Rule::PrintRank () const
{
	LOGMSG (MEDIUM_LEVEL, "Rule::PrintRank () - mSupport [%f], mConfidence [%f], mGain [%f], mJaccard [%f], mKulc [%f], mCosine [%f], mCoherence [%f], mSensitivity [%f], mSpecificity [%f], mLaplace [%f], mCorrelation [%f]\n", GetSupport (), GetConfidence (), GetGain (), GetJaccard (), GetKulc (), GetCosine (), GetCoherence (), GetSensitivity (), GetSpecificity (), GetLaplace (), GetCorrelation ());
}
