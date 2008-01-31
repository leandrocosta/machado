#ifndef _STL_HASH_INCLUDE_H_
#define _STL_HASH_INCLUDE_H_

#if (__GNUC__ < 3)
#include <hash_map>
#else
#include <ext/hash_map>
using __gnu_cxx::hash;
using __gnu_cxx::hashtable;
using __gnu_cxx::hash_multimap;
using __gnu_cxx::hash_map;

namespace __gnu_cxx
{
	template<> struct hash< std::string >
	{
		size_t operator()( const std::string& s ) const
		{
			return hash< const char* >()( s.c_str() );
		}
	};
}
#endif

#endif
