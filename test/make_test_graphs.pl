#!/usr/bin/perl -w

use strict;

use Parms;

my $LAZY_OUTPUT_DIR		= "output/lazy";
my $CLASSIFIER_C_OUTPUT_DIR	= "output/classifier_c";
my $CLASSIFIER_O_OUTPUT_DIR	= "output/classifier_o";
my $GRAPHS_OUTPUT_DIR		= "output/graphs";

my $GNUPLOT_APP			= "~/local/gnuplot/bin/gnuplot";

sub make_accuracy_histogram_graph ();
sub make_avg_patterns_histogram_graph ();
sub make_avg_rules_histogram_graph ();

sub make_ometric_acc_histogram_graph ($$$$$$$);

sub get_best_runs_for_data_base ($);
sub get_best_run_lazy ($);
sub get_best_run_classifier_c ($);
sub get_best_run_classifier_o ($);

sub get_ometric_runs_for_classifier_o ($$$$$$$$);
sub get_ometric_run_for_classifier_o ($$$$$$$$$);

my %accuracy_hsh;
my %avg_patterns_hsh;
my %avg_rules_hsh;

my %ometric_acc_hsh;
my %ometric_pat_hsh;
my %ometric_rul_hsh;

my $db;
my $i;

format OUTPUT_ACC =
@<<<<<<<<<<<<<<<<<<<@<<<<<<<<<<<<<<<<<<<@<<<<<<<<<<<<<<<<<<<@<<<<<<<<<<<<<<<<<<<
$db,$accuracy_hsh{$db}{'lazy'},$accuracy_hsh{$db}{'classifier_c'},$accuracy_hsh{$db}{'classifier_o'}
.

format OUTPUT_PAT =
@<<<<<<<<<<<<<<<<<<<@<<<<<<<<<<<<<<<<<<<@<<<<<<<<<<<<<<<<<<<@<<<<<<<<<<<<<<<<<<<
$db,$avg_patterns_hsh{$db}{'lazy'},$avg_patterns_hsh{$db}{'classifier_c'},$avg_patterns_hsh{$db}{'classifier_o'}
.

format OUTPUT_RUL =
@<<<<<<<<<<<<<<<<<<<@<<<<<<<<<<<<<<<<<<<@<<<<<<<<<<<<<<<<<<<@<<<<<<<<<<<<<<<<<<<
$db,$avg_rules_hsh{$db}{'lazy'},$avg_rules_hsh{$db}{'classifier_c'},$avg_rules_hsh{$db}{'classifier_o'}
.

format OUTPUT_OMETRIC_ACC =
@<<<<<<<<<<<<<<<<<<<@<<<<<<<<<<<<<<<<<<<@<<<<<<<<<<<<<<<<<<<@<<<<<<<<<<<<<<<<<<<@<<<<<<<<<<<<<<<<<<<
$db,$ometric_acc_hsh{$db}{'s'},$ometric_acc_hsh{$db}{'c'},$ometric_acc_hsh{$db}{'l'},$ometric_acc_hsh{$db}{'m'}
.

make_accuracy_histogram_graph ();
make_avg_patterns_histogram_graph ();
make_avg_rules_histogram_graph ();

my $support		= '0.1';
my $confidence		= '0.9';
my $min_num_rules	= '1';
my $max_num_rank_rules	= '1';
my $min_rule_len	= '1';
my $max_rule_len	= '1';
my $omode		= 'h';

make_ometric_acc_histogram_graph ($support, $confidence, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode);

exit;

###

sub get_best_run_lazy ($)
{
	my $data_base = $_[0];

	my $accuracy = 0;
	my $avg_patterns = 0;
	my $avg_rules = 0;

	my $out_file = "$LAZY_OUTPUT_DIR/$data_base/best.out";

	if (-e $out_file)
	{
		open INPUT, "<$out_file";

		$accuracy = <INPUT>;
		$avg_patterns = $accuracy;
		$avg_rules = $accuracy;

		chomp $accuracy;
		$accuracy =~ s/.*accuracy: (.*)$/$1/;

		chomp $avg_patterns;
		$avg_patterns =~ s/.*avg_patterns: (.*), avg_rules.*$/$1/;

		chomp $avg_rules;
		$avg_rules =~ s/.*avg_rules: (.*), accuracy.*$/$1/;

		close INPUT;
	}

	$accuracy_hsh{$data_base}{'lazy'} = $accuracy;
	$avg_patterns_hsh{$data_base}{'lazy'} = $avg_patterns;
	$avg_rules_hsh{$data_base}{'lazy'} = $avg_rules;
}

