#include "ObjectUIntHash.h"
#include "Logger.h"


ObjectUIntHash::ObjectUIntHash (const uint32 &max_size)
{
#ifdef USE_LOGGER
	LOGMSG (MEDIUM_LEVEL, "ObjectUIntHash::ObjectUIntHash () - [%p]\n", this);
#endif

	mMaxSize = max_size;
}

ObjectUIntHash::~ObjectUIntHash ()
{
#ifdef USE_LOGGER
	LOGMSG (MEDIUM_LEVEL, "ObjectUIntHash::~ObjectUIntHash () - [%p]\n", this);
#endif

	STLObjectUIntHash_cit it;

	for (it = mHash.begin (); it != mHash.end (); ++it)
		delete it->second;

	mHash.clear ();
}

void ObjectUIntHash::SetMaxSize (const uint32 &max_size)
{
	mMaxSize = max_size;
}

void ObjectUIntHash::Add (const uint32 &key, Object *pObject)
{
	bool bKeyExists = Find (key);

	if (bKeyExists)
		delete mHash [key];

	if (bKeyExists || mHash.size () < mMaxSize)
		mHash[key] = pObject;
}

Object* ObjectUIntHash::Get (const uint32 &key) const
{
	Object *pObject = NULL;

	if (Find (key))
		pObject = mHash.find (key)->second;

	return pObject;
}

void ObjectUIntHash::Remove (const uint32 &key)
{
	mHash.erase (key);
}

void ObjectUIntHash::RemoveAll ()
{
	mHash.clear ();
}

void ObjectUIntHash::DeleteAll ()
{
#ifdef USE_LOGGER
	LOGMSG (MEDIUM_LEVEL, "ObjectUIntHash::DeleteAll () - begin [%p]\n", this);
#endif

	STLObjectUIntHash_it it;

	for (it = mHash.begin (); it != mHash.end (); ++it)
		delete it->second;

	mHash.clear ();

#ifdef USE_LOGGER
	LOGMSG (MEDIUM_LEVEL, "ObjectUIntHash::DeleteAll () - end [%p]\n", this);
#endif
}

const bool ObjectUIntHash::Find (const uint32 &key) const
{
	return (mHash.find (key) != mHash.end ());
}

const uint32 ObjectUIntHash::GetSize () const
{
	return mHash.size ();
}

#ifdef USE_MEM_SIZE
const uint64 ObjectUIntHash::GetMemSize () const
{
	uint64 size = 0;

	size += sizeof (STLObjectUIntHash);
	size += sizeof (uint32);
	size += mHash.size () * (sizeof (uint32) + sizeof (Object *));

	return size;
}
#endif
