#include "UInt32Float32Hash.h"


UInt32Float32Hash::UInt32Float32Hash (const uint32 &max_size) : Object ()
{
	mMaxSize = max_size;
}

UInt32Float32Hash::~UInt32Float32Hash ()
{

}

void UInt32Float32Hash::Set (const uint32 &key, const float32 &value)
{
	if (mHash.size () < mMaxSize || mHash.find (key) != mHash.end ())
		mHash [key] = value;
}

const float32 UInt32Float32Hash::Get (const uint32 &key) const
{
	float32 value = 0;

	STLUInt32Float32Hash_cit it = mHash.find (key);

	if (it != mHash.end ())
		value = it->second;

	return value;
}

const bool UInt32Float32Hash::Find (const uint32 &key) const
{
	bool bRet = false;

	if (mHash.find (key) != mHash.end ())
		bRet = true;

	return bRet;
}
