#ifndef _LOCKABLE_HASH_H_
#define _LOCKABLE_HASH_H_

#include "Hash.h"
#include "Lockable.h"
#include "types.h"


class LockableHash : public Hash, protected Lockable
{
	public:
			LockableHash	(const uint64 &max_size = ULONG_MAX)	;
		virtual	~LockableHash	()					;

	protected:
			void	Add		(const string &key, void *p)	;
		virtual	void*	GetValue	(const string &key)		;
			void	Remove		(const string &key)		;

	public:
			const	uint64	GetSize		()				;
			const	bool	Find		(const string &key)		;
		virtual	const	uint64	GetMemSize	()			const	;
};


#endif
