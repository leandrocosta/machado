#include "Converter.h"

#include <sstream>
using std::stringstream;


const string Converter::UInt32ToStr (const uint32 &uint32)
{
	stringstream str;
	str << uint32;
	return str.str ();
}
