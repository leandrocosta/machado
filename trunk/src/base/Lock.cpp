#include "Lock.h"


void Lock::Get ()
{
	Lockable::Lock ();
}

void Lock::Release ()
{
	Lockable::Unlock ();
}
