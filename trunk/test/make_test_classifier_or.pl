#!/usr/bin/perl -w

use strict;
use Common;

sub make_test_classifier_or ($$$$$$$$$$$$$);
sub run_classifier_or ($$$$$$$$$$$$$$$);

sub save_output_file ($$$$$$$$$$$$$);

my $output_dir = $Common::OutputDirClassifierOR;

my $data_base;

foreach $data_base (@Common::DataBases)
{
	system "mkdir -p $output_dir/$data_base";
}

system "mkdir -p $output_dir/average";


my ($s, $c, $n, $l, $e, $a, $b, $u);

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

							for ($u = 0; $u < @Common::ClassifierRMeasures; $u++)
							{
								my $accuracy_avg	= 0;
								my $patterns_avg	= 0;
								my $rules_avg		= 0;
								my $time_avg		= 0;

								foreach $data_base (@Common::DataBases)
								{
									my ($acc, $pat, $rul, $tim);

									make_test_classifier_or ($data_base, $Common::ClassifierSupports[$s], $Common::Confidences[$c], $Common::MinNumRules[$n], $Common::MaxNumRankRules[$l], $Common::ClassifierOMetrics[$e], $Common::ORIGAMIAlphas[$a], $Common::ORIGAMIBetas[$b], $Common::ClassifierRMeasures[$u], \$tim, \$pat, \$rul, \$acc);

									$accuracy_avg	+= $acc;
									$patterns_avg	+= $pat;
									$rules_avg	+= $rul;
									$time_avg	+= $tim;
								}

								$accuracy_avg	/= scalar @Common::DataBases;
								$patterns_avg	/= scalar @Common::DataBases;
								$rules_avg	/= scalar @Common::DataBases;
								$time_avg	/= scalar @Common::DataBases;

								save_output_file ('average', $Common::ClassifierSupports[$s], $Common::Confidences[$c], $Common::MinNumRules[$n], $Common::MaxNumRankRules[$l], $Common::ClassifierOMetrics[$e], $Common::ORIGAMIAlphas[$a], $Common::ORIGAMIBetas[$b], $Common::ClassifierRMeasures[$u], $time_avg, $patterns_avg, $rules_avg, $accuracy_avg);
							}
						}
					}
				}
			}
		}
	}
}

sub make_test_classifier_or ($$$$$$$$$$$$$)
{
	my ($data_base, $support, $confidence, $min_num_rules, $max_num_rank_rules, $ometric, $alpha, $beta, $rule_measure, $avg_time, $avg_patterns, $avg_rules, $accuracy) = @_;

	$$avg_time	= 0;
	$$avg_patterns	= 0;
	$$avg_rules	= 0;
	$$accuracy	= 0;

	my $fold;

	for ($fold = 0; $fold < $Common::NumFolds; $fold++)
	{
		my $log_file = "$output_dir/$data_base/s".$support."_c".$confidence."_n".$min_num_rules."_l".$max_num_rank_rules."_e".$ometric."_a".$alpha."_b".$beta."_u".$rule_measure.".".$fold.".log";

		my $acc = 0;
		my $pat = 0;
		my $rul = 0;
		my $tim = 0;

		my $i;

		for ($i = 0; $i < $Common::NumFolds; $i++)
		{
			my ($a, $p, $r, $t);
		
			run_classifier_or ($data_base, $fold, $support, $confidence, $min_num_rules, $max_num_rank_rules, $ometric, $alpha, $beta, $rule_measure, $log_file, \$a, \$p, \$r, \$t);

			$acc += $a;
			$pat += $p;
			$rul += $r;
			$tim += $t;
		}

		$acc /= $Common::NumFolds;
		$pat /= $Common::NumFolds;
		$rul /= $Common::NumFolds;
		$tim /= $Common::NumFolds;

		$$avg_time	+= $tim;
		$$avg_patterns	+= $pat;
		$$avg_rules	+= $rul;
		$$accuracy	+= $acc;
	}

	$$avg_time	/= $Common::NumFolds;
	$$avg_patterns	/= $Common::NumFolds;
	$$avg_rules	/= $Common::NumFolds;
	$$accuracy	/= $Common::NumFolds;

	print "accuracy [$$accuracy], avg_patterns [$$avg_patterns], avg_rules [$$avg_rules], avg_time [$$avg_time]\n";

	save_output_file ($data_base, $support, $confidence, $min_num_rules, $max_num_rank_rules, $ometric, $alpha, $beta, $rule_measure, $$avg_time, $$avg_patterns, $$avg_rules, $$accuracy);

=comment
	my $out_file = "$output_dir/$data_base/s".$support."_c".$confidence."_n".$min_num_rules."_l".$max_num_rank_rules."_e".$ometric."_a".$alpha."_b".$beta.".out";

	open OUTPUT, ">$out_file";
	print OUTPUT "support [$support], confidence [$confidence], min_num_rules [$min_num_rules], max_num_rank_rules [$max_num_rank_rules], ometric [$ometric], alpha [$alpha], beta [$beta], avg_patterns [$$avg_patterns], avg_rules [$$avg_rules], accuracy [$$accuracy]\n";
	close OUTPUT;

	if ($$accuracy > Common::GetBestAccuracy ('classifier_or', $data_base))
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
=cut
}

