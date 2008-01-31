#include "LockableHash.h"
#include "Logger.h"


LockableHash::LockableHash (const uint64 &max_size) : Hash (max_size), Lockable ()
{

}

LockableHash::~LockableHash ()
{

}

void LockableHash::Add (const string &key, void *p)
{
	Lock ();
	Hash::Add (key, p);
	Unlock ();
}

void* LockableHash::GetValue (const string &key)
{
	void *p = NULL;

	Lock ();
	p = Hash::GetValue (key);
	Unlock ();

	return p;
}

void LockableHash::Remove (const string &key)
{
	Lock ();
	Hash::Remove (key);
	Unlock ();
}

const uint64 LockableHash::GetSize ()
{
	LOGMSG (MEDIUM_LEVEL, "LockableHash::GetSize () - [%p]\n", this);

	uint64 size = 0;

	Lock ();
	size = Hash::GetSize ();
	Unlock ();

	return size;
}

const bool LockableHash::Find (const string &key)
{
	bool b = false;

	Lock ();
	b = Hash::Find (key);
	Unlock ();

	return b;
}

const uint64 LockableHash::GetMemSize () const
{
	uint64 size = 0;
	
	size += Hash::GetMemSize ();
	size += Lockable::GetMemSize ();

	LOGMSG (MEDIUM_LEVEL, "LockableHash::GetMemSize () - [%u]\n", size);

	return size;
}
