#!/usr/bin/perl -w

use strict;

use Common;

sub make_app_comparison_table ();
sub get_comparisons_for_data_base ($$);

my @comparison_array;

make_app_comparison_table ();


exit;

###

sub make_app_comparison_table ()
{
	print "make_app_cpmparison_table ()\n";

	my $db;

	my ($y_lazy_y_classo, $y_lazy_n_classo, $n_lazy_y_classo, $n_lazy_n_classo, $y_lazy_y_classc, $y_lazy_n_classc, $n_lazy_y_classc, $n_lazy_n_classc, $y_classo_y_classc, $y_classo_n_classc, $n_classo_y_classc, $n_classo_n_classc);

format OUTPUT =
@<<<<<<<<<<<<<<<<<<<@<<<<<<<<<<<<<@<<<<<<<<<<<<<@<<<<<<<<<<<<<@<<<<<<<<<<<<<@<<<<<<<<<<<<<@<<<<<<<<<<<<<@<<<<<<<<<<<<<@<<<<<<<<<<<<<@<<<<<<<<<<<<<<<@<<<<<<<<<<<<<<<@<<<<<<<<<<<<<<<@<<<<<<<<<<<<<<<
$db, $y_lazy_y_classo, $y_lazy_n_classo, $n_lazy_y_classo, $n_lazy_n_classo, $y_lazy_y_classc, $y_lazy_n_classc, $n_lazy_y_classc, $n_lazy_n_classc, $y_classo_y_classc, $y_classo_n_classc, $n_classo_y_classc, $n_classo_n_classc
.

	my $out_file = "$Common::OutputDirTables/comparison.out";

	open OUTPUT, ">$out_file";
	print OUTPUT "data set            ylazy_yclasso ylazy_nclasso nlazy_yclasso nlazy_nclasso ylazy_yclassc ylazy_nclassc nlazy_yclassc nlazy_nclassc yclasso_yclassc yclasso_nclassc nclasso_yclassc nclasso_nclassc\n";

	my $data_base;

	foreach $data_base (@Common::DataBases)
	{
		print "base: $data_base\n";

		$db = $data_base;

		my @comparison_array;

		get_comparisons_for_data_base ($data_base, \@comparison_array);

		my $i;

		$y_lazy_y_classo = 0;
		$y_lazy_n_classo = 0;
		$n_lazy_y_classo = 0;
		$n_lazy_n_classo = 0;
		$y_lazy_y_classc = 0;
		$y_lazy_n_classc = 0;
		$n_lazy_y_classc = 0;
		$n_lazy_n_classc = 0;
		$y_classo_y_classc = 0;
		$y_classo_n_classc = 0;
		$n_classo_y_classc = 0;
		$n_classo_n_classc = 0;

		for ($i = 0; $i < scalar @comparison_array; $i++)
		{
			my $correct_lazy	= 0;
			my $correct_classc	= 0;
			my $correct_classo	= 0;

			$correct_lazy	= 1 if $comparison_array[$i]->{CLASS_CORRECT} eq $comparison_array[$i]->{CLASS_LAZY};
			$correct_classc	= 1 if $comparison_array[$i]->{CLASS_CORRECT} eq $comparison_array[$i]->{CLASS_CLASSIFIER_C};
			$correct_classo	= 1 if $comparison_array[$i]->{CLASS_CORRECT} eq $comparison_array[$i]->{CLASS_CLASSIFIER_O};

			$y_lazy_y_classo++	if $correct_lazy && $correct_classo;
			$y_lazy_n_classo++	if $correct_lazy && ! $correct_classo;
			$n_lazy_y_classo++	if ! $correct_lazy && $correct_classo;
			$n_lazy_n_classo++	if ! $correct_lazy && ! $correct_classo;

			$y_lazy_y_classc++	if $correct_lazy && $correct_classc;
			$y_lazy_n_classc++	if $correct_lazy && ! $correct_classc;
			$n_lazy_y_classc++	if ! $correct_lazy && $correct_classc;
			$n_lazy_n_classc++	if ! $correct_lazy && ! $correct_classc;

			$y_classo_y_classc++	if $correct_classo && $correct_classc;
			$y_classo_n_classc++	if $correct_classo && ! $correct_classc;
			$n_classo_y_classc++	if ! $correct_classo && $correct_classc;
			$n_classo_n_classc++	if ! $correct_classo && ! $correct_classc;
		}

		write OUTPUT;
	}

	close OUTPUT;
}

sub get_comparisons_for_data_base ($$)
{
	my ($data_base, $comparison_array) = @_;

	my $log_file_lazy		= Common::GetBestLogFile ('lazy', $data_base);
	my $log_file_classifier_c	= Common::GetBestLogFile ('classifier_c', $data_base);
	my $log_file_classifier_o	= Common::GetBestLogFile ('classifier_o', $data_base);


	open LOG_FILE_LAZY, "<$log_file_lazy";
	open LOG_FILE_CLASSIFIER_C, "<$log_file_classifier_c";
	open LOG_FILE_CLASSIFIER_O, "<$log_file_classifier_o";

	while (<LOG_FILE_LAZY>)
	{
		my $test_lazy		= $_;
		my $test_classifier_c	= <LOG_FILE_CLASSIFIER_C>;
		my $test_classifier_o	= <LOG_FILE_CLASSIFIER_O>;

		my $class_correct	= $test_lazy;
		my $class_lazy		= $test_lazy;
		my $class_classifier_c	= $test_classifier_c;
		my $class_classifier_o	= $test_classifier_o;

		$class_correct =~ s/.*class \[(.*)\], guess.*/$1/;
		$class_lazy =~ s/.*guess \[(.*)\], correct.*/$1/;
		$class_classifier_c =~ s/.*guess \[(.*)\], correct.*/$1/;
		$class_classifier_o =~ s/.*guess \[(.*)\], correct.*/$1/;

		my $comparison = {
			CLASS_CORRECT		=> $class_correct,
			CLASS_LAZY		=> $class_lazy,
			CLASS_CLASSIFIER_C	=> $class_classifier_c,
			CLASS_CLASSIFIER_O	=> $class_classifier_o
		};

		push @{$comparison_array}, $comparison;
	}

	close LOG_FILE_CLASSIFIER_O;
	close LOG_FILE_CLASSIFIER_C;
	close LOG_FILE_LAZY;
}
