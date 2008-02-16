#!/usr/bin/perl -w

use strict;
use Common;

sub make_test_lazy ($$$$$$);
sub run_lazy ($$$$$$$$$$$);

my $data_base;

foreach $data_base (@Common::DataBases)
{
	system "mkdir -p $Common::OutputDirLazy/$data_base";
}

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
					foreach $data_base (@Common::DataBases)
					{
						make_test_lazy ($data_base, $Common::LazySupports[$s], $Common::Confidences[$c], $Common::MinNumRules[$n], $Common::LazyMaxSizes[$m], $Common::MaxNumRankRules[$l]);
					}
				}
			}
		}
	}
}

sub make_test_lazy ($$$$$$)
{
	my ($data_base, $support, $confidence, $min_rules, $max_size, $ranking_size) = @_;

	my $accuracy		= 0;
	my $avg_patterns	= 0;
	my $avg_rules		= 0;
	my $fold;

	for ($fold = 0; $fold < $Common::NumFolds; $fold++)
	{
		my $log_file = "$Common::OutputDirLazy/$data_base/s".$support."_c".$confidence."_n".$min_rules."_m".$max_size."_l".$ranking_size.".".$fold.".log";

		my ($acc, $pat, $rul);

		run_lazy ($data_base, $fold, $support, $confidence, $min_rules, $max_size, $ranking_size, $log_file, \$acc, \$pat, \$rul);

		$accuracy	+= $acc;
		$avg_patterns	+= $pat;
		$avg_rules	+= $rul;
	}

	$accuracy	/= $Common::NumFolds;
	$avg_patterns	/= $Common::NumFolds;
	$avg_rules	/= $Common::NumFolds;

	print "accuracy [$accuracy], avg_patterns [$avg_patterns], avg_rules [$avg_rules]\n";

	system "mkdir -p $Common::OutputDirLazy/$data_base/";

	my $out_file = "$Common::OutputDirLazy/$data_base/s".$support."_c".$confidence."_n".$min_rules."_m".$max_size."_l".$ranking_size.".out";

	open OUTPUT, ">$out_file";
	print OUTPUT "support [$support], confidence [$confidence], min_rules [$min_rules], max_size [$max_size], ranking_size [$ranking_size], avg_patterns [$avg_patterns], avg_rules [$avg_rules], accuracy [$accuracy]\n";
	close OUTPUT;

	if ($accuracy > Common::GetBestAccuracy ('lazy', $data_base))
	{
		my $best_out_file = Common::GetBestOutputFile ('lazy', $data_base);

		system "cp $out_file $best_out_file";

		for ($fold = 0; $fold < $Common::NumFolds; $fold++)
		{
			my $log_file = "$Common::OutputDirLazy/$data_base/s".$support."_c".$confidence."_n".$min_rules."_m".$max_size."_l".$ranking_size.".".$fold.".log";
			my $best_log_file = "$Common::OutputDirLazy/$data_base/best.$fold.log";
			system "cp $log_file $best_log_file";
		}
	}
}

sub run_lazy ($$$$$$$$$$$)
{
	my ($data_base, $fold, $support, $confidence, $min_rules, $max_rule, $ranking_size, $log_file, $accuracy, $avg_patterns, $avg_rules) = @_;

	my $training_file = Common::GetTrainingFile ($data_base, $fold);
	my $testing_file = Common::GetTestingFile ($data_base, $fold);

	print "nice -n 15 $Common::AppLazy -i $training_file -t $testing_file -s $support -c $confidence -n $min_rules -m $max_rule -l $ranking_size 2&>$log_file\n";
	system "nice -n 15 $Common::AppLazy -i $training_file -t $testing_file -s $support -c $confidence -n $min_rules -m $max_rule -l $ranking_size 2&>$log_file";

	Common::GetRunResultFromLogFile ($log_file, $accuracy, $avg_patterns, $avg_rules);
}
