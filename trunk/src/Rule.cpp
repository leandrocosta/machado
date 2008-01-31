#include "Rule.h"
#include "Class.h"
#include "Pattern.h"
#include "TransactionList.h"
#include "base/Logger.h"

#include <iostream>
using std::cout;
using std::endl;


Rule::Rule (const Class *pClass, const Pattern *pPattern) : Object (), mpClass (pClass), mpPattern (pPattern)
{
	mSupport		= -1			;
	mConfidence		= -1			;
	mpTransactionList	= new TransactionList ();
}

Rule::~Rule ()
{
	mpTransactionList->RemoveAll ();

	delete mpTransactionList;
}

const bool Rule::operator> (const Object &rObject) const
{
	bool bRet = false;

	const Rule& rRule = static_cast<const Rule&>(rObject);

	if (mSupport + mConfidence >= rRule.GetSupport () + rRule.GetConfidence ())
		bRet = true;

	return bRet;
}

void Rule::MakeTransactionList ()
{
	LOGMSG (MEDIUM_LEVEL, "Rule::MakeTransactionList () - begin\n");

	const TransactionList *pTransactionList = mpPattern->GetTransactionList ();

	for (uint32 i = 0; i < pTransactionList->GetSize (); i++)
	{
		Transaction *pTransaction = static_cast<Transaction *>(pTransactionList->GetAt (i));

		if (pTransaction->GetClass () == mpClass)
			mpTransactionList->PushBack (pTransaction);
	}

	mSupport	= mpPattern->GetSupport ();
	mConfidence	= (float32) mpTransactionList->GetSize () / mpPattern->GetTransactionList ()->GetSize ();
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
