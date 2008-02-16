#!/usr/bin/perl -w

use strict;

use Common;

sub make_app_comparison_table ();
sub make_best_runs_table ();

sub get_comparisons_for_data_base ($$);

my @comparison_array;

system "mkdir -p $Common::OutputDirTables/";

make_app_comparison_table ();
make_best_runs_table ();


exit;

###

sub make_app_comparison_table ()
{
	print "make_app_comparison_table ()\n";

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

sub make_best_runs_table ()
{
	print "make_best_runs_table ()\n";

	make_best_runs_table_for_lazy ();
	make_best_runs_table_for_classifier_c ();
	make_best_runs_table_for_classifier_o ();
}

sub make_best_runs_table_for_lazy ()
{
	my $db;

	my ($support, $confidence, $min_rules, $max_size, $ranking_size, $avg_patterns, $avg_rules, $accuracy);

format OUTPUT_BEST_RUNS_LAZY =
@<<<<<<<<<<<<<<<<<<<@<<<<<<<<<<<<<@<<<<<<<<<<<<<@<<<<<<<<<<<<<@<<<<<<<<<<<<<@<<<<<<<<<<<<<@<<<<<<<<<<<<<@<<<<<<<<<<<<<@<<<<<<<<<<<<<
$db,                $support,     $confidence,  $min_rules,   $max_size,    $ranking_size, $avg_patterns, $avg_rules, $accuracy
.

	my $out_file = "$Common::OutputDirTables/table_best_runs_lazy.out";

	open OUTPUT_BEST_RUNS_LAZY, ">$out_file";
	print OUTPUT_BEST_RUNS_LAZY "data set            support       confidence    min_rules     max_size      ranking_size  avg_patterns  avg_rules     accuracy\n";

	my $data_base;

	foreach $data_base (@Common::DataBases)
	{
		print "base: $data_base\n";

		$db = $data_base;

		my $file_best_run = Common::GetBestOutputFile ('lazy', $data_base);

		open INPUT, "<$file_best_run";

		$support = <INPUT>;

		chomp $support;

		$confidence	= $support;
		$min_rules	= $support;
		$max_size	= $support;
		$ranking_size	= $support;
		$avg_patterns	= $support;
		$avg_rules	= $support;
		$accuracy	= $support;

		$support	=~ s/.*support \[([^\]]*)\].*/$1/;
		$confidence	=~ s/.*confidence \[([^\]]*)\].*/$1/;
		$min_rules	=~ s/.*min_rules \[([^\]]*)\].*/$1/;
		$max_size	=~ s/.*max_size \[([^\]]*)\].*/$1/;
		$ranking_size	=~ s/.*ranking_size \[([^\]]*)\].*/$1/;
		$avg_patterns	=~ s/.*avg_patterns \[([^\]]*)\].*/$1/;
		$avg_rules	=~ s/.*avg_rules \[([^\]]*)\].*/$1/;
		$accuracy	=~ s/.*accuracy \[([^\]]*)\].*/$1/;

		write OUTPUT_BEST_RUNS_LAZY;
	}

	close OUTPUT_BEST_RUNS_LAZY;
}

sub make_best_runs_table_for_classifier_c ()
{
	my $db;

	my ($support, $confidence, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $avg_patterns, $avg_rules, $accuracy);

format OUTPUT_BEST_RUNS_CLASSC =
@<<<<<<<<<<<<<<<<<<<@<<<<<<<<<<<<<@<<<<<<<<<<<<<@<<<<<<<<<<<<<@<<<<<<<<<<<<<<<<<<@<<<<<<<<<<<<<@<<<<<<<<<<<<<@<<<<<<<<<<<<<@<<<<<<<<<<<<<@<<<<<<<<<<<<<
$db,                $support,     $confidence,  $min_num_rules,$max_num_rank_rules,$min_rule_len,$max_rule_len,$avg_patterns,$avg_rules,$accuracy
.

	my $out_file = "$Common::OutputDirTables/table_best_runs_classc.out";

	open OUTPUT_BEST_RUNS_CLASSC, ">$out_file";
	print OUTPUT_BEST_RUNS_CLASSC "data set            support       confidence    min_num_rules max_num_rank_rules min_rule_len   max_rule_len avg_patterns  avg_rules     accuracy      \n";

	my $data_base;

	foreach $data_base (@Common::DataBases)
	{
		print "base: $data_base\n";

		$db = $data_base;

		my $file_best_run = Common::GetBestOutputFile ('classifier_c', $data_base);

		open INPUT, "<$file_best_run";

		$support = <INPUT>;

		chomp $support;

		$confidence		= $support;
		$min_num_rules		= $support;
		$max_num_rank_rules	= $support;
		$min_rule_len		= $support;
		$max_rule_len		= $support;
		$avg_patterns		= $support;
		$avg_rules		= $support;
		$accuracy		= $support;

		$support		=~ s/.*support \[([^\]]*)\].*/$1/;
		$confidence		=~ s/.*confidence \[([^\]]*)\].*/$1/;
		$min_num_rules		=~ s/.*min_num_rules \[([^\]]*)\].*/$1/;
		$max_num_rank_rules	=~ s/.*max_num_rank_rules \[([^\]]*)\].*/$1/;
		$min_rule_len		=~ s/.*min_rule_len \[([^\]]*)\].*/$1/;
		$max_rule_len		=~ s/.*max_rule_len \[([^\]]*)\].*/$1/;
		$avg_patterns		=~ s/.*avg_patterns \[([^\]]*)\].*/$1/;
		$avg_rules		=~ s/.*avg_rules \[([^\]]*)\].*/$1/;
		$accuracy		=~ s/.*accuracy \[([^\]]*)\],*/$1/;

		write OUTPUT_BEST_RUNS_CLASSC;
	}

	close OUTPUT_BEST_RUNS_CLASSC;
}

