#include "Exception.h"


Exception::Exception (const string& message)
{
	mMessage = message;
}

Exception::~Exception ()
{

}

const string& Exception::GetMessage () const
{
	return mMessage;
}
