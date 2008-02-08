#include "Rule.h"
#include "Class.h"
#include "Pattern.h"
#include "base/Logger.h"

#include <math.h>

#include <iostream>
using std::cout;
using std::endl;


Rule::Rule (const Class *pClass, const Pattern *pPattern) : Object ()
{
	mpClass		= pClass	;
	mpPattern	= pPattern	;
}

Rule::~Rule ()
{

}

const string& Rule::GetClassValue () const
{
	return mpClass->GetValue ();
}

const Class* Rule::GetClass () const
{
	return mpClass;
}

const string Rule::GetPrintableString () const
{
	return "class [" + mpClass->GetValue () + "] <= pattern [" + mpPattern->GetPrintableString () + "]";
}

void Rule::Print () const
{
	LOGMSG (MEDIUM_LEVEL, "Rule::Print () - class [%s] <= pattern [%s]\n", mpClass->GetValue ().c_str (), mpPattern->GetPrintableString ().c_str ());
}
