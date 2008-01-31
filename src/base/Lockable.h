#ifndef _LOCKABLE_H_
#define _LOCKABLE_H_


#include "Object.h"
#include <pthread.h>


class Lockable : public Object
{
	public:
		typedef enum MutexTypeT
		{
			DEFAULT		= 1,
			RECURSIVE	= 2,
			READ_WRITE	= 3
		} MutexType;

	protected:
			Lockable	(const MutexType &type = DEFAULT)	;
		virtual	~Lockable	()					;

	protected:
		void	Lock		()	const	;
		void	Unlock		()	const	;
		bool	TryLock		()	const	;

		void	ReadLock	()	const	;
		void	ReadUnlock	()	const	;
		bool	ReadTryLock	()	const	;

		void	WriteLock	()	const	;
		void	WriteUnlock	()	const	;
		bool	WriteTryLock	()	const	;

	public:
		virtual	const	uint64	GetMemSize	()	const	;

	private:
		pthread_mutexattr_t	m_mutex_attr	;
		MutexType		mMutexType	;

	private:
		mutable	pthread_mutex_t		m_mutex		;
		mutable	pthread_rwlock_t	m_rwlock	;
};


#endif
