#include "IOManager.h"
#include "Logger.h"

#include <fstream>
using std::ifstream;
using std::ofstream;
using std::exception;
using std::filebuf;
using std::ios;


IOManager::IOManager (const string &file)
{
	mFile = file;

	IOManager::EscapeString (mFile);
}

IOManager::~IOManager ()
{

}

void IOManager::EscapeString (string &str)
{
	for (uint32 i = 0; i < str.size (); i++)
	{
		switch (str.at (i))
		{
			case '?':
				str.replace (i, 1, "\\?");
				i++;
				break;

			case '=':
				str.replace (i, 1, "\\=");
				i++;
				break;

			case '&':
				str.replace (i, 1, "\\&");
				i++;
				break;

			case ';':
				str.replace (i, 1, "\\;");
				i++;
				break;

			case '\'':
				str.replace (i, 1, "\\'");
				i++;
				break;

			case '"':
				str.replace (i, 1, "\\\"");
				i++;
				break;

			case ' ':
				str.replace (i, 1, "\\ ");
				i++;
				break;

			case '|':
				str.replace (i, 1, "\\|");
				i++;
				break;

			case '(':
				str.replace (i, 1, "\\(");
				i++;
				break;

			case ')':
				str.replace (i, 1, "\\)");
				i++;
				break;
		}
	}
}

void IOManager::SetFile (const string &file)
{
	mFile = file;

	IOManager::EscapeString (mFile);
}

bool IOManager::WriteFile (const string &value) const
{
#ifdef USE_LOGGER
	LOGMSG (MEDIUM_LEVEL, "IOManager::WriteFile () [%p]\n", this);
#endif

	bool ret = false;

	ofstream outfile;

	try
	{
		outfile.open (mFile.c_str ());
	}
	catch (exception &e)
	{
#ifdef USE_LOGGER
		LOGMSG (NO_DEBUG, "IOManager::WriteFile () - error: opening file [%s] - exception [%s]\n", mFile.c_str (), e.what ());
#endif
	}

	if (outfile.is_open ())
	{
		outfile << value;
		outfile.close ();
		ret = true;
	}
	else
	{
#ifdef USE_LOGGER
		LOGMSG (NO_DEBUG, "IOManager::WriteFile () - error: opening file [%s]\n", mFile.c_str ());
#endif
	}

	return ret;
}

void IOManager::LoadFile (list<string> *pLines)
{
	string		line			;
	ifstream	infile (mFile.c_str ())	;

	if (infile.is_open ())
	{
		if (pLines)
		{
			while (getline (infile, line, '\n'))
			{
				pLines->push_back (line);
			}
		}
		else
		{
			filebuf *pbuf = infile.rdbuf ();

			if (pbuf)
			{
				unsigned int size = pbuf->pubseekoff (0, ios::end, ios::in);
				pbuf->pubseekpos (0, ios::in);
				char *buff = new char [size+1];
				pbuf->sgetn (buff, size);
				buff[size] = '\0';
				mBuffer.assign (buff);

				delete buff;
			}
			else
			{
#ifdef USE_LOGGER
				LOGMSG (NO_DEBUG, "IOManager::WriteFile () - error: rdbuff is NULL for file [%s]\n", mFile.c_str ());
#endif

				mBuffer.clear ();
			}
		}
	}
	else
	{
#ifdef USE_LOGGER
		LOGMSG (NO_DEBUG, "IOManager::WriteFile () - error: file [%s] doesn't exist\n", mFile.c_str ());
#endif

		mBuffer.clear ();
	}

	infile.close ();
}

const string& IOManager::GetFileBuffer () const
{
	return mBuffer;
}
