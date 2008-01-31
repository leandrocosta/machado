#include "ObjectStrHash.h"
#include "Logger.h"


ObjectStrHash::ObjectStrHash (const uint64 &max_size)
{
	LOGMSG (MEDIUM_LEVEL, "ObjectStrHash::ObjectStrHash () - [%p]\n", this);

	mMaxSize = max_size;
}

ObjectStrHash::~ObjectStrHash ()
{
	LOGMSG (MEDIUM_LEVEL, "ObjectStrHash::~ObjectStrHash () - [%p]\n", this);

	STLObjectStrHash_cit it;

	for (it = mHash.begin (); it != mHash.end (); it++)
		delete it->second;

	mHash.clear ();
}

void ObjectStrHash::SetMaxSize (const uint64 &max_size)
{
	mMaxSize = max_size;
}

void ObjectStrHash::Add (const string &key, Object *pObject)
{
	bool bKeyExists = Find (key);

	if (bKeyExists)
		delete mHash [key];

	if (bKeyExists || mHash.size () < mMaxSize)
		mHash[key] = pObject;
}

Object* ObjectStrHash::Get (const string &key) const
{
	Object *pObject = NULL;

	if (Find (key))
		pObject = mHash.find (key)->second;

	return pObject;
}

void ObjectStrHash::Remove (const string &key)
{
	mHash.erase (key);
}

void ObjectStrHash::RemoveAll ()
{
	mHash.clear ();
}

void ObjectStrHash::DeleteAll ()
{
	LOGMSG (MEDIUM_LEVEL, "ObjectStrHash::DeleteAll () - begin [%p]\n", this);

	STLObjectStrHash_it it;

	for (it = mHash.begin (); it != mHash.end (); it++)
		delete it->second;

	mHash.clear ();

	LOGMSG (MEDIUM_LEVEL, "ObjectStrHash::DeleteAll () - end [%p]\n", this);
}

const bool ObjectStrHash::Find (const string &key) const
{
	return (mHash.find (key) != mHash.end ());
}

const uint64 ObjectStrHash::GetSize () const
{
	return mHash.size ();
}

const uint64 ObjectStrHash::GetMemSize () const
{
	uint64 size = 0;

	size += sizeof (STLObjectStrHash);
	size += sizeof (uint64);
	size += mHash.size () * sizeof (Object *);

	STLObjectStrHash_cit it;

	for (it = mHash.begin (); it != mHash.end (); it++)
		size += (*it).first.size () + 1;

	return size;
}
