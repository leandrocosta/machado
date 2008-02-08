#include "main.h"
#include "AppOptions.h"


int main(int argc, char* argv[])
{
	AppOptions *pAppOptions = AppOptions::GetInstance ();

	pAppOptions->Run (argc, argv);

	if (pAppOptions->GetHelp ())
	{
		pAppOptions->Usage ();
	}
	else
	{
		LOGGER->SetDebugLevel (pAppOptions->GetDebugLevel ());

		DataBase base (
				pAppOptions->GetSupport (), pAppOptions->GetConfidence (),
				pAppOptions->GetMinRuleLen (), pAppOptions->GetMaxRuleLen ());

		base.LoadTrainData (pAppOptions->GetTrainingFile ());
		base.LoadTestData (pAppOptions->GetTestingFile ());
		base.PrintDataInfo ();
		base.SortTransactions ();
		base.ClassifyTestData (
				pAppOptions->GetRunMode (),
				pAppOptions->GetOrtMode (),
				pAppOptions->GetOrtMetric (),
				pAppOptions->GetMinNumRules (),
				pAppOptions->GetMaxNumRankRules ());
	}

	AppOptions::Finalize ();
	Logger::Finalize ();

	return 0;
}
