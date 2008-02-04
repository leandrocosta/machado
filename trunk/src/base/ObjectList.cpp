#include "ObjectList.h"
#include "ObjectListException.h"
#include "Logger.h"

#include <typeinfo>
#include <stdexcept>
#include <algorithm>

using std::out_of_range;
using std::find;


ObjectList::ObjectList (const uint64 &max_size) : Object ()
{
#ifdef USE_LOGGER
	LOGMSG (HIGH_LEVEL, "ObjectList::ObjectList () - [%p]\n", this);
#endif

	mMaxSize = max_size;
}

ObjectList::~ObjectList ()
{
#ifdef USE_LOGGER
	LOGMSG (HIGH_LEVEL, "ObjectList::~ObjectList () - [%p]\n", this);
#endif

	STLObjectList_cit it;

	for (it = mList.begin (); it != mList.end (); it++)
		delete (*it);
	
	mList.clear ();
}

const bool ObjectList::CompareLess::operator() (const Object *pLeft, const Object *pRight) const
{
#ifdef USE_LOGGER
	if (! pLeft || ! pRight)
		LOGMSG (NO_DEBUG, "ObjectList::CompareLess () - pLeft [%p], pRight [%p]\n", pLeft, pRight);
#endif

	return (*pLeft < *pRight);
}

const bool ObjectList::CompareLessEqual::operator() (const Object *pLeft, const Object *pRight) const
{
#ifdef USE_LOGGER
	if (! pLeft || ! pRight)
		LOGMSG (NO_DEBUG, "ObjectList::CompareLessEqual () - pLeft [%p], pRight [%p]\n", pLeft, pRight);
#endif

	return (*pLeft <= *pRight);
}

const bool ObjectList::CompareGreater::operator() (const Object *pLeft, const Object *pRight) const
{
#ifdef USE_LOGGER
	if (! pLeft || ! pRight)
		LOGMSG (NO_DEBUG, "ObjectList::CompareGreater () - pLeft [%p], pRight [%p]\n", pLeft, pRight);
#endif

	return (*pLeft > *pRight);
}

const bool ObjectList::CompareGreaterEqual::operator() (const Object *pLeft, const Object *pRight) const
{
#ifdef USE_LOGGER
	if (! pLeft || ! pRight)
		LOGMSG (NO_DEBUG, "ObjectList::CompareGreaterEqual () - pLeft [%p], pRight [%p]\n", pLeft, pRight);
#endif

	return (*pLeft >= *pRight);
}

const bool ObjectList::HasIntersectionByPtr (const ObjectList *pLeft, const ObjectList *pRight)
{
	bool bRet = false;

	STLObjectList_cit itLeftEnd	= pLeft->GetEnd ();
	STLObjectList_cit itRightEnd	= pRight->GetEnd ();

	for (STLObjectList_cit itLeft = pLeft->GetBegin (); itLeft != itLeftEnd; itLeft++)
	{
		for (STLObjectList_cit itRight = pRight->GetBegin (); itRight != itRightEnd; itRight++)
		{
			if (*itLeft == *itRight)
			{
				bRet = true;
				break;
			}
		}

		if (bRet)
			break;
	}

	return bRet;
}

ObjectList::STLObjectList_it ObjectList::GetBegin ()
{
	return mList.begin ();
}

const ObjectList::STLObjectList_cit ObjectList::GetBegin () const
{
	return mList.begin ();
}

ObjectList::STLObjectList_it ObjectList::GetEnd ()
{
	return mList.end ();
}

const ObjectList::STLObjectList_cit ObjectList::GetEnd () const
{
	return mList.end ();
}

