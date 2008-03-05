#!/usr/bin/perl -w

use strict;
use Common;

sub make_test_classifier_o ($$$$$$$$$$$$$$$$);
sub run_classifier_o ($$$$$$$$$$$$$$$$$$);

sub save_output_file ($$$$$$$$$$$$$$$$);

my $output_dir = $Common::OutputDirClassifierO;

my $data_base;

foreach $data_base (@Common::DataBases)
{
	system "mkdir -p $output_dir/$data_base";
}

system "mkdir -p $output_dir/average";


my ($s, $c, $p, $n, $l, $m, $x, $o, $e, $w, $g);

for ($s = 0; $s < scalar @Common::ClassifierSupports; $s++)
{
	for ($c = 0; $c < scalar @Common::Confidences; $c++)
	{
		for ($p = 0; $p < scalar @Common::ClassifierPatternSets; $p++)
		{
			next if $Common::ClassifierPatternSets [$p] ne 'f';

			for ($n = 0; $n < scalar @Common::MinNumRules; $n++)
			{
				for ($l = 0; $l < scalar @Common::MaxNumRankRules; $l++)
				{
					for ($m = 0; $m < scalar @Common::ClassifierMinRuleLens; $m++)
					{
						for ($x = 0; $x < scalar @Common::ClassifierMaxRuleLens; $x++)
						{
							for ($o = 0; $o < scalar @Common::ClassifierOModes; $o++)
							{
								for ($e = 0; $e < scalar @Common::ClassifierOMetrics; $e++)
								{
									for ($w = 0; $w < scalar @Common::ClassifierOMethods; $w++)
									{
										for ($g = 0; $g < scalar @Common::ClassifierOOrderings; $g++)
										{
											my $accuracy_avg = 0;
											my $patterns_avg = 0;
											my $rules_avg = 0;
											my $time_avg = 0;

											foreach $data_base (@Common::DataBases)
											{
												my ($acc, $pat, $rul, $tim);

												make_test_classifier_o ($data_base, $Common::ClassifierSupports[$s], $Common::Confidences[$c], $Common::ClassifierPatternSets[$p], $Common::MinNumRules[$n], $Common::MaxNumRankRules[$l], $Common::ClassifierMinRuleLens[$m], $Common::ClassifierMaxRuleLens[$x], $Common::ClassifierOModes[$o], $Common::ClassifierOMetrics[$e], $Common::ClassifierOMethods[$w], $Common::ClassifierOOrderings[$g], \$tim, \$pat, \$rul, \$acc);

												$accuracy_avg += $acc;
												$patterns_avg += $pat;
												$rules_avg += $rul;
												$time_avg += $tim;
											}

											$accuracy_avg /= scalar @Common::DataBases;
											$patterns_avg /= scalar @Common::DataBases;
											$rules_avg /= scalar @Common::DataBases;
											$time_avg /= scalar @Common::DataBases;

											save_output_file ('average', $Common::ClassifierSupports[$s], $Common::Confidences[$c], $Common::ClassifierPatternSets[$p], $Common::MinNumRules[$n], $Common::MaxNumRankRules[$l], $Common::ClassifierMinRuleLens[$m], $Common::ClassifierMaxRuleLens[$x], $Common::ClassifierOModes[$o], $Common::ClassifierOMetrics[$e], $Common::ClassifierOMethods[$w], $Common::ClassifierOOrderings[$g], $time_avg, $patterns_avg, $rules_avg, $accuracy_avg);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

sub make_test_classifier_o ($$$$$$$$$$$$$$$$)
{
	my ($data_base, $support, $confidence, $pattern_set, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode, $ometric, $omethod, $oordering, $avg_time, $avg_patterns, $avg_rules, $accuracy) = @_;

	$$avg_time	= 0;
	$$avg_patterns	= 0;
	$$avg_rules	= 0;
	$$accuracy	= 0;

	my $fold;

	for ($fold = 0; $fold < $Common::NumFolds; $fold++)
	{
		my $log_file = "$output_dir/$data_base/s".$support."_c".$confidence."_p".$pattern_set."_n".$min_num_rules."_l".$max_num_rank_rules."_m".$min_rule_len."_x".$max_rule_len."_o".$omode."_e".$ometric."_w".$omethod."_g".$oordering.".".$fold.".log";

		my ($acc, $pat, $rul, $tim);

		run_classifier_o ($data_base, $fold, $support, $confidence, $pattern_set, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode, $ometric, $omethod, $oordering, $log_file, \$acc, \$pat, \$rul, \$tim);

		$$avg_time	+= $tim;
		$$avg_patterns	+= $pat;
		$$avg_rules	+= $rul;
		$$accuracy	+= $acc;
	}

	$$avg_time	/= $Common::NumFolds;
	$$avg_patterns	/= $Common::NumFolds;
	$$avg_rules	/= $Common::NumFolds;
	$$accuracy	/= $Common::NumFolds;

	print "accuracy [$$accuracy], avg_patterns [$$avg_patterns], avg_rules [$$avg_rules], time [$$avg_time]\n";

	save_output_file ($data_base, $support, $confidence, $pattern_set, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode, $ometric, $omethod, $oordering, $$avg_time, $$avg_patterns, $$avg_rules, $$accuracy);

	if ($$accuracy > Common::GetBestAccuracy ('classifier_o', $data_base))
	{
		for ($fold = 0; $fold < $Common::NumFolds; $fold++)
		{
			my $log_file = "$output_dir/$data_base/s".$support."_c".$confidence."_p".$pattern_set."_n".$min_num_rules."_l".$max_num_rank_rules."_m".$min_rule_len."_x".$max_rule_len."_o".$omode."_e".$ometric."_w".$omethod."_g".$oordering.".".$fold.".log";
			my $best_log_file = "$output_dir/$data_base/best.$fold.log";
			system "cp $log_file $best_log_file";
		}
	}

=comment
	my $out_file = "$output_dir/$data_base/s".$support."_c".$confidence."_p".$pattern_set."_n".$min_num_rules."_l".$max_num_rank_rules."_m".$min_rule_len."_x".$max_rule_len."_o".$omode."_e".$ometric."_w".$omethod."_g".$oordering.".out";

	open OUTPUT, ">$out_file";
	print OUTPUT "support [$support], confidence [$confidence], pattern_set [$pattern_set], min_num_rules [$min_num_rules], max_num_rank_rules [$max_num_rank_rules], min_rule_len [$min_rule_len], max_rule_len [$max_rule_len], omode [$omode], ometric [$ometric], omethod [$omethod], oordering [$oordering], avg_patterns [$$avg_patterns], avg_rules [$$avg_rules], accuracy [$$accuracy]\n";
	close OUTPUT;

	if ($$accuracy > Common::GetBestAccuracy ('classifier_o', $data_base))
	{
		my $best_out_file = Common::GetBestOutputFile ('classifier_o', $data_base);

		print "cp $out_file $best_out_file\n";
		system "cp $out_file $best_out_file";

		for ($fold = 0; $fold < $Common::NumFolds; $fold++)
		{
			my $log_file = "$output_dir/$data_base/s".$support."_c".$confidence."_p".$pattern_set."_n".$min_num_rules."_l".$max_num_rank_rules."_m".$min_rule_len."_x".$max_rule_len."_o".$omode."_e".$ometric."_w".$omethod."_g".$oordering.".".$fold.".log";
			my $best_log_file = "$output_dir/$data_base/best.$fold.log";
			system "cp $log_file $best_log_file";
		}
	}
=cut
}

sub run_classifier_o ($$$$$$$$$$$$$$$$$$)
{
	my ($data_base, $fold, $support, $confidence, $pattern_set, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode, $ometric, $omethod, $oordering, $log_file, $accuracy, $avg_patterns, $avg_rules, $avg_time) = @_;

	my $training_file = Common::GetTrainingFile ($data_base, $fold);
	my $testing_file = Common::GetTestingFile ($data_base, $fold);

	print "nice -n 10 $Common::AppClassifier -i $training_file -t $testing_file -s $support -c $confidence -p $pattern_set -n $min_num_rules -l $max_num_rank_rules -m $min_rule_len -x $max_rule_len -r o -o $omode -e $ometric -w $omethod -g $oordering -d -1 2&>$log_file\n";
	system "nice -n 10 $Common::AppClassifier -i $training_file -t $testing_file -s $support -c $confidence -p $pattern_set -n $min_num_rules -l $max_num_rank_rules -m $min_rule_len -x $max_rule_len -r o -o $omode -e $ometric -w $omethod -g $oordering -d -1 2&>$log_file";

	Common::GetRunResultFromLogFile ($log_file, $accuracy, $avg_patterns, $avg_rules, $avg_time);
}

sub save_output_file ($$$$$$$$$$$$$$$$)
{
	my ($data_base, $support, $confidence, $pattern_set, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode, $ometric, $omethod, $oordering, $avg_time, $avg_patterns, $avg_rules, $accuracy) = @_;

	my $out_file = "$output_dir/$data_base/s".$support."_c".$confidence."_p".$pattern_set."_n".$min_num_rules."_l".$max_num_rank_rules."_m".$min_rule_len."_x".$max_rule_len."_o".$omode."_e".$ometric."_w".$omethod."_g".$oordering.".out";

	open OUTPUT, ">$out_file";
	print OUTPUT "support [$support], confidence [$confidence], pattern_set [$pattern_set], min_num_rules [$min_num_rules], max_num_rank_rules [$max_num_rank_rules], min_rule_len [$min_rule_len], max_rule_len [$max_rule_len], omode [$omode], ometric [$ometric], omethod [$omethod], oordering [$oordering], avg_time [$avg_time], avg_patterns [$avg_patterns], avg_rules [$avg_rules], accuracy [$accuracy]\n";
	close OUTPUT;

	if ($accuracy > Common::GetBestAccuracy ('classifier_o', $data_base))
	{
		my $best_out_file = Common::GetBestOutputFile ('classifier_o', $data_base);

		print "cp $out_file $best_out_file\n";
		system "cp $out_file $best_out_file";
	}
}
