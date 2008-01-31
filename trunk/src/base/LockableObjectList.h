#ifndef _LOCKABLE_OBJECT_LIST_H_
#define _LOCKABLE_OBJECT_LIST_H_


#include "ObjectList.h"
#include "Lockable.h"


class LockableObjectList : public ObjectList, protected Lockable
{
	public:
			LockableObjectList	(const uint64 &max_size = ULONG_MAX)	;
		virtual	~LockableObjectList	()					;

	public:
				void	PushFront	(Object *pObject)		;
				void	PushBack	(Object *pObject)		;

		virtual		Object*	PopFront	()				;
		virtual		Object*	PopBack		()				;
		virtual		Object* PopAt		(const uint64 &index)		;

		virtual		Object*	GetFront	()			const	;
		virtual		Object*	GetBack		()			const	;
		virtual		Object*	GetAt		(const uint64 &index)	const	;

				void	Sort		()				;
				void	ReverseSort	()				;
				void	MoveToEnd	(Object *pObject)		;
				void	Remove		(Object *pObject)		;
				void	RemoveAll	()				;
			const	uint64	GetSize		()			const	;

//	public:
//		virtual	void	Serialize	(void **buffer, uint32 &size)	const	;

	public:
		virtual	void	Serialize	(ostream &stream)	const	;

	public:
		virtual	void	Print	()	const	;

	public:
		virtual	const	uint64	GetMemSize	()	const	;
};


#endif