void ObjectList::Sort ()
{
#ifdef USE_LOGGER
	LOGMSG (MEDIUM_LEVEL, "ObjectList::Sort () - size [%llu] - begin\n", GetSize ());
#endif
	sort (mList.begin (), mList.end (), ObjectList::msLessComparer);
//	sort (mList.begin (), mList.end (), ObjectList::msLessEqualComparer);
#ifdef USE_LOGGER
	LOGMSG (HIGH_LEVEL, "ObjectList::Sort () - size [%llu] - end\n", GetSize ());
#endif
}
void ObjectList::ReverseSort ()
{
#ifdef USE_LOGGER
	LOGMSG (MEDIUM_LEVEL, "ObjectList::ReverseSort () - size [%llu] - begin\n", GetSize ());
#endif
	sort (mList.begin (), mList.end (), ObjectList::msGreaterComparer);
//	sort (mList.begin (), mList.end (), ObjectList::msGreaterEqualComparer);
#ifdef USE_LOGGER
	LOGMSG (HIGH_LEVEL, "ObjectList::ReverseSort () - size [%llu] - end\n", GetSize ());
#endif
}

const uint64 ObjectList::GetSize () const
{
#ifdef USE_LOGGER
	LOGMSG (HIGH_LEVEL, "ObjectList::GetSize () - [%p]\n", this);
#endif

	return mList.size ();
}

void ObjectList::SetMaxSize (const uint64 &max_size)
{
	mMaxSize = max_size;
}

void ObjectList::PushFront (Object *pObject)
{
	if (mList.size () < mMaxSize)
		mList.push_front (pObject);
}

void ObjectList::PushBack (Object *pObject)
{
#ifdef USE_LOGGER
	LOGMSG (HIGH_LEVEL, "ObjectList::PushBack () - [%p]\n", this);
#endif

	if (mList.size () < mMaxSize)
		mList.push_back (pObject);

#ifdef USE_LOGGER
	LOGMSG (HIGH_LEVEL, "ObjectList::PushBack () - end\n");
#endif
}

Object* ObjectList::PopFront ()
{
	Object *pObject = NULL;

	if (mList.size ())
	{
		pObject = mList.front ();
		mList.pop_front ();
	}

	return pObject;
}

Object* ObjectList::PopBack ()
{
	Object *pObject = NULL;

	if (mList.size ())
	{
		pObject = mList.back ();
		mList.pop_back ();
	}

	return pObject;
}

Object* ObjectList::PopAt (const uint64 &index)
{
	Object *pObject = NULL;

	try
	{
		pObject = mList.at (index);
		mList.erase (mList.begin () + index);
	}
	catch (out_of_range &e)
	{
		throw (ObjectListException ("Out of range access in " + string (typeid (*this).name ())));
	}

	return pObject;
}

Object* ObjectList::GetFront () const
{
	return mList.front ();
}

Object* ObjectList::GetBack () const
{
	return mList.back ();
}

Object* ObjectList::GetAt (const uint64 &index) const
{
	Object *pObject = NULL;

	try
	{
		pObject = mList.at (index);
	}
	catch (out_of_range &e)
	{
		throw (ObjectListException ("Out of range access in " + string (typeid (*this).name ())));
	}

	return pObject;
}

void ObjectList::MoveToEnd (Object *pObject)
{
	ObjectList::Remove	(pObject);
	ObjectList::PushBack	(pObject);
}

void ObjectList::Remove (Object *pObject)
{
	STLObjectList_it it;

	for (it = mList.begin (); it != mList.end (); it++)
	{
		if (*it == pObject)
		{
			mList.erase (it);
			break;
		}
	}
}

void ObjectList::RemoveAll ()
{
	mList.clear ();
}

void ObjectList::DeleteAll ()
{
#ifdef USE_LOGGER
	LOGMSG (MEDIUM_LEVEL, "ObjectList::DeleteAll () - begin [%p]\n", this);
#endif

	STLObjectList_it it;

	for (it = mList.begin (); it != mList.end (); it++)
		delete (*it);

	mList.clear ();

#ifdef USE_LOGGER
	LOGMSG (HIGH_LEVEL, "ObjectList::DeleteAll () - end [%p]\n", this);
#endif
}

ObjectList::STLObjectList_it ObjectList::Erase (STLObjectList_it it)
{
#ifdef USE_LOGGER
	LOGMSG (HIGH_LEVEL, "ObjectList::Erase () - [%p]\n", *it);
#endif

	return mList.erase (it);
}

const bool ObjectList::Find (const Object *pObject) const
{
	bool bRet = false;

	STLObjectList_cit it;

	for (it = mList.begin (); it != mList.end (); it++)
	{
		if (*pObject == *(*it))
		{
			bRet = true;
			break;
		}
	}

	return bRet;
}

