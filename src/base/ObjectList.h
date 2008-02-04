#ifndef _OBJECT_LIST_H_
#define _OBJECT_LIST_H_


#include "Object.h"

#include <deque>
using std::deque;


class ObjectList : public Object
{
	public:
			ObjectList	(const uint64 &max_size = ULONG_MAX)	;
		virtual	~ObjectList	()					;

	private:
		typedef struct CompareLess
		{
			const	bool	operator()	(const Object *pLeft, const Object *pRight)	const	;
		} LessComparer;

		typedef struct CompareLessEqual
		{
			const	bool	operator()	(const Object *pLeft, const Object *pRight)	const	;
		} LessEqualComparer;

		typedef struct CompareGreater
		{
			const	bool	operator()	(const Object *pLeft, const Object *pRight)	const	;
		} GreaterComparer;

		typedef struct CompareGreaterEqual
		{
			const	bool	operator()	(const Object *pLeft, const Object *pRight)	const	;
		} GreaterEqualComparer;

	public:
		static	const	bool	HasIntersectionByPtr	(const ObjectList *pLeft, const ObjectList *pRight)	;

	protected:
		typedef deque<Object *>			STLObjectList		;
		typedef STLObjectList::iterator		STLObjectList_it	;
		typedef STLObjectList::const_iterator	STLObjectList_cit	;

	public:
			STLObjectList_it	GetBegin	()		;
		const	STLObjectList_cit	GetBegin	()	const	;
			STLObjectList_it	GetEnd		()		;
		const	STLObjectList_cit	GetEnd		()	const	;

	public:
		void	SetMaxSize	(const uint64 &max_size)	;

	public:
		void	PushFront	(Object *pObject)	;
		void	PushBack	(Object *pObject)	;

	public:
		virtual	Object*	PopFront	()				;
		virtual	Object*	PopBack		()				;
		virtual	Object* PopAt		(const uint64 &index)		;

		virtual	Object*	GetFront	()			const	;
		virtual	Object*	GetBack		()			const	;
		virtual	Object*	GetAt		(const uint64 &index)	const	;

	public:
		void	Sort		()			;
		void	ReverseSort	()			;
		void	MoveToEnd	(Object *pObject)	;
		void	Remove		(Object *pObject)	;
		void	RemoveAll	()			;
		void	DeleteAll	()			;

	public:
		STLObjectList_it	Erase	(STLObjectList_it it)	;

	public:
		const	bool	Find		(const Object *pObject) const	;
		const	bool	FindByPtr	(const Object *pObject)	const	;

	public:
		virtual	const	bool	IsSubList (const ObjectList& rList)	const	;

	public:
		const	uint64	GetSize	()	const	;

	public:
		virtual	void	Serialize	(ostream &stream)	const	;

	public:
		virtual	void	Print	()	const	;

	public:
		virtual	const	uint64	GetMemSize	()	const	;

	private:
		static	LessComparer		msLessComparer		;
		static	LessEqualComparer	msLessEqualComparer	;
		static	GreaterComparer		msGreaterComparer	;
		static	GreaterEqualComparer	msGreaterEqualComparer	;

	private:
		STLObjectList	mList		;
		uint64		mMaxSize	;
};


#endif
