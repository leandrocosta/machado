#ifndef _LOCK_H_
#define _LOCK_H_


#include "Lockable.h"


class Lock : private Lockable
{
	public:
		void	Get	()	;
		void	Release	()	;
};


#endif
