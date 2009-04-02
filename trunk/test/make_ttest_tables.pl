#!/usr/bin/perl -w

use strict;
use Common;

sub get_best_results_for_application ($$);
sub make_ttest_tables_for_data_base ($);

my %best_runs_for_each_db_acc_hsh;

system "mkdir -p $Common::OutputDirTTests/";

my $data_base;

foreach $data_base (@Common::DataBases)
{
	print "base: $data_base\n";

	make_ttest_tables_for_data_base ($data_base);
}

exit;

###


sub make_ttest_tables_for_data_base ($)
{
	my $data_base = $_[0];

	get_best_results_for_application ('classifier_c', $data_base);
	get_best_results_for_application ('classifier_o', $data_base);
	get_best_results_for_application ('classifier_or', $data_base);

	my $ttest_R 	= "$Common::OutputDirTTests/ttest_${data_base}_best.R";
	my $ttest_dat_c = "$Common::OutputDirTTests/ttest_${data_base}_best_classifier_c.dat";
	my $ttest_dat_o = "$Common::OutputDirTTests/ttest_${data_base}_best_classifier_o.dat";
	my $ttest_out	= "$Common::OutputDirTTests/ttest_${data_base}_best.out";

	open OUTPUT, ">$ttest_R";
	print OUTPUT "c <- scan('$ttest_dat_c');\n";
	print OUTPUT "o <- scan('$ttest_dat_o');\n";
	print OUTPUT "t.test(c,o,var.equal=TRUE,var.paired=TRUE,conf.level=0.99);\n";
	close OUTPUT;

	system "R --no-restore --no-save --no-readline <$ttest_R 2>/dev/null | grep -A9 \"Two Sample t-test\" | grep -v \"data: \" >$ttest_out";
}

sub get_best_results_for_application ($$)
{
	my ($application, $data_base) = @_;

	print "application: $application\n";

	my $out_file = "$Common::OutputDirTTests/ttest_${data_base}_best_$application.dat";

	open OUTPUT, ">$out_file";

	my $fold;

	for ($fold = 0; $fold < $Common::NumFolds; $fold++)
	{
		my $log_file = Common::GetBestLogFile ($application, $data_base, $fold);

		my ($accuracy, $avg_patterns, $avg_rules, $avg_time);

		Common::GetRunResultFromLogFile ($log_file, \$accuracy, \$avg_patterns, \$avg_rules, \$avg_time);

#		$best_runs_for_each_db_acc_hsh{$data_base}{$application}{$fold} = $accuracy;

#		print OUTPUT $best_runs_for_each_db_acc_hsh{$data_base}{'classifier_c'}{$fold}."\t".$best_runs_for_each_db_acc_hsh{$data_base}{'classifier_o'}{$fold}."\n";
		print OUTPUT "$accuracy\n";
	}

	close OUTPUT;
}

#for app in `ls -1 output | grep classifier`; do
#	echo app: $app;
#done;
#grep average output/classifier_o/anneal.ac/best.0.log | sed -e 's/accuracy \[\(.*\)\], average patterns.*/\1/g'
