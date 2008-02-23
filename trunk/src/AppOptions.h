#ifndef _APP_OPTIONS_H_
#define _APP_OPTIONS_H_


#include "base/Logger.h"
#include "base/types.h"
#include "DataBase.h"
#include "PatternList.h"

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
		void	SetAppName	(const string &appName)	;

	public:
		const	string&		GetTrainingFile		()	const	;
		const	string&		GetTestingFile		()	const	;
		const	bool&		GetUseMaximalPatterns	()	const	;
		const	float32&	GetSupport		()	const	;
		const	float32&	GetConfidence		()	const	;
		const	uint32&		GetMinNumRules		()	const	;
		const	uint32&		GetMaxNumRankRules	()	const	;
		const	uint32&		GetMinRuleLen		()	const	;
		const	uint32&		GetMaxRuleLen		()	const	;

		const	DataBase::RunMode&		GetRunMode	()	const	;
		const	PatternList::OrtMode&		GetOrtMode	()	const	;
		const	PatternList::OrtMetric&		GetOrtMetric	()	const	;
		const	PatternList::OrtMethod&		GetOrtMethod	()	const	;
		const	PatternList::OrtOrdering&	GetOrtOrdering	()	const	;

		const	e_debug&	GetDebugLevel	()	const	;
		const	bool&		GetVerbose	()	const	;
		const	bool&		GetHelp		()	const	;

	private:
		static	AppOptions*	mpInstance;

	private:
		string		mAppName		;
		string		mTrainingFile		;
		string		mTestingFile		;
		bool		mUseMaximalPatterns	;
		float32		mSupport		;
		float32		mConfidence		;
		uint32		mMinNumRules		;
		uint32		mMaxNumRankRules	;
		uint32		mMinRuleLen		;
		uint32		mMaxRuleLen		;

		DataBase::RunMode		mRunMode	;
		PatternList::OrtMode		mOrtMode	;
		PatternList::OrtMetric		mOrtMetric	;
		PatternList::OrtMethod		mOrtMethod	;
		PatternList::OrtOrdering	mOrtOrdering	;

		e_debug	m_debug_level	;
		bool	mVerbose	;
		bool	mHelp		;

	public:
		static	const	string	DEFAULT_APP_NAME	;
		static	const	string	DEFAULT_TRAINING_FILE	;
		static	const	string	DEFAULT_TESTING_FILE	;

		static	const	bool	DEFAULT_USE_MAXIMAL_PATTERNS	= false	;
		static	const	float32	DEFAULT_SUPPORT			= 0.001	;
		static	const	float32	DEFAULT_CONFIDENCE		= 0.9	;
		static	const	uint32	DEFAULT_MAX_NUM_RANK_RULES	= 100	;
		static	const	uint32	DEFAULT_MIN_NUM_RULES		= 1	;
		static	const	uint32	DEFAULT_MIN_RULE_LEN		= 1	;
		static	const	uint32	DEFAULT_MAX_RULE_LEN		= 10	;

		static	const	DataBase::RunMode		DEFAULT_RUN_MODE	= DataBase::MODE_ORTHOGONAL		;
		static	const	PatternList::OrtMode		DEFAULT_ORT_MODE	= PatternList::ORTH_HEURISTICAL		;
		static	const	PatternList::OrtMetric		DEFAULT_ORT_METRIC	= PatternList::METRIC_CLASS_COVERAGE	;
		static	const	PatternList::OrtMethod		DEFAULT_ORT_METHOD	= PatternList::METHOD_SET		;
		static	const	PatternList::OrtOrdering	DEFAULT_ORT_ORDERING	= PatternList::ORDERING_NONE		;

		static	const	e_debug	DEFAULT_DEBUG_LEVEL	= NO_DEBUG	;
		static	const	bool	DEFAULT_VERBOSE		= false		;
		static	const	bool	DEFAULT_HELP		= false		;
};


#endif
