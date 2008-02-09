#ifndef _DATA_BASE_EXCEPTION_H_
#define _DATA_BASE_EXCEPTION_H_


#include "base/ObjectException.h"


class DataBaseException : public ObjectException
{
	public:
			DataBaseException	(const string &message = "Generic DataBase Exception")	;
		virtual	~DataBaseException	()							;
};


#endif
