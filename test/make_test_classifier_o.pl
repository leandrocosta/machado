#!/usr/bin/perl -w

use strict;
use Common;

sub make_test_classifier_o ($$$$$$$$$);
sub run_classifier_o ($$$$$$$$$$$$$$);

my $data_base;

foreach $data_base (@Common::DataBases)
{
	system "mkdir -p $Common::OutputDirClassifierO/$data_base";
}


my ($s, $c, $n, $l, $m, $a, $o, $e);

for ($s = 0; $s < scalar @Common::ClassifierSupports; $s++)
{
	for ($c = 0; $c < scalar @Common::Confidences; $c++)
	{
		for ($n = 0; $n < scalar @Common::MinNumRules; $n++)
		{
			for ($l = 0; $l < scalar @Common::MaxNumRankRules; $l++)
			{
				for ($m = 0; $m < scalar @Common::ClassifierMinRuleLens; $m++)
				{
					for ($a = 0; $a < scalar @Common::ClassifierMaxRuleLens; $a++)
					{
						for ($o = 0; $o < scalar @Common::ClassifierOModes; $o++)
						{
							for ($e = 0; $e < scalar @Common::ClassifierOMetrics; $e++)
							{
								foreach $data_base (@Common::DataBases)
								{
									make_test_classifier_o ($data_base, $Common::ClassifierSupports[$s], $Common::Confidences[$c], $Common::MinNumRules[$n], $Common::MaxNumRankRules[$l], $Common::ClassifierMinRuleLens[$m], $Common::ClassifierMaxRuleLens[$a], $Common::ClassifierOModes[$o], $Common::ClassifierOMetrics[$e]);
								}
							}
						}
					}
				}
			}
		}
	}
}

sub make_test_classifier_o ($$$$$$$$$)
{
	my ($data_base, $support, $confidence, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode, $ometric) = @_;

	my $accuracy		= 0;
	my $avg_patterns	= 0;
	my $avg_rules		= 0;
	my $fold;

	for ($fold = 0; $fold < $Common::NumFolds; $fold++)
	{
		my $log_file = "$Common::OutputDirClassifierO/$data_base/s".$support."_c".$confidence."_n".$min_num_rules."_l".$max_num_rank_rules."_m".$min_rule_len."_a".$max_rule_len."_o".$omode."_e".$ometric.".".$fold.".log";

		my ($acc, $pat, $rul);

		run_classifier_o ($data_base, $fold, $support, $confidence, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode, $ometric, $log_file, \$acc, \$pat, \$rul);

		$accuracy	+= $acc;
		$avg_patterns	+= $pat;
		$avg_rules	+= $rul;
	}

	$accuracy	/= $Common::NumFolds;
	$avg_patterns	/= $Common::NumFolds;
	$avg_rules	/= $Common::NumFolds;

	print "accuracy [$accuracy], avg_patterns [$avg_patterns], avg_rules [$avg_rules]\n";

	system "mkdir -p $Common::OutputDirClassifierO/$data_base/";

	my $out_file = "$Common::OutputDirClassifierO/$data_base/s".$support."_c".$confidence."_n".$min_num_rules."_l".$max_num_rank_rules."_m".$min_rule_len."_a".$max_rule_len."_o".$omode."_e".$ometric.".out";

	open OUTPUT, ">$out_file";
	print OUTPUT "support [$support], confidence [$confidence], min_num_rules [$min_num_rules], max_num_rank_rules [$max_num_rank_rules], min_rule_len [$min_rule_len], max_rule_len [$max_rule_len], omode [$omode], ometric [$ometric], avg_patterns [$avg_patterns], avg_rules [$avg_rules], accuracy [$accuracy]\n";
	close OUTPUT;

	if ($accuracy > Common::GetBestAccuracy ('classifier_o', $data_base))
	{
		my $best_out_file = Common::GetBestOutputFile ('classifier_o', $data_base);

		system "cp $out_file $best_out_file";

		for ($fold = 0; $fold < $Common::NumFolds; $fold++)
		{
			my $log_file = "$Common::OutputDirClassifierO/$data_base/s".$support."_c".$confidence."_n".$min_num_rules."_l".$max_num_rank_rules."_m".$min_rule_len."_a".$max_rule_len."_o".$omode."_e".$ometric.".".$fold.".log";
			my $best_log_file = "$Common::OutputDirClassifierO/$data_base/best.$fold.log";
			system "cp $log_file $best_log_file";
		}
	}
}

sub run_classifier_o ($$$$$$$$$$$$$$)
{
	my ($data_base, $fold, $support, $confidence, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode, $ometric, $log_file, $accuracy, $avg_patterns, $avg_rules) = @_;

	my $training_file = Common::GetTrainingFile ($data_base, $fold);
	my $testing_file = Common::GetTestingFile ($data_base, $fold);

	print "nice -n 10 $Common::AppClassifier -i $training_file -t $testing_file -s $support -c $confidence -n $min_num_rules -l $max_num_rank_rules -m $min_rule_len -a $max_rule_len -r o -o $omode -e $ometric -d -1 2&>$log_file\n";
	system "nice -n 10 $Common::AppClassifier -i $training_file -t $testing_file -s $support -c $confidence -n $min_num_rules -l $max_num_rank_rules -m $min_rule_len -a $max_rule_len -r o -o $omode -e $ometric -d -1 2&>$log_file";

	Common::GetRunResultFromLogFile ($log_file, $accuracy, $avg_patterns, $avg_rules);
}