sub get_best_run_classifier_c ($)
{
	my $data_base = $_[0];

	my $accuracy = 0;
	my $avg_patterns = 0;
	my $avg_rules = 0;

	my $out_file = "$CLASSIFIER_C_OUTPUT_DIR/$data_base/best.out";

	if (-e $out_file)
	{
		open INPUT, "<$out_file";

		$accuracy = <INPUT>;
		$avg_patterns = $accuracy;
		$avg_rules = $accuracy;

		chomp $accuracy;
		$accuracy =~ s/.*accuracy: (.*)$/$1/;

		chomp $avg_patterns;
		$avg_patterns =~ s/.*avg_patterns: (.*), avg_rules.*$/$1/;

		chomp $avg_rules;
		$avg_rules =~ s/.*avg_rules: (.*), accuracy.*$/$1/;

		close INPUT;
	}

	$accuracy_hsh{$data_base}{'classifier_c'} = $accuracy;
	$avg_patterns_hsh{$data_base}{'classifier_c'} = $avg_patterns;
	$avg_rules_hsh{$data_base}{'classifier_c'} = $avg_rules;
}

sub get_best_run_classifier_o ($)
{
	my $data_base = $_[0];

	my $accuracy = 0;
	my $avg_patterns = 0;
	my $avg_rules = 0;

	my $out_file = "$CLASSIFIER_O_OUTPUT_DIR/$data_base/best.out";

	if (-e $out_file)
	{
		open INPUT, "<$out_file";

		$accuracy = <INPUT>;
		$avg_patterns = $accuracy;
		$avg_rules = $accuracy;

		chomp $accuracy;
		$accuracy =~ s/.*accuracy: (.*)$/$1/;

		chomp $avg_patterns;
		$avg_patterns =~ s/.*avg_patterns: (.*), avg_rules.*$/$1/;

		chomp $avg_rules;
		$avg_rules =~ s/.*avg_rules: (.*), accuracy.*$/$1/;

		close INPUT;
	}

	$accuracy_hsh{$data_base}{'classifier_o'} = $accuracy;
	$avg_patterns_hsh{$data_base}{'classifier_o'} = $avg_patterns;
	$avg_rules_hsh{$data_base}{'classifier_o'} = $avg_rules;
}

sub get_best_runs_for_data_base ($)
{
	get_best_run_lazy ($_[0]);
	get_best_run_classifier_c ($_[0]);
	get_best_run_classifier_o ($_[0]);
}

sub get_ometric_runs_for_classifier_o ($$$$$$$$)
{
	my ($data_base, $support, $confidence, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode) = @_;

	get_ometric_run_for_classifier_o ($data_base, $support, $confidence, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode, 's');
	get_ometric_run_for_classifier_o ($data_base, $support, $confidence, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode, 'c');
	get_ometric_run_for_classifier_o ($data_base, $support, $confidence, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode, 'l');
	get_ometric_run_for_classifier_o ($data_base, $support, $confidence, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode, 'm');
}

sub get_ometric_run_for_classifier_o ($$$$$$$$$)
{
	my ($data_base, $support, $confidence, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode, $ometric) = @_;

	my $accuracy = 0;
	my $avg_patterns = 0;
	my $avg_rules = 0;

	my $out_file = "$CLASSIFIER_O_OUTPUT_DIR/$data_base/s".$support."_c".$confidence."_n".$min_num_rules."_l".$max_num_rank_rules."_m".$min_rule_len."_a".$max_rule_len."_o".$omode."_e".$ometric.".out";

	if (-e $out_file)
	{
		open INPUT, "<$out_file";

		$accuracy = <INPUT>;
		$avg_patterns = $accuracy;
		$avg_rules = $accuracy;

		chomp $accuracy;
		$accuracy =~ s/.*accuracy: (.*)$/$1/;

		chomp $avg_patterns;
		$avg_patterns =~ s/.*avg_patterns: (.*), avg_rules.*$/$1/;

		chomp $avg_rules;
		$avg_rules =~ s/.*avg_rules: (.*), accuracy.*$/$1/;

		close INPUT;
	}

	$ometric_acc_hsh{$data_base}{$ometric} = $accuracy;
	$ometric_pat_hsh{$data_base}{$ometric} = $avg_patterns;
	$ometric_rul_hsh{$data_base}{$ometric} = $avg_rules;
}

