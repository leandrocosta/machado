#include "Object.h"
#include "ObjectException.h"
#include <typeinfo>


Object::Object ()
{

}

Object::~Object ()
{

}

const bool Object::operator< (const Object& rRight) const
{
	throw (ObjectException ("Operator less (<) not defined for class " + string (typeid (*this).name ())));

	return this < &rRight;
}

const bool Object::operator> (const Object& rRight) const
{
	throw (ObjectException ("Operator greater (>) not defined for class " + string (typeid (*this).name ())));

	return this > &rRight;
}

const bool Object::operator<= (const Object& rRight) const
{
	throw (ObjectException ("Operator less (<=) not defined for class " + string (typeid (*this).name ())));

	return this <= &rRight;
}

const bool Object::operator>= (const Object& rRight) const
{
	throw (ObjectException ("Operator greater (>=) not defined for class " + string (typeid (*this).name ())));

	return this >= &rRight;
}

const bool Object::operator== (const Object& rRight) const
{
	throw (ObjectException ("Operator equal (==) not defined for class " + string (typeid (*this).name ())));

	return this == &rRight;
}

const bool Object::operator!= (const Object& rRight) const
{
	throw (ObjectException ("Operator equal (!=) not defined for class " + string (typeid (*this).name ())));

	return this != &rRight;
}

#ifdef USE_SERIALIZE
/*
void Object::Serialize (void **buffer, uint32 &size) const
{
	throw (ObjectException ("Method Serialize () not defined for class " + string (typeid (*this).name ())));
}

void Object::Unserialize (const void *buffer, uint32 &size)
{
	throw (ObjectException ("Method Unserialize () not defined for class " + string (typeid (*this).name ())));
}
*/

void Object::Serialize (ostream &stream) const
{
	throw (ObjectException ("Method Serialize () not defined for class " + string (typeid (*this).name ())));
}

void Object::Unserialize (istream &stream)
{
	throw (ObjectException ("Method Unserialize () not defined for class " + string (typeid (*this).name ())));
}
#endif

Object* Object::Clone () const
{
	throw (ObjectException ("Method Clone () not defined for class " + string (typeid (*this).name ())));
}

void Object::Print () const
{
	throw (ObjectException ("Method Print () not defined for class " + string (typeid (*this).name ())));
}

#ifdef USE_MEM_SIZE
const uint64 Object::GetMemSize () const
{
	throw (ObjectException ("Method GetMemSize () not defined for class " + string (typeid (*this).name ())));

	return 0;
}
#endif
