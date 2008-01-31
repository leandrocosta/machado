#include "Logger.h"

#include <stdarg.h>
#include <unistd.h>
#include <time.h>

#ifdef HAVE_SYS_SYSCALL_H
#include <sys/syscall.h>
#endif

#ifdef USE_THREADS
#include "Thread.h"
#endif

#include "types.h"


Logger*	Logger::mspInstance = NULL;


Logger* Logger::GetInstance ()
{
	if (mspInstance == NULL)
		mspInstance = new Logger ();

	return mspInstance;
}

void Logger::Finalize ()
{
	LOGMSG (LOW_LEVEL, "Logger::Finalize () - [%p]\n", mspInstance);

	if (mspInstance)
		delete mspInstance;

	mspInstance = NULL;
}

#ifdef USE_THREADS
Logger::Logger (const e_debug &debug, const string &log_file, FILE *pFP) : Lockable ()
#else
Logger::Logger (const e_debug &debug, const string &log_file, FILE *pFP)
#endif
{
	m_debug_level	= debug;
	m_log_file	= log_file;
	mpFP		= pFP;
}

Logger::~Logger ()
{
//	LOGMSG (NO_DEBUG, "Logger::~Logger () - [%p]\n", this);

	CloseLogFile ();
}

void Logger::SetDebugLevel (e_debug debug)
{
	m_debug_level = debug;
}

void Logger::SetLogFile (const string &file)
{
	m_log_file = file;

	OpenLogFile ();
}

void Logger::OpenLogFile ()
{
	mpFP = fopen (m_log_file.c_str (), "w");
}

void Logger::CloseLogFile ()
{
	if (mpFP)
		fclose (mpFP);

	mpFP = NULL;
}

void Logger::Print (e_debug debug, char *msg, ...)
{
	if (m_debug_level >= debug)
	{
#ifdef USE_THREADS
		int oldstate = Thread::DisableCancel ();
#endif

		char buffer[Logger::MSG_BUFF_SIZE];
		va_list ap;
		
		va_start (ap, msg);
		vsnprintf (buffer, Logger::MSG_BUFF_SIZE, msg, ap);

		time_t str_time;
		struct tm str_tm;
		char log_date[Logger::DATE_BUFF_SIZE];
		time (&str_time);
		localtime_r (&str_time, &str_tm);
		strftime (log_date, Logger::DATE_BUFF_SIZE, "%d/%m/%Y-%H:%M:%S", &str_tm);

		if (mpFP)
		{
#ifdef USE_THREADS
			Lock ();
#endif

#ifdef HAVE_SYS_SYSCALL_H
			fprintf	(mpFP, "%s-%ld: %s", log_date, syscall (__NR_gettid), buffer);
#else
			fprintf	(mpFP, "%s-%ld: %s", log_date, syscall (Logger::GETTID_SYSCALL_ID), buffer);
#endif

			fflush	(mpFP);
#ifdef USE_THREADS
			Unlock ();
#endif
		}
		else
		{
#ifdef USE_THREADS
			Lock ();
#endif

#ifdef HAVE_SYS_SYSCALL_H
			fprintf	(stderr, "%s-%ld: %s", log_date, syscall (__NR_gettid), buffer);
#else
			fprintf	(stderr, "%s-%ld: %s", log_date, syscall (Logger::GETTID_SYSCALL_ID), buffer);
#endif

			fflush	(stderr);
#ifdef USE_THREADS
			Unlock ();
#endif
		}

		va_end (ap);

#ifdef USE_THREADS
		Thread::SetCancelState (oldstate);
#endif
	}
}

const uint64 Logger::GetMemSize () const
{
	LOGMSG (MEDIUM_LEVEL, "Logger::GetMemSize () - [%p]\n", this);

	uint64 size = 0;

	size += sizeof (e_debug);
	size += sizeof (string);
	size += sizeof (FILE *);
	size += m_log_file.size () + 1;
#ifdef USE_THREADS
	size += Lockable::GetMemSize ();
#endif

	return size;
}
