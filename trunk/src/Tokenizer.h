#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_


#include <string>
#include <list>
using std::string;
using std::list;


class Tokenizer
{
	public:
			Tokenizer	(const string& delimiter)	;
			Tokenizer	(const char& delimiter)		;
		virtual	~Tokenizer	()				;

	public:
		void	Tokenize	(const string &str, list<string>& tokens)	const	;

	protected:
			void	SetDelimiter	(const string &delimiter)		;
		const	string&	GetDelimiter	()				const	;

	private:
		string	mDelimiter;
};


#endif
