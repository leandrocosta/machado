#include "Rule.h"
#include "Class.h"
#include "Pattern.h"
#include "base/Logger.h"

#include <math.h>

#include <iostream>
using std::cout;
using std::endl;


Rule::Rule (const Class *pClass, const Pattern *pPattern, const uint64 &projection_size, const uint64 &num_classes) : Object (), mpClass (pClass), mpPattern (pPattern)
{
	uint64 correct_transactions = pPattern->GetNumTransactionsOfClass (mpClass->GetValue ());

	mSupport	= mpPattern->GetSupport ();
//	mConfidence	= (float32) correct_transactions / mpPattern->GetFrequence ();

//	mSupport	= (float32) correct_transactions / projection_size;
	mConfidence	= (float32) correct_transactions / mpPattern->GetFrequence ();
	mGain		= mSupport * (log2 (mConfidence) - log2 (pClass->GetProjectionTransactionListSize () / projection_size));
	mJaccard	= (float32) correct_transactions / (mpPattern->GetFrequence () + pClass->GetProjectionTransactionListSize () - correct_transactions);
	mKulc		= (mSupport / 2) * ((float32) 1.0 / ((float32) mpPattern->GetFrequence () / projection_size) + (float32) 1.0 / ((float32) pClass->GetProjectionTransactionListSize () / projection_size));
	mCosine		= mSupport / sqrt (((float32) mpPattern->GetFrequence () / projection_size) * ((float32) pClass->GetProjectionTransactionListSize () / projection_size));
	mCoherence	= mSupport /((mpPattern->GetFrequence () + pClass->GetProjectionTransactionListSize () - (float32) correct_transactions) / projection_size);
	mSensitivity	= (float32) correct_transactions / pClass->GetProjectionTransactionListSize ();
	mSpecificity	= (float32) (projection_size - pClass->GetProjectionTransactionListSize () - mpPattern->GetFrequence () + correct_transactions) / (projection_size - pClass->GetProjectionTransactionListSize ());
	mLaplace	= (float32) (correct_transactions + 1) / (mpPattern->GetFrequence () + num_classes);
	mCorrelation	= mSupport / ((float32) mpPattern->GetFrequence () / projection_size * pClass->GetProjectionTransactionListSize () / projection_size);
}

Rule::~Rule ()
{
//	mTransactionList.RemoveAll ();
}

const bool Rule::operator> (const Object &rObject) const
{
	bool bRet = false;

	const Rule& rRule = static_cast<const Rule&>(rObject);

	if (mSupport + mConfidence >= rRule.GetSupport () + rRule.GetConfidence ())
		bRet = true;

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
	LOGMSG (LOW_LEVEL, "Rule::Print () - support [%0.6f], confidence [%0.6f] - class [%s] <= pattern [%s]\n", mSupport, mConfidence, mpClass->GetValue ().c_str (), mpPattern->GetPrintableString ().c_str ());

//	cout << "classe [" << mpClass->GetValue () << "], suporte [" << mSupport << "], confiança [" << mConfidence << "], padrão [" << mpPattern->GetPrintableString () << "]" << endl;
}
