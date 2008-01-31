#ifndef _OBJECT_EXCEPTION_H_
#define _OBJECT_EXCEPTION_H_


#include "Exception.h"


class ObjectException : public Exception
{
	public:
			ObjectException		(const string &message = "Generic Object Exception")	;
		virtual	~ObjectException	()							;
};


#endif
