#include "AppOptions.h"
#include <stdlib.h>
#include <getopt.h>

#include <iostream>
using namespace std;

const	string	AppOptions::DEFAULT_APP_NAME		= "classifier"		;
const	string	AppOptions::DEFAULT_TRAINING_FILE	= "training.dat"	;
const	string	AppOptions::DEFAULT_TESTING_FILE	= "testing.dat"		;
AppOptions*	AppOptions::mpInstance			= NULL			;


AppOptions* AppOptions::GetInstance ()
{
	if (mpInstance == NULL)
		mpInstance = new AppOptions ();

	return mpInstance;
}

void AppOptions::Finalize ()
{
	if (mpInstance)
		delete mpInstance;

	mpInstance = NULL;
}

AppOptions::AppOptions ()
{
	mAppName	= AppOptions::DEFAULT_APP_NAME;
	mTrainingFile	= AppOptions::DEFAULT_TRAINING_FILE;
	mTestingFile	= AppOptions::DEFAULT_TESTING_FILE;
	mSupport	= AppOptions::DEFAULT_SUPPORT;
	mConfidence	= AppOptions::DEFAULT_CONFIDENCE;
	mMinRuleLen	= AppOptions::DEFAULT_MIN_RULE_LEN;
	mMaxRuleLen	= AppOptions::DEFAULT_MAX_RULE_LEN;
	m_rmode		= AppOptions::DEFAULT_RUN_MODE;
	m_omode		= AppOptions::DEFAULT_ORT_MODE;
	m_debug_level	= AppOptions::DEFAULT_DEBUG_LEVEL;
	mVerbose	= AppOptions::DEFAULT_VERBOSE;
	mHelp		= AppOptions::DEFAULT_HELP;
}

void AppOptions::Run (int argc, char* const* argv)
{
	int option_index = 0;

	static struct option long_options[] = {
		{"training-file"	, 1, 0, 'i'},
		{"testing-file"		, 1, 0, 't'},
		{"support"		, 1, 0, 's'},
		{"confidence"		, 1, 0, 'c'},
		{"min-rule-len"		, 1, 0, 'm'},
		{"max-rule-len"		, 1, 0, 'a'},
		{"run-mode"		, 1, 0, 'r'},
		{"orthogonality-mode"	, 1, 0, 'o'},
		{"debug"		, 1, 0, 'd'},
		{"verbose"		, 0, 0, 'v'},
		{"help"			, 0, 0, 'h'},
		{0			, 0, 0, 0  }
	};

	int c = 0;

	while (1)
	{
		c = getopt_long (argc, argv, "i:t:s:c:m:a:r:o:d:vh", long_options, &option_index);

		if (c == -1)
			break;

		switch (c)
		{
			case 'i':
				mTrainingFile = optarg;
				break;

			case 't':
				mTestingFile = optarg;
				break;

			case 's':
				mSupport = atof (optarg);
				break;

			case 'c':
				mConfidence = atof (optarg);
				break;

			case 'm':
				mMinRuleLen = atoi (optarg);
				break;

			case 'a':
				mMaxRuleLen = atoi (optarg);
				break;

			case 'r':
				m_rmode = (e_rmode) optarg [0];
				break;

			case 'o':
				m_omode = (e_omode) optarg [0];
				break;

			case 'd':
				m_debug_level = (e_debug) atoi (optarg);
				break;

			case 'v':
				mVerbose = true;
				break;

			case 'h':
				mHelp = true;
				break;

			default:
				mHelp = true;
				break;
		}
	}
}

void AppOptions::Usage () const
{
	cout << "Usage: ./" << mAppName << " [options]" << endl;
	cout << "Options:" << endl;
	cout << "  -i, --training-file      Set the training file" << endl;
	cout << "  -t, --testing-file       Set the testing file" << endl;
	cout << "  -s, --support            Set the support" << endl;
	cout << "  -c, --confidence         Set the confidence" << endl;
	cout << "  -m, --min-rule-len       Set the minimum length of the rules" << endl;
	cout << "  -a, --max-rule-len       Set the maximum lenfth of the rules" << endl;
	cout << "  -r, --run-mode           Set the run mode [c,o] [CLASSICAL, ORTHOGONAL]" << endl;
	cout << "  -r, --orthogonality-mode           Set the orthogonality mode [s,c,b] [SIMILARITY, COVERAGE, BOTH]" << endl;
	cout << "  -d, --debug              Set the level of debug [0-4] [NODEBUG - MAXLEVEL]" << endl;
	cout << "  -v, --verbose            Use verbose mode" << endl;
	cout << "  -h, --help               Display this information" << endl;
	cout << endl;
}

void AppOptions::SetAppName (const string &appName)
{
	mAppName = appName;
}

const string& AppOptions::GetTrainingFile () const
{
	return mTrainingFile;
}

const string& AppOptions::GetTestingFile () const
{
	return mTestingFile;
}

const float32& AppOptions::GetSupport () const
{
	return mSupport;
}

const float32& AppOptions::GetConfidence () const
{
	return mConfidence;
}

const uint32& AppOptions::GetMinRuleLen () const
{
	return mMinRuleLen;
}

const uint32& AppOptions::GetMaxRuleLen () const
{
	return mMaxRuleLen;
}

const e_rmode& AppOptions::GetRunMode () const
{
	return m_rmode;
}

const e_omode& AppOptions::GetOrtMode () const
{
	return m_omode;
}

const e_debug& AppOptions::GetDebugLevel () const
{
	return m_debug_level;
}

const bool& AppOptions::GetVerbose () const
{
	return mVerbose;
}

const bool& AppOptions::GetHelp () const
{
	return mHelp;
}
