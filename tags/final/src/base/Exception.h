#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_


#include <string>
using std::string;


class Exception
{
	public:
			Exception	(const string& message = "Generic Exception")	;
		virtual	~Exception	()						;

	public:
		const	string&	GetMessage	()	const	;

	private:
		string	mMessage	;
};

#endif
