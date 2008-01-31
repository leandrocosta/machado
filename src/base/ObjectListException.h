#ifndef _OBJECT_LIST_EXCEPTION_H_
#define _OBJECT_LIST_EXCEPTION_H_


#include "Exception.h"


class ObjectListException : public Exception
{
	public:
			ObjectListException	(const string &message = "Generic Object List Exception")	;
		virtual	~ObjectListException	()								;
};


#endif
