#include "IOManager.h"
#include "base/Logger.h"
#include "base/types.h"

#include <fstream>
using std::ifstream;
using std::ofstream;
using std::exception;
using std::filebuf;
using std::ios;


IOManager::IOManager (const string &directory, const string &file)
{
	mDirectory	= directory	;
	mFile		= file		;

	if (! mDirectory.empty () && mDirectory.at (mDirectory.size () - 1) != '/')
		mDirectory += "/";

	IOManager::EscapeString (mDirectory);
	IOManager::EscapeString (mFile);

	mTmpFile = mFile + ".tmp";
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

void IOManager::MoveFile (const string &file_src, const string &file_dst)
{
	LOGMSG (MEDIUM_LEVEL, "IOManager::MoveFile () [%s] to [%s]\n", file_src.c_str (), file_dst.c_str ());

	string cmd = "mv " + file_src + " " + file_dst;

	system (cmd.c_str ());
}

void IOManager::SetDirectory (const string &directory)
{
	mDirectory = directory;

	if (! mDirectory.empty () && mDirectory.at (mDirectory.size () - 1) != '/')
		mDirectory += "/";

	IOManager::EscapeString (mDirectory);
}

void IOManager::SetFile (const string &file)
{
	mFile = file;

	IOManager::EscapeString (mFile);

	mTmpFile = mFile + ".tmp";
}

void IOManager::MakeDirectory (const string &directory)
{
	LOGMSG (MEDIUM_LEVEL, "IOManager::MakeDirectory () [%s]\n", directory.c_str ());

	string cmd = "mkdir -p " + directory;

	system (cmd.c_str ());
}

bool IOManager::WriteFile (const string &value) const
{
	LOGMSG (MEDIUM_LEVEL, "IOManager::WriteFile () [%p]\n", this);

	bool ret = false;

//	string	tmpfile = mDirectory + mTmpFile;
	string	file	= mDirectory + mFile;

	ofstream outfile;

	try
	{
//		outfile.open (tmpfile.c_str ());
		outfile.open (file.c_str ());
	}
	catch (exception &e)
	{
//		LOGMSG (NO_DEBUG, "IOManager::WriteFile () - error: opening file [%s] - exception [%s]\n", tmpfile.c_str (), e.what ());
		LOGMSG (NO_DEBUG, "IOManager::WriteFile () - error: opening file [%s] - exception [%s]\n", file.c_str (), e.what ());
	}

	if (outfile.is_open ())
	{
		outfile << value;
		outfile.close ();
//		MoveFile (tmpfile, file);
		ret = true;
	}
	else
//		LOGMSG (NO_DEBUG, "IOManager::WriteFile () - error: opening file [%s]\n", tmpfile.c_str ());
		LOGMSG (NO_DEBUG, "IOManager::WriteFile () - error: opening file [%s]\n", file.c_str ());

	return ret;
}

void IOManager::LoadFile (list<string> *pLines)
{
	string	line				;
	string	file = mDirectory + mFile	;

	ifstream	infile (file.c_str ())	;

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
				LOGMSG (NO_DEBUG, "IOManager::WriteFile () - error: rdbuff is NULL for file [%s]\n", file.c_str ());

				mBuffer.clear ();
			}
		}
	}
	else
	{
		LOGMSG (NO_DEBUG, "IOManager::WriteFile () - error: file [%s] doesn't exist\n", file.c_str ());

		mBuffer.clear ();
	}

	infile.close ();
}

const string& IOManager::GetFileBuffer () const
{
	return mBuffer;
}
