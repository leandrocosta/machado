#ifndef _OBJECT_LIST_H_
#define _OBJECT_LIST_H_


#include "Object.h"

#include <deque>
using std::deque;


class ObjectList : public Object
{
	public:
			ObjectList	(
					const 	uint64&	max_size	= ULONG_MAX,
					const 	uint64&	size		= 0,
						Object*	pValue		= NULL		)	;
		virtual	~ObjectList	()							;

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
		typedef deque<Object *>				STLObjectList		;
		typedef STLObjectList::iterator			STLObjectList_it	;
		typedef STLObjectList::const_iterator		STLObjectList_cit	;
		typedef STLObjectList::reverse_iterator		STLObjectList_rit	;
		typedef STLObjectList::const_reverse_iterator	STLObjectList_crit	;

	public:
			STLObjectList_it	GetBegin	()		;
		const	STLObjectList_cit	GetBegin	()	const	;
			STLObjectList_it	GetEnd		()		;
		const	STLObjectList_cit	GetEnd		()	const	;
			STLObjectList_rit	GetRBegin	()		;
		const	STLObjectList_crit	GetRBegin	()	const	;
			STLObjectList_rit	GetREnd		()		;
		const	STLObjectList_crit	GetREnd		()	const	;

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
		void			MoveToEnd	(Object *pObject)	;
		void			Remove		(Object *pObject)	;
		void			RemoveAll	()			;
		void			DeleteAll	()			;
		STLObjectList_it	Erase		(STLObjectList_it it)	;

	public:
		const	bool	Find		(const Object *pObject) const	;
		const	bool	FindByPtr	(const Object *pObject)	const	;

	public:
		virtual	const	bool	IsSubList (const ObjectList& rList)	const	;

	public:
			void		Sort				()				;
			void		ReverseSort			()				;
		const ObjectList*	GetPartialSortCopy		(const uint64 &size)	const	;
		const ObjectList*	GetPartialReverseSortCopy	(const uint64 &size)	const	;

	public:
		const	uint64	GetSize	()	const	;

	public:
#ifdef USE_SERIALIZE
		virtual	void	Serialize	(ostream &stream)	const	;
#endif

	public:
		virtual	void	Print	()	const	;

	public:
#ifdef USE_MEM_SIZE
		virtual	const	uint64	GetMemSize	()	const	;
#endif

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
