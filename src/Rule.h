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
		Rule		(const Class *pClass, const Pattern *pPattern)	;
		virtual	~Rule	()						;

	public:
//		virtual	const	bool	operator==	(const Object& rObject)	const	;
//		virtual	const	bool	operator<	(const Object& rObject)	const	;
		virtual	const	bool	operator>	(const Object& rObject)	const	;

//	public:
//		void	MakeTransactionList	()	;

	public:
		const	float32&	GetSupport		()	const	;
		const	float32&	GetConfidence		()	const	;

	public:
		void	Print	()	const	;

	private:
		const	Class*		mpClass		;
		const	Pattern*	mpPattern	;

	public:
		const	string&	GetClassValue	()	const	;

	private:
		float32		mSupport		;
		float32		mConfidence		;

		TransactionList	mTransactionList	;
};


#endif
