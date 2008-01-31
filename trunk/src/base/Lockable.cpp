#include <errno.h>
#include "Lockable.h"
#include "LockableException.h"
#include <typeinfo>
#include "Logger.h"


Lockable::Lockable (const MutexType &type) : Object ()
{
	pthread_mutexattr_init (&m_mutex_attr)	;

	switch (type)
	{
		case Lockable::READ_WRITE:
			break;
		case Lockable::RECURSIVE:
			pthread_mutexattr_settype (&m_mutex_attr,	PTHREAD_MUTEX_RECURSIVE)	;
			break;
		case Lockable::DEFAULT:
		default:
			pthread_mutexattr_settype (&m_mutex_attr,	PTHREAD_MUTEX_DEFAULT)		;
	}

	switch (type)
	{
		case Lockable::READ_WRITE:
			pthread_rwlock_init	(&m_rwlock,	NULL)		;
			break;
		case Lockable::DEFAULT:
		case Lockable::RECURSIVE:
			pthread_mutex_init	(&m_mutex,	&m_mutex_attr)	;
	}

	mMutexType = type;
}

Lockable::~Lockable ()
{
//	LOGMSG (MAX_LEVEL, "Lockable::~Lockable () - [%p]\n", this);

	switch (mMutexType)
	{
		case Lockable::READ_WRITE:
			pthread_rwlock_destroy		(&m_rwlock)	;
			break;
		case Lockable::DEFAULT:
		case Lockable::RECURSIVE:
		default:
			pthread_mutex_destroy		(&m_mutex)	;
			pthread_mutexattr_destroy	(&m_mutex_attr)	;
	}
}

void Lockable::Lock () const
{
//	fprintf (stderr, "Lockable::Lock () for class %s - begin [%p]\n", string (typeid (*this).name ()).c_str (), this);
//	fprintf (stderr, "Lockable::Lock () - begin [%p]\n", this);
//	fflush (stderr);

	if (mMutexType != Lockable::READ_WRITE)
		pthread_mutex_lock (&m_mutex);
	else
		throw LockableException ("READ/WRITE Lock - can't execute Lock () for class " + string (typeid (*this).name ()));

//	fprintf (stderr, "Lockable::Lock () for class %s - end [%p]\n", string (typeid (*this).name ()).c_str (), this);
//	fprintf (stderr, "Lockable::Lock () - end [%p]\n", this);
}

void Lockable::Unlock () const
{
//	fprintf (stderr, "Lockable::Unlock () for class %s - begin [%p]\n", string (typeid (*this).name ()).c_str (), this);
//	fprintf (stderr, "Lockable::Unlock () - begin [%p]\n", this);
//	fflush (stderr);

	if (mMutexType != Lockable::READ_WRITE)
		pthread_mutex_unlock (&m_mutex);
	else
		throw LockableException ("READ/WRITE Lock - can't execute Unlock () for class " + string (typeid (*this).name ()));

//	fprintf (stderr, "Lockable::Unlock () for class %s - end [%p]\n", string (typeid (*this).name ()).c_str (), this);
//	fprintf (stderr, "Lockable::Unlock () - end [%p]\n", this);
}

bool Lockable::TryLock () const
{
	bool ret = true;

	if (mMutexType != Lockable::READ_WRITE)
	{
		if (pthread_mutex_trylock (&m_mutex) == EBUSY)
			ret = false;
	}
	else
		throw LockableException ("READ/WRITE Lock - can't execute TryLock () for class " + string (typeid (*this).name ()));

	return ret;
}

void Lockable::ReadLock () const
{
	LOGMSG (MEDIUM_LEVEL, "Lockable::ReadLock () - begin [%p]\n", this);

	if (mMutexType == Lockable::READ_WRITE)
		pthread_rwlock_rdlock(&m_rwlock);
	else
		throw LockableException ("CLASSIC Lock - can't execute ReadLock () for class " + string (typeid (*this).name ()));

	LOGMSG (MEDIUM_LEVEL, "Lockable::ReadLock () - end [%p]\n", this);
}

void Lockable::ReadUnlock () const
{
	LOGMSG (MEDIUM_LEVEL, "Lockable::ReadUnlock () - begin [%p]\n", this);

	if (mMutexType == Lockable::READ_WRITE)
		pthread_rwlock_unlock(&m_rwlock);
	else
		throw LockableException ("CLASSIC Lock - can't execute ReadUnlock () for class " + string (typeid (*this).name ()));

	LOGMSG (MEDIUM_LEVEL, "Lockable::ReadUnlock () - end [%p]\n", this);
}

bool Lockable::ReadTryLock () const
{
	bool ret = true;

	if (mMutexType == Lockable::READ_WRITE)
	{
		if (pthread_rwlock_tryrdlock(&m_rwlock) == EBUSY)
			ret = false;
	}
	else
		throw LockableException ("CLASSIC Lock - can't execute ReadTryLock () for class " + string (typeid (*this).name ()));

	return ret;
}

void Lockable::WriteLock () const
{
	LOGMSG (MEDIUM_LEVEL, "Lockable::WriteLock () - begin [%p]\n", this);

	if (mMutexType == Lockable::READ_WRITE)
		pthread_rwlock_wrlock(&m_rwlock);
	else
		throw LockableException ("CLASSIC Lock - can't execute WriteLock () for class " + string (typeid (*this).name ()));

	LOGMSG (MEDIUM_LEVEL, "Lockable::WriteLock () - end [%p]\n", this);
}

void Lockable::WriteUnlock () const
{
	if (mMutexType == Lockable::READ_WRITE)
		pthread_rwlock_unlock(&m_rwlock);
	else
		throw LockableException ("CLASSIC Lock - can't execute WriteUnlock () for class " + string (typeid (*this).name ()));
}

bool Lockable::WriteTryLock () const
{
	bool ret = true;

	if (mMutexType == Lockable::READ_WRITE)
	{
		if (pthread_rwlock_trywrlock(&m_rwlock) == EBUSY)
			ret = false;
	}
	else
		throw LockableException ("CLASSIC Lock - can't execute WriteTryLock () for class " + string (typeid (*this).name ()));

	return ret;
}

const uint64 Lockable::GetMemSize () const
{
	LOGMSG (MEDIUM_LEVEL, "Lockable::GetMemSize () - [%p]\n", this);

	uint64 size = 0;
	
	size += sizeof (pthread_mutexattr_t);
	size += sizeof (pthread_mutex_t);
	size += sizeof (pthread_rwlock_t);
	size += sizeof (MutexType);

	LOGMSG (MEDIUM_LEVEL, "Lockable::GetMemSize () - [%u]\n", size);

	return size;
}
