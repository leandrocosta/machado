#include "LockableObjectList.h"
#include "Logger.h"


LockableObjectList::LockableObjectList (const uint64 &max_size) : ObjectList (max_size), Lockable (Lockable::READ_WRITE)
{
	LOGMSG (MEDIUM_LEVEL, "LockableObjectList::LockableObjectList () - [%p]\n", this);
}

LockableObjectList::~LockableObjectList ()
{
	LOGMSG (MEDIUM_LEVEL, "LockableObjectList::~LockableObjectList () - [%p]\n", this);
}

void LockableObjectList::PushFront (Object *pObject)
{
	WriteLock ();
	ObjectList::PushFront (pObject);
	WriteUnlock ();
}

void LockableObjectList::PushBack (Object *pObject)
{
	LOGMSG (MEDIUM_LEVEL, "LockableObjectList::PushBack () - [%p]\n", this);

	WriteLock ();
	ObjectList::PushBack (pObject);
	WriteUnlock ();

	LOGMSG (MEDIUM_LEVEL, "LockableObjectList::PushBack () - end\n");
}

Object* LockableObjectList::PopFront ()
{
	Object *pObject = NULL;

	WriteLock ();
	pObject = ObjectList::PopFront ();
	WriteUnlock ();

	return pObject;
}

Object* LockableObjectList::PopBack ()
{
	Object *pObject = NULL;

	WriteLock ();
	pObject = ObjectList::PopBack ();
	WriteUnlock ();

	return pObject;
}

Object* LockableObjectList::PopAt (const uint64 &index)
{
	Object *pObject = NULL;

	WriteLock ();
	pObject = ObjectList::PopAt (index);
	WriteUnlock ();

	return pObject;
}

Object* LockableObjectList::GetFront () const
{
	Object *pObject = NULL;

	ReadLock ();
	pObject = ObjectList::GetFront ();
	ReadUnlock ();

	return pObject;
}


Object* LockableObjectList::GetBack () const
{
	Object *pObject = NULL;

	ReadLock ();
	pObject = ObjectList::GetBack ();
	ReadUnlock ();

	return pObject;
}


Object* LockableObjectList::GetAt (const uint64 &index) const
{
	Object *pObject = NULL;

	ReadLock ();
	pObject = ObjectList::GetAt (index);
	ReadUnlock ();

	return pObject;
}

void LockableObjectList::Sort ()
{
	WriteLock ();
	ObjectList::Sort ();
	WriteUnlock ();
}

void LockableObjectList::ReverseSort ()
{
	WriteLock ();
	ObjectList::ReverseSort ();
	WriteUnlock ();
}

void LockableObjectList::MoveToEnd (Object *pObject)
{
	LOGMSG (MEDIUM_LEVEL, "LockableObjectList::MoveToEnd () - begin [%p]\n", this);

	WriteLock ();
	ObjectList::MoveToEnd (pObject);
	WriteUnlock ();

	LOGMSG (MEDIUM_LEVEL, "LockableObjectList::MoveToEnd () - end [%p]\n", this);
}

void LockableObjectList::Remove (Object *pObject)
{
	WriteLock ();
	ObjectList::Remove (pObject);
	WriteUnlock ();
}

void LockableObjectList::RemoveAll ()
{
	WriteLock ();
	ObjectList::RemoveAll ();
	WriteUnlock ();
}

const uint64 LockableObjectList::GetSize () const
{
	LOGMSG (MEDIUM_LEVEL, "LockableObjectList::GetSize () - [%p]\n", this);

	uint64 size = 0;

	ReadLock ();
	size = ObjectList::GetSize ();
	ReadUnlock ();

	return size;
}

/*
void LockableObjectList::Serialize (void **buffer, uint32 &size) const
{
	LOGMSG (MEDIUM_LEVEL, "LockableObjectList::Serialize () - [%p]\n", this);

	ReadLock ();
	ObjectList::Serialize (buffer, size);
	ReadUnlock ();
}
*/

void LockableObjectList::Serialize (ostream &stream) const
{
	LOGMSG (MEDIUM_LEVEL, "LockableObjectList::Serialize () - [%p]\n", this);

	ReadLock ();
	ObjectList::Serialize (stream);
	ReadUnlock ();
}

void LockableObjectList::Print () const
{
	LOGMSG (MEDIUM_LEVEL, "LockableObjectList::Print () - begin [%p]\n", this);

	ReadLock ();
	ObjectList::Print ();
	ReadUnlock ();

	LOGMSG (MEDIUM_LEVEL, "LockableObjectList::Print () - end [%p]\n", this);
}

const uint64 LockableObjectList::GetMemSize () const
{
	LOGMSG (MEDIUM_LEVEL, "LockableObjectList::GetMemSize () - [%p]\n", this);

	uint64 size = 0;
	
	ReadLock ();
	size += ObjectList::GetMemSize ();
	size += Lockable::GetMemSize ();
	ReadUnlock ();

	LOGMSG (MEDIUM_LEVEL, "LockableObjectList::GetMemSize () - [%u]\n", size);

	return size;
}
