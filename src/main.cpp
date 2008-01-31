#include "main.h"
#include "AppOptions.h"
#include "DataBase.h"
#include "base/Logger.h"


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

		DataBase *pDataBase = new DataBase (
				pAppOptions->GetSupport (), pAppOptions->GetConfidence (),
				pAppOptions->GetMinRuleLen (), pAppOptions->GetMaxRuleLen ());

		pDataBase->SetDataFile (pAppOptions->GetTrainingFile ());
		pDataBase->LoadTrainData ();

		pDataBase->SetDataFile (pAppOptions->GetTestingFile ());
		pDataBase->LoadTestData ();

		pDataBase->SetRunMode (pAppOptions->GetRunMode ());
		pDataBase->SetOrtMode (pAppOptions->GetOrtMode ());

		pDataBase->ClassifyTestData ();

		delete pDataBase;
	
		Logger::Finalize ();
	}

	return 0;
}
