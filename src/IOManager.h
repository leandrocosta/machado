#ifndef _IO_MANAGER_H_
#define _IO_MANAGER_H_


#include <string>
#include <list>
using std::string;
using std::list;

#include "base/types.h"


class IOManager
{
	public:
			IOManager	(const string &directory = "", const string &file = "")	;
		virtual	~IOManager	()							;

	private:
		static	void	EscapeString	(string &str)	;

	protected:
		static	void	MoveFile	(const string &file_src, const string &file_dst)	;
			void	SetDirectory	(const string &directory)				;
			void	SetFile		(const string &file)					;
			bool	WriteFile	(const string &value)				const	;
			void	LoadFile	(list<string> *pLines = NULL)				;
		const	string&	GetFileBuffer	()						const	;

	public:
		static	void	MakeDirectory	(const string &directory)	;

	private:
		string	mDirectory	;
		string	mTmpFile	;
		string	mBuffer		;

	protected:
		string	mFile	;

	public:
		static	const	uint32	MAX_SUB_DIRS	= 1000	;
};


#endif
