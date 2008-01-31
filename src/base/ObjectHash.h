#ifndef _OBJECT_HASH_H_
#define _OBJECT_HASH_H_


#include "Object.h"

#include <string>
using std::string;

#include "stl_hash_include.h"


class ObjectHash : public Object
{
	public:
			ObjectHash	(const uint64 &max_size = ULONG_MAX)	;
		virtual	~ObjectHash	()					;

	protected:
		typedef hash_map<string, Object*>	STLObjectHash		;
		typedef	STLObjectHash::iterator		STLObjectHash_it	;
		typedef	STLObjectHash::const_iterator	STLObjectHash_cit	;

	protected:
			STLObjectHash_it	GetBegin	()		;
		const	STLObjectHash_cit	GetBegin	()	const	;
			STLObjectHash_it	GetEnd		()		;
		const	STLObjectHash_cit	GetEnd		()	const	;

	public:
		const	uint64	GetSize	()	const	;

	public:
		void	Add		(const string &key, Object *pObject)	;
		void	Remove		(const string &key)			;
		void	RemoveAll	()					;
		void	DeleteAll	()					;

	public:
		virtual	Object*	Get	(const string &key)	;

	public:
		virtual	const	bool	Find		(const string &key)	const	;
		virtual	const	uint64	GetMemSize	()			const	;

	private:
		STLObjectHash	mHash		;
		uint64		mMaxSize	;
};


#endif
