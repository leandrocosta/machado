#ifndef _IO_MANAGER_H_
#define _IO_MANAGER_H_


#include <string>
#include <list>
using std::string;
using std::list;

#include "types.h"


class IOManager
{
	public:
			IOManager	(const string &file = "")	;
		virtual	~IOManager	()				;

	private:
		static	void	EscapeString	(string &str)	;

	protected:
			void	SetFile		(const string &file)			;
			bool	WriteFile	(const string &value)		const	;
			void	LoadFile	(list<string> *pLines = NULL)		;
		const	string&	GetFileBuffer	()				const	;

	private:
		string	mBuffer	;

	protected:
		string	mFile	;
};


#endif