sub make_accuracy_histogram_graph ()
{
	my $data_base;

	foreach $data_base (@Parms::DataBases)
	{
		print "base: $data_base\n";

		get_best_runs_for_data_base ($data_base);
	}

	my $acc_histogram_graph = 'histogram_acc';

	open OUTPUT_ACC, ">$GRAPHS_OUTPUT_DIR/$acc_histogram_graph.dat";

	print OUTPUT_ACC "data_file           lazy                classifier_c        classifier_o        \n";

	foreach $data_base (@Parms::DataBases)
	{
		$db = $data_base;
		write OUTPUT_ACC;
	}

	close OUTPUT_ACC;

	my $gnu_file = "$GRAPHS_OUTPUT_DIR/$acc_histogram_graph.gnu";
	my $eps_file = "$GRAPHS_OUTPUT_DIR/$acc_histogram_graph.eps";
	my $dat_file = "$GRAPHS_OUTPUT_DIR/$acc_histogram_graph.dat";

	open OUTPUT, ">$gnu_file";

	print OUTPUT "set term postscript eps enhanced color\n";
	print OUTPUT "set output '$eps_file'\n";
	print OUTPUT "set boxwidth 0.9 absolute\n";
	print OUTPUT "set style fill solid 1.00 border -1\n";
	print OUTPUT "set style histogram clustered gap 1 title offset character 0, 0, 0\n";
	print OUTPUT "set datafile missing '-'\n";
	print OUTPUT "set style data histograms\n";
	print OUTPUT "set xtics border in scale 1,0.5 nomirror rotate by -45 offset character 0, 0, 0 \n";
	print OUTPUT "set xtics (\"anneal.ac\" 0,";

	$i = 0;

	foreach $data_base (@Parms::DataBases)
	{
		print OUTPUT ", " if $i != 0;
		print OUTPUT "\"$data_base\" $i";
		$i++;
	}

	print OUTPUT ")\n";
	print OUTPUT "set title \"Accuracy Histogram\"\n";
	print OUTPUT "set xlabel \"Data Sets\"\n";
	print OUTPUT "set ylabel \"Accuracy\"\n";
	print OUTPUT "set yrange [0:1] noreverse nowriteback\n";
	print OUTPUT "plot '$dat_file' using 2:xtic(1) ti col, '' u 3 ti col, '' u 4 ti col";
	close OUTPUT;

	system "$GNUPLOT_APP $gnu_file";
}

sub make_avg_patterns_histogram_graph ()
{
	my $data_base;

	foreach $data_base (@Parms::DataBases)
	{
		print "base: $data_base\n";

		get_best_runs_for_data_base ($data_base);
	}

	my $pat_histogram_graph = 'histogram_pat';

	open OUTPUT_PAT, ">$GRAPHS_OUTPUT_DIR/$pat_histogram_graph.dat";

	print OUTPUT_PAT "data_file           lazy                classifier_c        classifier_o        \n";

	foreach $data_base (@Parms::DataBases)
	{
		$db = $data_base;
		write OUTPUT_PAT;
	}

	close OUTPUT_PAT;

	my $gnu_file = "$GRAPHS_OUTPUT_DIR/$pat_histogram_graph.gnu";
	my $eps_file = "$GRAPHS_OUTPUT_DIR/$pat_histogram_graph.eps";
	my $dat_file = "$GRAPHS_OUTPUT_DIR/$pat_histogram_graph.dat";

	open OUTPUT, ">$gnu_file";

	print OUTPUT "set term postscript eps enhanced color\n";
	print OUTPUT "set output '$eps_file'\n";
	print OUTPUT "set boxwidth 0.9 absolute\n";
	print OUTPUT "set style fill solid 1.00 border -1\n";
	print OUTPUT "set style histogram clustered gap 1 title offset character 0, 0, 0\n";
	print OUTPUT "set datafile missing '-'\n";
	print OUTPUT "set style data histograms\n";
	print OUTPUT "set xtics border in scale 1,0.5 nomirror rotate by -45 offset character 0, 0, 0 \n";
	print OUTPUT "set xtics (\"anneal.ac\" 0,";

	$i = 0;

	foreach $data_base (@Parms::DataBases)
	{
		print OUTPUT ", " if $i != 0;
		print OUTPUT "\"$data_base\" $i";
		$i++;
	}

	print OUTPUT ")\n";
	print OUTPUT "set title \"Patterns Average Histogram\"\n";
	print OUTPUT "set xlabel \"Data Sets\"\n";
	print OUTPUT "set ylabel \"Patterns Average\"\n";
#	print OUTPUT "set yrange [0:10000] noreverse nowriteback\n";
	print OUTPUT "plot '$dat_file' using 2:xtic(1) ti col, '' u 3 ti col, '' u 4 ti col";
	close OUTPUT;

	system "$GNUPLOT_APP $gnu_file";
}