sub make_best_runs_table_for_classifier_o ()
{
	my $db;

	my ($support, $confidence, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode, $ometric, $avg_patterns, $avg_rules, $accuracy);

format OUTPUT_BEST_RUNS_CLASSO =
@<<<<<<<<<<<<<<<<<<<@<<<<<<<<<<<<<@<<<<<<<<<<<<<@<<<<<<<<<<<<<@<<<<<<<<<<<<<<<<<<@<<<<<<<<<<<<<@<<<<<<<<<<<<<@<<<<<<<<<<<<<@<<<<<<<<<<<<<@<<<<<<<<<<<<<@<<<<<<<<<<<<<@<<<<<<<<<<<<<
$db,                $support,     $confidence,  $min_num_rules,$max_num_rank_rules,$min_rule_len,$max_rule_len,$omode,$ometric,$avg_patterns,$avg_rules,$accuracy
.

	my $out_file = "$Common::OutputDirTables/table_best_runs_classo.out";

	open OUTPUT_BEST_RUNS_CLASSO, ">$out_file";
	print OUTPUT_BEST_RUNS_CLASSO "data set            support       confidence    min_num_rules max_num_rank_rules min_rule_len   max_rule_len omode         ometric       avg_patterns  avg_rules     accuracy      \n";

	my $data_base;

	foreach $data_base (@Common::DataBases)
	{
		print "base: $data_base\n";

		$db = $data_base;

		my $file_best_run = Common::GetBestOutputFile ('classifier_o', $data_base);

		open INPUT, "<$file_best_run";

		$support = <INPUT>;

		chomp $support;

		$confidence		= $support;
		$min_num_rules		= $support;
		$max_num_rank_rules	= $support;
		$min_rule_len		= $support;
		$max_rule_len		= $support;
		$omode			= $support;
		$ometric		= $support;
		$avg_patterns		= $support;
		$avg_rules		= $support;
		$accuracy		= $support;

		$support		=~ s/.*support \[([^\]]*)\].*/$1/;
		$confidence		=~ s/.*confidence \[([^\]]*)\].*/$1/;
		$min_num_rules		=~ s/.*min_num_rules \[([^\]]*)\]max_num_rank_rules.*/$1/;
		$max_num_rank_rules	=~ s/.*max_num_rank_rules \[([^\]]*)\]min_rule_len.*/$1/;
		$min_rule_len		=~ s/.*min_rule_len \[([^\]]*)\].*/$1/;
		$max_rule_len		=~ s/.*max_rule_len \[([^\]]*)\].*/$1/;
		$omode			=~ s/.*omode \[([^\]]*)\].*/$1/;
		$ometric		=~ s/.*ometric \[([^\]]*)\].*/$1/;
		$avg_patterns		=~ s/.*avg_patterns \[([^\]]*)\].*/$1/;
		$avg_rules		=~ s/.*avg_rules \[([^\]]*)\].*/$1/;
		$accuracy		=~ s/.*accuracy \[([^\]]*)\].*/$1/;

		write OUTPUT_BEST_RUNS_CLASSO;
	}

	close OUTPUT_BEST_RUNS_CLASSO;
}

sub get_comparisons_for_data_base ($$)
{
	my ($data_base, $comparison_array) = @_;

	my $fold;

	for ($fold = 0; $fold < $Common::NumFolds; $fold++)
	{
		my $log_file_lazy		= Common::GetBestLogFile ('lazy', $data_base, $fold);
		my $log_file_classifier_c	= Common::GetBestLogFile ('classifier_c', $data_base, $fold);
		my $log_file_classifier_o	= Common::GetBestLogFile ('classifier_o', $data_base, $fold);

		print "lazy: $log_file_lazy\n";
		print "classifier_c: $log_file_classifier_c\n";
		print "classifier_o: $log_file_classifier_o\n";

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

			$class_correct		=~ s/.*class \[([^\]]*)\].*/$1/;
			$class_lazy		=~ s/.*guess \[([^\]]*)\].*/$1/;
			$class_classifier_c	=~ s/.*guess \[([^\]]*)\].*/$1/;
			$class_classifier_o	=~ s/.*guess \[([^\]]*)\].*/$1/;

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
}
