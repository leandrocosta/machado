#ifndef _APP_OPTIONS_H_
#define _APP_OPTIONS_H_


#include "base/Logger.h"
#include "base/types.h"
#include "DataBase.h"

#include <string>
using namespace std;


class AppOptions
{
	public:
		static	AppOptions*	GetInstance	();
		static	void		Finalize	();

	protected:
		AppOptions	();

	public:
		void	Run	(int argc, char* const* arg)		;
		void	Usage	()				const	;

	public:
			void		SetAppName	(const string &appName)		;
		const	string&		GetTrainingFile	()			const	;
		const	string&		GetTestingFile	()			const	;
		const	float32&	GetSupport	()			const	;
		const	float32&	GetConfidence	()			const	;
		const	uint32&		GetMinRuleLen	()			const	;
		const	uint32&		GetMaxRuleLen	()			const	;
		const	e_rmode&	GetRunMode	()			const	;
		const	e_omode&	GetOrtMode	()			const	;
		const	e_debug&	GetDebugLevel	()			const	;
		const	bool&		GetVerbose	()			const	;
		const	bool&		GetHelp		()			const	;

	private:
		static	AppOptions*	mpInstance;

	private:
		string	mAppName;
		string	mTrainingFile;
		string	mTestingFile;
		float32	mSupport;
		float32	mConfidence;
		uint32	mMinRuleLen;
		uint32	mMaxRuleLen;
		e_rmode	m_rmode;
		e_omode	m_omode;
		e_debug	m_debug_level;
		bool	mVerbose;
		bool	mHelp;

	public:
		static	const	string	DEFAULT_APP_NAME			;
		static	const	string	DEFAULT_TRAINING_FILE			;
		static	const	string	DEFAULT_TESTING_FILE			;
		static	const	float32	DEFAULT_SUPPORT		= 0.1		;
		static	const	float32	DEFAULT_CONFIDENCE	= 0.9		;
		static	const	uint32	DEFAULT_MIN_RULE_LEN	= 1		;
		static	const	uint32	DEFAULT_MAX_RULE_LEN	= 100		;
		static	const	e_rmode	DEFAULT_RUN_MODE	= ORTHOGONAL	;
		static	const	e_omode	DEFAULT_ORT_MODE	= BOTH		;
		static	const	e_debug	DEFAULT_DEBUG_LEVEL	= NO_DEBUG	;
		static	const	bool	DEFAULT_VERBOSE		= false		;
		static	const	bool	DEFAULT_HELP		= false		;
};


#endif
