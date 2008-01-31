#include "ClassList.h"
#include "base/Logger.h"

#include <iostream>
using std::cout;
using std::endl;


ClassList::ClassList (const uint64 &max_size) : ObjectList (max_size)
{
	LOGMSG (MEDIUM_LEVEL, "ClassList::ClassList () - p [%p]\n", this);
}

ClassList::~ClassList ()
{
	LOGMSG (MEDIUM_LEVEL, "ClassList::~ClassList () - p [%p]\n", this);
}

ClassList::STLClassList_it ClassList::GetBegin ()
{
	return ObjectList::GetBegin ();
}

const ClassList::STLClassList_cit ClassList::GetBegin () const
{
	return ObjectList::GetBegin ();
}

ClassList::STLClassList_it ClassList::GetEnd ()
{
	return ObjectList::GetEnd ();
}

const ClassList::STLClassList_cit ClassList::GetEnd () const
{
	return ObjectList::GetEnd ();
}

const bool ClassList::operator> (const Object &rObject) const
{
	LOGMSG (MEDIUM_LEVEL, "ClassList::operator> () - p [%p]\n", this);

	const ClassList itemList = static_cast<const ClassList&>(rObject);

	if (GetSize () != itemList.GetSize ())
		return GetSize () > itemList.GetSize ();
	else
	{
		uint64		index = 0	;

		for (index = 0; index < GetSize (); index++)
		{
			Class*	pClass1 = static_cast<Class *>(GetAt (index));
			Class*	pClass2 = static_cast<Class *>(itemList.GetAt (index));

			if (pClass1->GetValue () != pClass2->GetValue ())
				return pClass1 > pClass2;
		}

		return false;
	}
}

Class* ClassList::GetClassByValue (const string &value) const
{
	LOGMSG (HIGH_LEVEL, "ClassList::GetClassByValue () - value [%s]\n", value.c_str ());

	Class*		pClass = NULL	;
	STLClassList_cit	it		;

	for (it = GetBegin (); it != GetEnd (); ++it)
	{
		if ((static_cast<Class *>(*it))->GetValue () == value)
		{
			pClass = static_cast<Class *>(*it);
			break;
		}
	}

	return pClass;
}

/*
void ClassList::Print () const
{
	Class*		pClass = NULL;
	STLClassList_cit	it;

	for (it = GetBegin (); it != GetEnd (); ++it)
	{
		pClass = static_cast<Class *>(*it);

		cout << pClass->GetValue () << " ";
	}

	cout << endl;
}
*/
