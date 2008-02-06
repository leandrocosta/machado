#include "Rule.h"
#include "Class.h"
#include "Pattern.h"
#include "base/Logger.h"

#include <iostream>
using std::cout;
using std::endl;


Rule::Rule (const Class *pClass, const Pattern *pPattern) : Object (), mpClass (pClass), mpPattern (pPattern)
{
	uint32 correct_transactions = pPattern->GetNumTransactionsOfClass (mpClass);

/*
	const TransactionList &rTransactionList = mpPattern->GetTransactionList ();

	TransactionList::STLTransactionList_cit itEnd = rTransactionList.GetEnd ();

	for (TransactionList::STLTransactionList_cit it = rTransactionList.GetBegin (); it != itEnd; it++)
	{
		Transaction *pTransaction = static_cast<Transaction *>(*it);

		if (pTransaction->GetClass () == mpClass)
			correct_transactions++;
//			mTransactionList.PushBack (pTransaction);
	}
*/

	mSupport	= mpPattern->GetSupport ();
//	mConfidence	= (float32) mTransactionList.GetSize () / mpPattern->GetFrequence ();
	mConfidence	= (float32) correct_transactions / mpPattern->GetFrequence ();
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

const string& Rule::GetClassValue () const
{
	return mpClass->GetValue ();
}

void Rule::Print () const
{
//	LOGMSG (LOW_LEVEL, "Rule::Print () - confidence [%0.2f] - pattern [%s] => class [%s]\n", mConfidence, mpPattern->GetPrintableString ().c_str (), mpClass->GetValue ().c_str ());
	LOGMSG (LOW_LEVEL, "Rule::Print () - confidence [%0.2f] - class [%s] <= pattern [%s]\n", mConfidence, mpClass->GetValue ().c_str (), mpPattern->GetPrintableString ().c_str ());

	cout << "classe [" << mpClass->GetValue () << "], suporte [" << mSupport << "], confiança [" << mConfidence << "], padrão [" << mpPattern->GetPrintableString () << "]" << endl;
}