const bool ObjectList::FindByPtr (const Object *pObject) const
{
	/*
	bool bRet = false;

	for (STLObjectList_cit it = mList.begin (); it != mList.end (); it++)
	{
		if (*it == pObject)
		{
			bRet = true;
			break;
		}
	}

	return bRet;
	*/

	return find (mList.begin (), mList.end (), pObject) != mList.end ();
}

const bool ObjectList::IsSubList (const ObjectList& rList) const
{
#ifdef USE_LOGGER
	LOGMSG (NO_DEBUG, "ObjectList::IsSubList () - begin [%p]\n", this);
#endif

	STLObjectList_cit itLeft	= mList.begin ();
	STLObjectList_cit itLeftEnd	= mList.end ();

	STLObjectList_cit itRight	= rList.GetBegin ();
	STLObjectList_cit itRightEnd	= rList.GetEnd ();

	while (itLeft != itLeftEnd && itRight != itRightEnd)
	{
		if (*(*itLeft) == *(*itRight))
			itRight++;

		itLeft++;
	}

#ifdef USE_LOGGER
	LOGMSG (NO_DEBUG, "ObjectList::IsSubList () - [%s]\n", itRight == itRightEnd ? "yes":"no");
#endif

	return (itRight == itRightEnd);
}

/*
void ObjectList::Serialize (void **buffer, uint32 &size) const
{
#ifdef USE_LOGGER
	LOGMSG (MEDIUM_LEVEL, "ObjectList::Serialize () - [%p]\n", this);
#endif

	size = sizeof (uint64);

	*buffer = malloc(sizeof (uint64));
	char *offset = (char *) *buffer;

	uint64 num_items = mList.size ();

	memcpy (offset, &num_items, sizeof (uint64));

	STLObjectList_cit it;

	for (it = mList.begin (); it != mList.end (); it++)
	{
		void*	obj_buffer	= NULL	;
		uint32	obj_size	= 0	;

		(*it)->Serialize (&obj_buffer, obj_size);

		*buffer = realloc (*buffer, size + obj_size);

		offset = (char *) *buffer + size;
		memcpy (offset, obj_buffer, obj_size);
		size += obj_size;

		free (obj_buffer);
	}

#ifdef USE_LOGGER
	LOGMSG (NO_DEBUG, "ObjectList::Serialize () - size [%u]\n", size);
#endif
}
*/

void ObjectList::Serialize (ostream &stream) const
{
#ifdef USE_LOGGER
	LOGMSG (MEDIUM_LEVEL, "ObjectList::Serialize () - [%p]\n", this);
#endif

	size_t size = mList.size ();
	stream.write ((const char *) &size, sizeof (size_t));

	STLObjectList_cit it;

	for (it = mList.begin (); it != mList.end (); it++)
		(*it)->Serialize (stream);

#ifdef USE_LOGGER
	LOGMSG (NO_DEBUG, "ObjectList::Serialize () - end\n");
#endif
}

void ObjectList::Print () const
{
#ifdef USE_LOGGER
	LOGMSG (MAX_LEVEL, "ObjectList::Print () - begin [%p]\n", this);
#endif

	STLObjectList_cit it;

	for (it = mList.begin (); it != mList.end (); it++)
		(*it)->Print ();

#ifdef USE_LOGGER
	LOGMSG (MAX_LEVEL, "ObjectList::Print () - end [%p]\n", this);
#endif
}

const uint64 ObjectList::GetMemSize () const
{
#ifdef USE_LOGGER
	LOGMSG (MEDIUM_LEVEL, "ObjectList::GetMemSize () - [%p]\n", this);
#endif

	uint64 size = 0;

	size += sizeof (STLObjectList);
	size += sizeof (uint64);

	STLObjectList_cit it;

	for (it = mList.begin (); it != mList.end (); it++)
		size += (*it)->GetMemSize ();

#ifdef USE_LOGGER
	LOGMSG (MEDIUM_LEVEL, "ObjectList::GetMemSize () - [%u]\n", size);
#endif

	return size;
}