sub run_classifier_or ($$$$$$$$$$$$$$$)
{
	my ($data_base, $fold, $support, $confidence, $min_num_rules, $max_num_rank_rules, $ometric, $alpha, $beta, $rule_measure, $log_file, $accuracy, $avg_patterns, $avg_rules, $avg_time) = @_;

	my $training_file = Common::GetTrainingFile ($data_base, $fold);
	my $testing_file = Common::GetTestingFile ($data_base, $fold);

	print "nice -n 10 $Common::AppClassifier -i $training_file -t $testing_file -s $support -c $confidence -p r -n $min_num_rules -l $max_num_rank_rules -r o -o o -e $ometric -a $alpha -b $beta -u $rule_measure -d -1 2&>$log_file\n";
	system "nice -n 10 $Common::AppClassifier -i $training_file -t $testing_file -s $support -c $confidence -p r -n $min_num_rules -l $max_num_rank_rules -r o -o o -e $ometric -a $alpha -b $beta -u $rule_measure -d -1 2&>$log_file";

	Common::GetRunResultFromLogFile ($log_file, $accuracy, $avg_patterns, $avg_rules, $avg_time);
}

sub save_output_file ($$$$$$$$$$$$$)
{
	my ($data_base, $support, $confidence, $min_num_rules, $max_num_rank_rules, $ometric, $alpha, $beta, $rule_measure, $avg_time, $avg_patterns, $avg_rules, $accuracy) = @_;

	my $out_file = "$output_dir/$data_base/s".$support."_c".$confidence."_n".$min_num_rules."_l".$max_num_rank_rules."_e".$ometric."_a".$alpha."_b".$beta."_u".$rule_measure.".out";

	open OUTPUT, ">$out_file";
	print OUTPUT "support [$support], confidence [$confidence], min_num_rules [$min_num_rules], max_num_rank_rules [$max_num_rank_rules], ometric [$ometric], alpha [$alpha], beta [$beta], rule_measure [$rule_measure], avg_time [$avg_time], avg_patterns [$avg_patterns], avg_rules [$avg_rules], accuracy [$accuracy]\n";
	close OUTPUT;

	if ($accuracy > Common::GetBestAccuracy ('classifier_or', $data_base))
	{
		my $best_out_file = Common::GetBestOutputFile ('classifier_or', $data_base);

		print "cp $out_file $best_out_file\n";
		system "cp $out_file $best_out_file";

		if ($data_base ne 'average')
		{
			my $fold;

			for ($fold = 0; $fold < $Common::NumFolds; $fold++)
			{
				my $log_file = "$output_dir/$data_base/s".$support."_c".$confidence."_n".$min_num_rules."_l".$max_num_rank_rules."_e".$ometric."_a".$alpha."_b".$beta."_u".$rule_measure.".".$fold.".log";
				my $best_log_file = "$output_dir/$data_base/best.$fold.log";
				system "cp $log_file $best_log_file";
			}
		}
	}
}
