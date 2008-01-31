#ifndef _LOCKABLE_EXCEPTION_H_
#define _LOCKABLE_EXCEPTION_H_


#include "Exception.h"


class LockableException : public Exception
{
	public:
			LockableException	(const string &message = "Generic Lockable Exception")	;
		virtual	~LockableException	()							;
};


#endif
