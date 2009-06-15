#ifndef _RULE_H_
#define _RULE_H_


#include "base/Object.h"
#include "TransactionList.h"

#include <string>
using std::string;

class	Class		;
class	Pattern		;


class Rule : public Object
{
	public:
			Rule	(
				const Class *pClass,
				const Pattern *pPattern)	;
		virtual	~Rule	()				;

	public:
		const	string&	GetClassValue	()	const	;
		const	Class*	GetClass	()	const	;

	public:
		virtual	const	string	GetPrintableString	()	const	;

	public:
		void	Print	()	const	;

	protected:
		const	Class*		mpClass		;
		const	Pattern*	mpPattern	;
};


#endif
