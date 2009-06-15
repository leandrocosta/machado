#include "ObjectHash.h"
#include "Logger.h"


ObjectHash::ObjectHash (const uint64 &max_size)
{
	mMaxSize = max_size;
}

ObjectHash::~ObjectHash ()
{
	mHash.clear ();
}

ObjectHash::STLObjectHash_it ObjectHash::GetBegin ()
{
	return mHash.begin ();
}

const ObjectHash::STLObjectHash_cit ObjectHash::GetBegin () const
{
	return mHash.begin ();
}

ObjectHash::STLObjectHash_it ObjectHash::GetEnd ()
{
	return mHash.end ();
}

const ObjectHash::STLObjectHash_cit ObjectHash::GetEnd () const
{
	return mHash.end ();
}

void ObjectHash::Add (const string &key, Object *pObject)
{
	if (mHash.size () < mMaxSize)
		mHash[key] = pObject;
}

Object* ObjectHash::Get (const string &key)
{
	return mHash[key];
}

void ObjectHash::Remove (const string &key)
{
	mHash.erase (key);
}

void ObjectHash::RemoveAll ()
{
	mHash.clear ();
}

const uint64 ObjectHash::GetSize () const
{
	return mHash.size ();
}

const bool ObjectHash::Find (const string &key) const
{
#ifdef USE_LOGGER
	LOGMSG (HIGH_LEVEL, "ObjectHash::Find () - p [%p]\n", this);
#endif

	return (mHash.find (key) != mHash.end ());
}

#ifdef USE_MEM_SIZE
const uint64 ObjectHash::GetMemSize () const
{
	uint32 size = 0;

	size += sizeof (STLObjectHash);
	size += sizeof (uint64);
	size += mHash.size () * sizeof (void *);

	STLObjectHash_cit itEnd = mHash.end ();

	for (STLObjectHash_cit it = mHash.begin (); it != itEnd; ++it)
	{
		size += (*it).first.size () + 1;
		size += sizeof (void *);
	}

	return size;
}
#endif
