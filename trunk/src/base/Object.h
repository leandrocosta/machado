#ifndef _OBJECT_H_
#define _OBJECT_H_


#include "types.h"

#include <istream>
using std::istream;

#include <ostream>
using std::ostream;

class ObjectFactory;


class Object
{
	public:
			Object	()	;
		virtual	~Object	()	;

	public:
		virtual	const	bool	operator==	(const Object& rObject)	const	;
		virtual	const	bool	operator<	(const Object& rObject)	const	;
		virtual	const	bool	operator>	(const Object& rObject)	const	;

//	public:
//		virtual	void	Serialize	(void **buffer, uint32 &size)		const	;
//		virtual	void	Unserialize	(const void *buffer, uint32 &size)		;

	public:
		virtual	void	Serialize	(ostream &stream)	const	;
		virtual	void	Unserialize	(istream &stream)		;

	public:
		virtual	Object*	Clone	()	const	;

	public:
		virtual	const	ObjectFactory*	GetFactory	()	const	;

	public:
		virtual	void	Print	()	const	;

	public:
		virtual	const	uint64	GetMemSize	()	const	;
};


#endif
