#ifndef _OBJECT_STR_HASH_H_
#define _OBJECT_STR_HASH_H_


#include "Object.h"

#include "stl_hash_include.h"
#include "types.h"


class ObjectUIntHash : public Object
{
	public:
			ObjectUIntHash	(const uint32 &max_size = UINT_MAX)	;
		virtual	~ObjectUIntHash	()					;

	protected:
		typedef hash_map<uint32, Object *>		STLObjectUIntHash	;
		typedef	STLObjectUIntHash::iterator		STLObjectUIntHash_it	;
		typedef	STLObjectUIntHash::const_iterator	STLObjectUIntHash_cit	;

	public:
		void	SetMaxSize	(const uint32 &max_size)	;

	public:
		void	Add	(const uint32 &key, Object *pObject)	;

	public:
		virtual	Object*	Get	(const uint32 &key)	const	;

	public:
		void	Remove		(const uint32 &key)	;
		void	RemoveAll	()			;
		void	DeleteAll	()			;

	public:
		const	bool	Find	(const uint32 &key)	const	;

	public:
		const	uint32	GetSize	()	const	;

	public:
		virtual	const	uint64	GetMemSize	()	const	;

	private:
		STLObjectUIntHash	mHash		;
		uint32			mMaxSize	;
};


#endif
