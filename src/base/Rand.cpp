#include "Rand.h"
#include <stdlib.h>
#include <time.h>


void Rand::SetSeed ()
{
	srand (time (NULL));
}

const uint32 Rand::GetRandomNumber (const uint32 &min, const uint32 &max)
{
	return rand () % (max - min) + min;
}
