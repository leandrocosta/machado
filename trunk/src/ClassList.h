#ifndef _CLASS_LIST_H_
#define _CLASS_LIST_H_


#include "base/ObjectList.h"
#include "Class.h"

#include <string>
using std::string;


class ClassList : public ObjectList
{
	public:
			ClassList	(const uint64 &max_size = ULONG_MAX)	;
		virtual	~ClassList	()					;

	public:
		typedef STLObjectList			STLClassList	;
		typedef STLClassList::iterator		STLClassList_it	;
		typedef STLClassList::const_iterator	STLClassList_cit	;

	public:
			STLClassList_it	GetBegin	()		;
		const	STLClassList_cit	GetBegin	()	const	;
			STLClassList_it	GetEnd		()		;
		const	STLClassList_cit	GetEnd		()	const	;

	public:
		virtual	const	bool	operator>	(const Object& rObject)	const	;

	public:
		Class*	GetClassByValue	(const string &value)	const	;

//	public:
//		void	Print	()	const	;
};


#endif
