#include "StringUInt32Hash.h"


StringUInt32Hash::StringUInt32Hash (const uint32 &max_size) : Object ()
{
	mMaxSize = max_size;
}

StringUInt32Hash::~StringUInt32Hash ()
{

}

void StringUInt32Hash::Set (const string &key, const uint32 &value)
{
	if (mHash.size () < mMaxSize || mHash.find (key) != mHash.end ())
		mHash [key] = value;
}

const uint32 StringUInt32Hash::Get (const string &key) const
{
	uint32 value = 0;

	STLStringUInt32Hash_cit it = mHash.find (key);

	if (it != mHash.end ())
		value = it->second;

	return value;
}

const bool StringUInt32Hash::Find (const string &key) const
{
	bool bRet = false;

	if (mHash.find (key) != mHash.end ())
		bRet = true;

	return bRet;
}

void StringUInt32Hash::Clear ()
{
	mHash.clear ();
}
