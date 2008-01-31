#include "Tokenizer.h"
#include "base/Logger.h"


Tokenizer::Tokenizer (const string &delimiter)
{
	mDelimiter = delimiter;

	LOGMSG (MAX_LEVEL, "Tokenizer::Tokenizer () - delimiter [%s]\n", mDelimiter.c_str ());
}

Tokenizer::Tokenizer (const char &delimiter)
{
	mDelimiter = delimiter;

	LOGMSG (MAX_LEVEL, "Tokenizer::Tokenizer () - delimiter [%s]\n", mDelimiter.c_str ());
}

Tokenizer::~Tokenizer ()
{

}

void Tokenizer::SetDelimiter (const string &delimiter)
{
	mDelimiter = delimiter;

	LOGMSG (MAX_LEVEL, "Tokenizer::SetDelimiter () - delimiter [%s]\n", mDelimiter.c_str ());
}

const string& Tokenizer::GetDelimiter () const
{
	return mDelimiter;
}

void Tokenizer::Tokenize (const string &str, list<string>& tokens) const
{
	// Skip delimiter at beginning
	string::size_type prv_pos = str.find_first_not_of (mDelimiter, 0);

	// Find first "non-delimiter"
	string::size_type pos = str.find_first_of(mDelimiter, prv_pos);

	while (string::npos != pos || string::npos != prv_pos)
	{
		// Found a token, add it to the list
		tokens.push_back(str.substr(prv_pos, pos - prv_pos));

		// Skip delimiter.  Note the "not_of"
		prv_pos = str.find_first_not_of(mDelimiter, pos);

		// Find next "non-delimiter"
		pos = str.find_first_of(mDelimiter, prv_pos);
	}
}
