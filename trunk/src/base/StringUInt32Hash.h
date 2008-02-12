#ifndef _STRING_UINT32_HASH_H
#define _STRING_UINT32_HASH_H


#include "Object.h"
#include "stl_hash_include.h"
#include "types.h"

#include <string>
using std::string;


class StringUInt32Hash : public Object
{
	public:
			StringUInt32Hash	(const uint32 &max_size = UINT_MAX)	;
		virtual ~StringUInt32Hash	()					;

	private:
		typedef hash_map<string, uint32>		STLStringUInt32Hash	;
		typedef	STLStringUInt32Hash::iterator		STLStringUInt32Hash_it	;
		typedef	STLStringUInt32Hash::const_iterator	STLStringUInt32Hash_cit	;

	public:
			void	Set	(const string &key, const uint32 &value)		;
		const	uint32	Get	(const string &key)				const	;
		const	bool	Find	(const string &key)				const	;
			void	Clear	()							;

	private:
		STLStringUInt32Hash	mHash		;
		uint32			mMaxSize	;
};


#endif
