#include "ObjectStrHash.h"
#include "Logger.h"


ObjectStrHash::ObjectStrHash (const uint64 &max_size)
{
#ifdef USE_LOGGER
	LOGMSG (MEDIUM_LEVEL, "ObjectStrHash::ObjectStrHash () - [%p]\n", this);
#endif

	mMaxSize = max_size;
}

ObjectStrHash::~ObjectStrHash ()
{
#ifdef USE_LOGGER
	LOGMSG (MEDIUM_LEVEL, "ObjectStrHash::~ObjectStrHash () - [%p]\n", this);
#endif

	STLObjectStrHash_cit it;

	for (it = mHash.begin (); it != mHash.end (); ++it)
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
#ifdef USE_LOGGER
	LOGMSG (MEDIUM_LEVEL, "ObjectStrHash::DeleteAll () - begin [%p]\n", this);
#endif

	STLObjectStrHash_it it;

	for (it = mHash.begin (); it != mHash.end (); ++it)
		delete it->second;

	mHash.clear ();

#ifdef USE_LOGGER
	LOGMSG (MEDIUM_LEVEL, "ObjectStrHash::DeleteAll () - end [%p]\n", this);
#endif
}

const bool ObjectStrHash::Find (const string &key) const
{
	return (mHash.find (key) != mHash.end ());
}

const uint64 ObjectStrHash::GetSize () const
{
	return mHash.size ();
}

#ifdef USE_MEM_SIZE
const uint64 ObjectStrHash::GetMemSize () const
{
	uint64 size = 0;

	size += sizeof (STLObjectStrHash);
	size += sizeof (uint64);
	size += mHash.size () * sizeof (Object *);

	STLObjectStrHash_cit it;

	for (it = mHash.begin (); it != mHash.end (); ++it)
		size += (*it).first.size () + 1;

	return size;
}
#endif