sub make_avg_rules_histogram_graph ()
{
	my $data_base;

	foreach $data_base (@Parms::DataBases)
	{
		print "base: $data_base\n";

		get_best_runs_for_data_base ($data_base);
	}

	my $pat_histogram_graph = 'histogram_rul';

	open OUTPUT_RUL, ">$GRAPHS_OUTPUT_DIR/$pat_histogram_graph.dat";

	print OUTPUT_RUL "data_file           lazy                classifier_c        classifier_o        \n";

	foreach $data_base (@Parms::DataBases)
	{
		$db = $data_base;
		write OUTPUT_RUL;
	}

	close OUTPUT_RUL;

	my $gnu_file = "$GRAPHS_OUTPUT_DIR/$pat_histogram_graph.gnu";
	my $eps_file = "$GRAPHS_OUTPUT_DIR/$pat_histogram_graph.eps";
	my $dat_file = "$GRAPHS_OUTPUT_DIR/$pat_histogram_graph.dat";

	open OUTPUT, ">$gnu_file";

	print OUTPUT "set term postscript eps enhanced color\n";
	print OUTPUT "set output '$eps_file'\n";
	print OUTPUT "set boxwidth 0.9 absolute\n";
	print OUTPUT "set style fill solid 1.00 border -1\n";
	print OUTPUT "set style histogram clustered gap 1 title offset character 0, 0, 0\n";
	print OUTPUT "set datafile missing '-'\n";
	print OUTPUT "set style data histograms\n";
	print OUTPUT "set xtics border in scale 1,0.5 nomirror rotate by -45 offset character 0, 0, 0 \n";
	print OUTPUT "set xtics (\"anneal.ac\" 0,";

	$i = 0;

	foreach $data_base (@Parms::DataBases)
	{
		print OUTPUT ", " if $i != 0;
		print OUTPUT "\"$data_base\" $i";
		$i++;
	}

	print OUTPUT ")\n";
	print OUTPUT "set title \"Rules Average Histogram\"\n";
	print OUTPUT "set xlabel \"Data Sets\"\n";
	print OUTPUT "set ylabel \"Rules Average\"\n";
#	print OUTPUT "set yrange [0:10000] noreverse nowriteback\n";
	print OUTPUT "plot '$dat_file' using 2:xtic(1) ti col, '' u 3 ti col, '' u 4 ti col";
	close OUTPUT;

	system "$GNUPLOT_APP $gnu_file";
}

sub make_ometric_acc_histogram_graph ($$$$$$$)
{
	my ($support, $confidence, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode) = @_;

	my $data_base;

	foreach $data_base (@Parms::DataBases)
	{
		print "base: $data_base\n";

		get_ometric_runs_for_classifier_o ($data_base, $support, $confidence, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode);
	}

	my $acc_histogram_graph = 'histogram_ometric_acc';

	open OUTPUT_OMETRIC_ACC, ">$GRAPHS_OUTPUT_DIR/$acc_histogram_graph.dat";

	print OUTPUT_OMETRIC_ACC "data_file           similarity          coverage            class_coverage      pair_class_coverage \n";

	foreach $data_base (@Parms::DataBases)
	{
		$db = $data_base;
		write OUTPUT_OMETRIC_ACC;
	}

	close OUTPUT_OMETRIC_ACC;

	my $gnu_file = "$GRAPHS_OUTPUT_DIR/$acc_histogram_graph.gnu";
	my $eps_file = "$GRAPHS_OUTPUT_DIR/$acc_histogram_graph.eps";
	my $dat_file = "$GRAPHS_OUTPUT_DIR/$acc_histogram_graph.dat";

	open OUTPUT, ">$gnu_file";

	print OUTPUT "set term postscript eps enhanced color\n";
	print OUTPUT "set output '$eps_file'\n";
	print OUTPUT "set boxwidth 0.9 absolute\n";
	print OUTPUT "set style fill solid 1.00 border -1\n";
	print OUTPUT "set style histogram clustered gap 1 title offset character 0, 0, 0\n";
	print OUTPUT "set datafile missing '-'\n";
	print OUTPUT "set style data histograms\n";
	print OUTPUT "set xtics border in scale 1,0.5 nomirror rotate by -45 offset character 0, 0, 0 \n";
	print OUTPUT "set xtics (\"anneal.ac\" 0,";

	$i = 0;

	foreach $data_base (@Parms::DataBases)
	{
		print OUTPUT ", " if $i != 0;
		print OUTPUT "\"$data_base\" $i";
		$i++;
	}

	print OUTPUT ")\n";
	print OUTPUT "set title \"Orthogonality Metrics Accuracy Histogram\"\n";
	print OUTPUT "set xlabel \"Data Sets\"\n";
	print OUTPUT "set ylabel \"Accuracy\"\n";
	print OUTPUT "set yrange [0:1] noreverse nowriteback\n";
	print OUTPUT "plot '$dat_file' using 2:xtic(1) ti col, '' u 3 ti col, '' u 4 ti col, '' u 5 ti col";
	close OUTPUT;

	system "$GNUPLOT_APP $gnu_file";
}
