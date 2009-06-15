#ifndef _OBJECT_STR_HASH_H_
#define _OBJECT_STR_HASH_H_


#include "Object.h"

#include <string>
using std::string;

#include "stl_hash_include.h"
#include "types.h"


class ObjectStrHash : public Object
{
	public:
			ObjectStrHash	(const uint64 &max_size = ULONG_MAX)	;
		virtual	~ObjectStrHash	()					;

	protected:
		typedef hash_map<string, Object *>		STLObjectStrHash	;
		typedef	STLObjectStrHash::iterator		STLObjectStrHash_it	;
		typedef	STLObjectStrHash::const_iterator	STLObjectStrHash_cit	;

	public:
		void	SetMaxSize	(const uint64 &max_size)	;

	public:
		void	Add	(const string &key, Object *pObject)	;

	public:
		virtual	Object*	Get	(const string &key)	const	;

	public:
		void	Remove		(const string &key)	;
		void	RemoveAll	()			;
		void	DeleteAll	()			;

	public:
		const	bool	Find	(const string &key)	const	;

	public:
		const	uint64	GetSize	()	const	;

	public:
#ifdef USE_MEM_SIZE
		virtual	const	uint64	GetMemSize	()	const	;
#endif

	private:
		STLObjectStrHash	mHash		;
		uint64			mMaxSize	;
};


#endif
