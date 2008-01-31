#include "ObjectFactory.h"
#include "ObjectException.h"
#include "Logger.h"
#include <typeinfo>


ObjectFactory*	ObjectFactory::mspInstance	= NULL	;
uint8		ObjectFactory::msTypeID		= 0	;
Lock		ObjectFactory::msLock			;


ObjectFactory* ObjectFactory::GetInstance ()
{
	if (mspInstance == NULL)
		mspInstance = new ObjectFactory ();

	return mspInstance;
}

void ObjectFactory::Finalize ()
{
	if (mspInstance)
		delete mspInstance;

	mspInstance = NULL;
}

ObjectFactory::ObjectFactory () : Object ()
{
	LOGMSG (NO_DEBUG, "ObjectFactory::ObjectFactory () - p [%p]\n", this);

	mTypeID = GetSeqTypeID ();
}

ObjectFactory::~ObjectFactory ()
{

}

const uint8 ObjectFactory::GetSeqTypeID ()
{
	uint8 typeID;

	msLock.Get ();
	typeID = msTypeID++;
	msLock.Release ();

	LOGMSG (NO_DEBUG, "ObjectFactory::GetSeqTypeID () - return type id [%u]\n", typeID);

	return typeID;
}

const uint8& ObjectFactory::GetTypeID () const
{
	LOGMSG (MAX_LEVEL, "ObjectFactory::GetTypeID () - type id [%u]\n", mTypeID);

	return mTypeID;
}
Object* ObjectFactory::CreateNew () const
{
	throw (ObjectException ("Method CreateNew () not defined for class " + string (typeid (*this).name ())));

	return NULL;
}
