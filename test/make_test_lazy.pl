#!/usr/bin/perl -w

use strict;
use Common;

sub make_test_lazy ($$$$$$$$$$);
sub run_lazy ($$$$$$$$$$$$);

sub save_output_file ($$$$$$$$$$);

my $output_dir = $Common::OutputDirLazy;

my $data_base;

foreach $data_base (@Common::DataBases)
{
	system "mkdir -p $output_dir/$data_base";
}

system "mkdir -p $output_dir/average";


my ($s, $c, $n, $m, $l);

for ($s = 0; $s < scalar @Common::LazySupports; $s++)
{
	for ($c = 0; $c < scalar @Common::Confidences; $c++)
	{
		for ($n = 0; $n < scalar @Common::MinNumRules; $n++)
		{
			for ($m = 0; $m < scalar @Common::LazyMaxSizes; $m++)
			{
				for ($l = 0; $l < scalar @Common::MaxNumRankRules; $l++)
				{
					my $accuracy_avg	= 0;
					my $patterns_avg	= 0;
					my $rules_avg		= 0;
					my $time_avg		= 0;

					foreach $data_base (@Common::DataBases)
					{
						my ($acc, $pat, $rul, $tim);

						make_test_lazy ($data_base, $Common::LazySupports[$s], $Common::Confidences[$c], $Common::MinNumRules[$n], $Common::LazyMaxSizes[$m], $Common::MaxNumRankRules[$l], \$tim, \$pat, \$rul, \$acc);

						$accuracy_avg	+= $acc;
						$patterns_avg	+= $pat;
						$rules_avg	+= $rul;
						$time_avg	+= $tim;
					}

					$accuracy_avg /= scalar @Common::DataBases;
					$patterns_avg /= scalar @Common::DataBases;
					$rules_avg /= scalar @Common::DataBases;
					$time_avg /= scalar @Common::DataBases;

					save_output_file ('average', $Common::LazySupports[$s], $Common::Confidences[$c], $Common::MinNumRules[$n], $Common::LazyMaxSizes[$m], $Common::MaxNumRankRules[$l], $time_avg, $patterns_avg, $rules_avg, $accuracy_avg);
				}
			}
		}
	}
}

sub make_test_lazy ($$$$$$$$$$)
{
	my ($data_base, $support, $confidence, $min_rules, $max_size, $ranking_size, $avg_time, $avg_patterns, $avg_rules, $accuracy) = @_;

	$$avg_time	= 0;
	$$avg_patterns	= 0;
	$$avg_rules	= 0;
	$$accuracy	= 0;

	my $fold;

	for ($fold = 0; $fold < $Common::NumFolds; $fold++)
	{
		my $log_file = "$output_dir/$data_base/s".$support."_c".$confidence."_n".$min_rules."_m".$max_size."_l".$ranking_size.".".$fold.".log";

		my ($acc, $pat, $rul, $tim);

		run_lazy ($data_base, $fold, $support, $confidence, $min_rules, $max_size, $ranking_size, $log_file, \$acc, \$pat, \$rul, \$tim);

		$$avg_time	+= $tim;
		$$avg_patterns	+= $pat;
		$$avg_rules	+= $rul;
		$$accuracy	+= $acc;
	}

	$$avg_time	/= $Common::NumFolds;
	$$avg_patterns	/= $Common::NumFolds;
	$$avg_rules	/= $Common::NumFolds;
	$$accuracy	/= $Common::NumFolds;

	print "accuracy [$$accuracy], avg_patterns [$$avg_patterns], avg_rules [$$avg_rules], avg_time [$avg_time]\n";

	save_output_file ($data_base, $support, $confidence, $min_rules, $max_size, $ranking_size, $$avg_time, $$avg_patterns, $$avg_rules, $$accuracy);

=comment
	my $out_file = "$output_dir/$data_base/s".$support."_c".$confidence."_n".$min_rules."_m".$max_size."_l".$ranking_size.".out";

	open OUTPUT, ">$out_file";
	print OUTPUT "support [$support], confidence [$confidence], min_rules [$min_rules], max_size [$max_size], ranking_size [$ranking_size], avg_patterns [$$avg_patterns], avg_rules [$$avg_rules], accuracy [$$accuracy]\n";
	close OUTPUT;

	if ($$accuracy > Common::GetBestAccuracy ('lazy', $data_base))
	{
		my $best_out_file = Common::GetBestOutputFile ('lazy', $data_base);

		print "cp $out_file $best_out_file\n";
		system "cp $out_file $best_out_file";

		for ($fold = 0; $fold < $Common::NumFolds; $fold++)
		{
			my $log_file = "$output_dir/$data_base/s".$support."_c".$confidence."_n".$min_rules."_m".$max_size."_l".$ranking_size.".".$fold.".log";
			my $best_log_file = "$output_dir/$data_base/best.$fold.log";
			system "cp $log_file $best_log_file";
		}
	}
=cut
}

sub run_lazy ($$$$$$$$$$$$)
{
	my ($data_base, $fold, $support, $confidence, $min_rules, $max_rule, $ranking_size, $log_file, $accuracy, $avg_patterns, $avg_rules, $avg_time) = @_;

	my $training_file = Common::GetTrainingFile ($data_base, $fold);
	my $testing_file = Common::GetTestingFile ($data_base, $fold);

	print "nice -n 15 $Common::AppLazy -i $training_file -t $testing_file -s $support -c $confidence -n $min_rules -m $max_rule -l $ranking_size 2&>$log_file\n";
	system "nice -n 15 $Common::AppLazy -i $training_file -t $testing_file -s $support -c $confidence -n $min_rules -m $max_rule -l $ranking_size 2&>$log_file";

	Common::GetRunResultFromLogFile ($log_file, $accuracy, $avg_patterns, $avg_rules, $avg_time);
}

sub save_output_file ($$$$$$$$$$)
{
	my ($data_base, $support, $confidence, $min_rules, $max_size, $ranking_size, $avg_time, $avg_patterns, $avg_rules, $accuracy) = @_;

	my $out_file = "$output_dir/$data_base/s".$support."_c".$confidence."_n".$min_rules."_m".$max_size."_l".$ranking_size.".out";

	open OUTPUT, ">$out_file";
	print OUTPUT "support [$support], confidence [$confidence], min_rules [$min_rules], max_size [$max_size], ranking_size [$ranking_size], avg_time [$avg_time], avg_patterns [$avg_patterns], avg_rules [$avg_rules], accuracy [$accuracy]\n";
	close OUTPUT;

	if ($accuracy > Common::GetBestAccuracy ('lazy', $data_base))
	{
		my $best_out_file = Common::GetBestOutputFile ('lazy', $data_base);

		print "cp $out_file $best_out_file\n";
		system "cp $out_file $best_out_file";

		if ($data_base ne 'average')
		{
			my $fold;

			for ($fold = 0; $fold < $Common::NumFolds; $fold++)
			{
				my $log_file = "$output_dir/$data_base/s".$support."_c".$confidence."_n".$min_rules."_m".$max_size."_l".$ranking_size.".".$fold.".log";
				my $best_log_file = "$output_dir/$data_base/best.$fold.log";
				system "cp $log_file $best_log_file";
			}
		}
	}
}
