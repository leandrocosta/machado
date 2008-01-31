#ifndef _HASH_H_
#define _HASH_H_


#include <string>
using std::string;

#include "stl_hash_include.h"
#include "types.h"

typedef hash_map<string, void *>	STLHash;
typedef	STLHash::iterator		STLHash_it;
typedef	STLHash::const_iterator		STLHash_cit;


class Hash
{
	public:
			Hash	(const uint64 &max_size = ULONG_MAX)	;
		virtual	~Hash	()					;

	protected:
			const	STLHash&	GetHash		()				const	;
		virtual		void		Add		(const string &key, void *p)	;
		virtual		void*		GetValue	(const string &key)		;
		virtual		void		Remove		(const string &key)		;
		virtual		void		RemoveAll	()				;

	public:
			const	uint64	GetSize		()			const	;
		virtual	const	bool	Find		(const string &key)	const	;
		virtual	const	uint64	GetMemSize	()			const	;

	private:
		STLHash	mHash		;
		uint64	mMaxSize	;
};


#endif
