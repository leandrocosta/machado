#ifndef _RAND_H_
#define _RAND_H_

#include "types.h"


class Rand
{
	public:
		static		void	SetSeed		()					;
		static	const	uint32	GetRandomNumber	(const uint32 &min, const uint32 &max)	;
};


#endif
