#!/usr/bin/perl -w

use strict;
use Common;

sub make_test_classifier_or ($$$$$$$$);
sub run_classifier_or ($$$$$$$$$$$$$);

my $output_dir = $Common::OutputDirClassifierOR;

my $data_base;

foreach $data_base (@Common::DataBases)
{
	system "mkdir -p $output_dir/$data_base";
}


my ($s, $c, $n, $l, $e, $a, $b);

for ($s = 0; $s < scalar @Common::ClassifierSupports; $s++)
{
	for ($c = 0; $c < scalar @Common::Confidences; $c++)
	{
		for ($n = 0; $n < scalar @Common::MinNumRules; $n++)
		{
			for ($l = 0; $l < scalar @Common::MaxNumRankRules; $l++)
			{
				for ($e = 0; $e < scalar @Common::ClassifierOMetrics; $e++)
				{
					for ($a = 0; $a < scalar @Common::ORIGAMIAlphas; $a++)
					{
#						next if $Common::ORIGAMIAlphas [$a] == 0.2;
#						next if $Common::ORIGAMIAlphas [$a] == 0.4;
#						next if $Common::ORIGAMIAlphas [$a] == 0.6;
#						next if $Common::ORIGAMIAlphas [$a] > 0.7;

#						for ($b = $a+1; $b < scalar @Common::ORIGAMIBetas; $b++)
						for ($b = 0; $b < scalar @Common::ORIGAMIBetas; $b++)
						{
							next if $Common::ORIGAMIBetas [$b] <= $Common::ORIGAMIAlphas [$a];
#							next if $Common::ORIGAMIBetas [$b] == 0.4;
#							next if $Common::ORIGAMIBetas [$b] == 0.6;
#							next if $Common::ORIGAMIBetas [$b] == 0.8;

							foreach $data_base (@Common::DataBases)
							{
								make_test_classifier_or ($data_base, $Common::ClassifierSupports[$s], $Common::Confidences[$c], $Common::MinNumRules[$n], $Common::MaxNumRankRules[$l], $Common::ClassifierOMetrics[$e], $Common::ORIGAMIAlphas[$a], $Common::ORIGAMIBetas[$b]);
							}
						}
					}
				}
			}
		}
	}
}

sub make_test_classifier_or ($$$$$$$$)
{
	my ($data_base, $support, $confidence, $min_num_rules, $max_num_rank_rules, $ometric, $alpha, $beta) = @_;

	my $accuracy		= 0;
	my $avg_patterns	= 0;
	my $avg_rules		= 0;
	my $fold;

	for ($fold = 0; $fold < $Common::NumFolds; $fold++)
	{
		my $log_file = "$output_dir/$data_base/s".$support."_c".$confidence."_n".$min_num_rules."_l".$max_num_rank_rules."_e".$ometric."_a".$alpha."_b".$beta.".".$fold.".log";

		my ($acc, $pat, $rul);

		run_classifier_or ($data_base, $fold, $support, $confidence, $min_num_rules, $max_num_rank_rules, $ometric, $alpha, $beta, $log_file, \$acc, \$pat, \$rul);

		$accuracy	+= $acc;
		$avg_patterns	+= $pat;
		$avg_rules	+= $rul;
	}

	$accuracy	/= $Common::NumFolds;
	$avg_patterns	/= $Common::NumFolds;
	$avg_rules	/= $Common::NumFolds;

	print "accuracy [$accuracy], avg_patterns [$avg_patterns], avg_rules [$avg_rules]\n";

	system "mkdir -p $output_dir/$data_base/";

	my $out_file = "$output_dir/$data_base/s".$support."_c".$confidence."_n".$min_num_rules."_l".$max_num_rank_rules."_e".$ometric."_a".$alpha."_b".$beta.".out";

	open OUTPUT, ">$out_file";
	print OUTPUT "support [$support], confidence [$confidence], min_num_rules [$min_num_rules], max_num_rank_rules [$max_num_rank_rules], ometric [$ometric], alpha [$alpha], beta [$beta], avg_patterns [$avg_patterns], avg_rules [$avg_rules], accuracy [$accuracy]\n";
	close OUTPUT;

	if ($accuracy > Common::GetBestAccuracy ('classifier_or', $data_base))
	{
		my $best_out_file = Common::GetBestOutputFile ('classifier_or', $data_base);

		print "cp $out_file $best_out_file\n";
		system "cp $out_file $best_out_file";

		for ($fold = 0; $fold < $Common::NumFolds; $fold++)
		{
			my $log_file = "$output_dir/$data_base/s".$support."_c".$confidence."_n".$min_num_rules."_l".$max_num_rank_rules."_e".$ometric."_a".$alpha."_b".$beta.".".$fold.".log";
			my $best_log_file = "$output_dir/$data_base/best.$fold.log";
			system "cp $log_file $best_log_file";
		}
	}
}

sub run_classifier_or ($$$$$$$$$$$$$)
{
	my ($data_base, $fold, $support, $confidence, $min_num_rules, $max_num_rank_rules, $ometric, $alpha, $beta, $log_file, $accuracy, $avg_patterns, $avg_rules) = @_;

	my $training_file = Common::GetTrainingFile ($data_base, $fold);
	my $testing_file = Common::GetTestingFile ($data_base, $fold);

	print "nice -n 10 $Common::AppClassifier -i $training_file -t $testing_file -s $support -c $confidence -p r -n $min_num_rules -l $max_num_rank_rules -r o -o o -e $ometric -a $alpha -b $beta -d -1 2&>$log_file\n";
	system "nice -n 10 $Common::AppClassifier -i $training_file -t $testing_file -s $support -c $confidence -p r -n $min_num_rules -l $max_num_rank_rules -r o -o o -e $ometric -a $alpha -b $beta -d -1 2&>$log_file";

	Common::GetRunResultFromLogFile ($log_file, $accuracy, $avg_patterns, $avg_rules);
}
