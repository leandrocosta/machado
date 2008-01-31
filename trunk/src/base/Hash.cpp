#include "Hash.h"


Hash::Hash (const uint64 &max_size)
{
	mMaxSize = max_size;
}

Hash::~Hash ()
{
	mHash.clear ();
}

const STLHash& Hash::GetHash () const
{
	return mHash;
}

void Hash::Add (const string &key, void *p)
{
	if (mHash.size () < mMaxSize)
		mHash[key] = p;
}

void* Hash::GetValue (const string &key)
{
	return mHash[key];
}

void Hash::Remove (const string &key)
{
	mHash.erase (key);
}

void Hash::RemoveAll ()
{
	mHash.clear ();
}

const uint64 Hash::GetSize () const
{
	return mHash.size ();
}

const bool Hash::Find (const string &key) const
{
	return (mHash.find (key) != mHash.end ());
}

const uint64 Hash::GetMemSize () const
{
	uint64 size = 0;

	size += sizeof (STLHash);
	size += sizeof (uint64);
	size += mHash.size () * sizeof (void *);

	STLHash_cit it;

	for (it = mHash.begin (); it != mHash.end (); it++)
	{
		size += (*it).first.size () + 1;
		size += sizeof (void *);
	}

	return size;
}
