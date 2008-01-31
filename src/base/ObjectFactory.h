#ifndef _OBJECT_FACTORY_H_
#define _OBJECT_FACTORY_H_


#include "Object.h"
#include "Lock.h"


class ObjectFactory : public Object
{
	// Singleton methods
	public:
		static	ObjectFactory*	GetInstance	()	;
		static	void		Finalize	()	;

	// Constructor
	protected:
		ObjectFactory	()	;

	// Destructor
	public:
		virtual	~ObjectFactory	()	;

	protected:
		static	const	uint8	GetSeqTypeID	()	;

	public:
			const	uint8&	GetTypeID	()	const	;
		virtual		Object*	CreateNew	()	const	;

	private:
		static	ObjectFactory*	mspInstance	;

	private:
		static	uint8	msTypeID	;
		static	Lock	msLock		;

	private:
		uint8	mTypeID	;
};

#endif
