#include "main.h"
#include "AppOptions.h"


int main(int argc, char* argv[])
{
	/*
	 * AppOptions is a singleton,
	 * used to store the user options
	 */

	AppOptions *pAppOptions = AppOptions::GetInstance ();

	/*
	 * Run AppOptions passing argc and argv
	 * as input parameters
	 */

	pAppOptions->Run (argc, argv);

	/*
	 * If the '-h' options was used,
	 * print the 'usage' message and
	 * finalize the execution
	 */

	if (pAppOptions->GetHelp ())
	{
		pAppOptions->Usage ();
	}
	else
	{
		/*
		 * Set the debug level to the
		 * option passed in option '-d'
		 */

		LOGGER->SetDebugLevel (pAppOptions->GetDebugLevel ());

		/*
		 * Construct the data base, passing
		 * support, confidence, minimum rule length
		 * and maximum rule length
		 */

		DataBase base (
				pAppOptions->GetSupport (), pAppOptions->GetConfidence (),
				pAppOptions->GetMinRuleLen (), pAppOptions->GetMaxRuleLen ());

		/*
		 * Load training data file
		 */

		base.LoadTrainData (pAppOptions->GetTrainingFile ());

		/*
		 * Load testing data file
		 */

		base.LoadTestData (pAppOptions->GetTestingFile ());

		/*
		 * Print data file informations (number
		 * of items, training transactions and
		 * testing transactions
		 */

		base.PrintDataInfo ();

		/*
		 * Sort each transaction (including training
		 * and testing transactions) by item ID. It
		 * makes the searching for items and easier
		 */

		base.SortTransactions ();

		/*
		 * For each training transaction, construct
		 * an array of boolean values indexed by the
		 * item IDs. It makes the method
		 * Transaction::IsCoveredBy () run much faster
		 */

		base.MakeItemCoverageArrays ();

		/*
		 * Classify testing transactions, using
		 * run mode, orthogonality mode, orthogonality
		 * metric, mininum number of rules and
		 * maximum number of rules in the rank list
		 * (maximum size of rank)
		 */

		/*
		 * Set the measure used by operators
		 * for rules during the sorting of rank
		 */

		RankingRule::SetPrioritizedMeasure (pAppOptions->GetRuleMeasure ());

		base.ClassifyTestData (
				pAppOptions->GetRunMode (),
				pAppOptions->GetOrtMode (),
				pAppOptions->GetOrtMethod (),
				pAppOptions->GetOrtMetric (),
				pAppOptions->GetOrtOrdering (),
				pAppOptions->GetRuleMeasure (),
				pAppOptions->GetMinNumRules (),
				pAppOptions->GetMaxNumRankRules (),
				pAppOptions->GetPatternSet (),
        pAppOptions->GetClassCoverageFactor (),
				pAppOptions->GetAlpha (),
				pAppOptions->GetBeta ());
	}

	/*
	 * Finalize singleton AppOptions
	 */

	AppOptions::Finalize ();

	/*
	 * Finalize singleton Logger
	 */

	Logger::Finalize ();

	return 0;
}
