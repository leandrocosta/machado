#ifndef _UINT32_FLOAT32_HASH_H
#define _UINT32_FLOAT32_HASH_H


#include "Object.h"
#include "stl_hash_include.h"
#include "types.h"


class UInt32Float32Hash : public Object
{
	public:
			UInt32Float32Hash	(const uint32 &max_size = UINT_MAX)	;
		virtual ~UInt32Float32Hash	()					;

	private:
		typedef hash_map<uint32, float32>		STLUInt32Float32Hash		;
		typedef	STLUInt32Float32Hash::iterator		STLUInt32Float32Hash_it		;
		typedef	STLUInt32Float32Hash::const_iterator	STLUInt32Float32Hash_cit	;

	public:
			void	Set	(const uint32 &key, const float32 &value)		;
		const	float32	Get	(const uint32 &key)				const	;
		const	bool	Find	(const uint32 &key)				const	;

	private:
		STLUInt32Float32Hash	mHash		;
		uint32			mMaxSize	;
};


#endif
