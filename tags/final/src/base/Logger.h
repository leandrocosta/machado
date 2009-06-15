#ifndef _LOGGER_H_
#define _LOGGER_H_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>

#include <string>
using std::string;

#ifdef USE_THREADS
#include "Lockable.h"
#endif

#include "types.h"


typedef enum e_debug_level
{
	SILENT		= -1,
	NO_DEBUG	= 0,
	LOW_LEVEL	= 1,
	MEDIUM_LEVEL	= 2,
	HIGH_LEVEL	= 3,
	MAX_LEVEL	= 4
} e_debug;

#define	LOGGER	Logger::GetInstance ()
#define LOGMSG	Logger::GetInstance ()->Print


/// class Logger - Singleton
#ifdef USE_THREADS
class Logger : private Lockable
#else
class Logger
#endif
{
	// Singleton methods
	public:
		static	Logger*	GetInstance	()	;
		static	void	Finalize	()	;

	// Constructor
	protected:
		Logger	(const e_debug &debug = NO_DEBUG,
				const string &log_file = "", FILE *pFP = NULL)	;

	// Destructor
	public:
		virtual	~Logger	()	;

	// Private methods
	private:
		void	OpenLogFile	()	;
		void	CloseLogFile	()	;

	// Public methods
	public:
		void	SetDebugLevel	(e_debug debug)			;
		void	SetLogFile	(const string &file)		;
		void	Print		(e_debug debug, char *msg, ...)	;

	public:
#ifdef USE_MEM_SIZE
		virtual	const	uint64	GetMemSize	()	const	;
#endif

	// Private fields
	private:
		static	Logger*	mspInstance	;

	private:
		e_debug	m_debug_level	;
		string	m_log_file	;
		FILE*	mpFP		;

	// Public constant class fields
	public:
		static	const	uint32	MSG_BUFF_SIZE		= BUFSIZ	;	
		static	const	uint32	DATE_BUFF_SIZE		= 32		;	
#ifndef HAVE_SYS_SYSCALL_H
		static	const	uint32	GETTID_SYSCALL_ID	= 224		;
#endif
};


#endif
