#!/usr/bin/perl -w

use strict;
use Common;

sub make_app_histogram_graphs ();
sub make_ometric_histogram_graphs ($$$$$$$);

sub get_best_runs_for_data_base ($);
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

my $support		= '0.1';
my $confidence		= '0.9';
my $min_num_rules	= '1';
my $max_num_rank_rules	= '1';
my $min_rule_len	= '1';
my $max_rule_len	= '1';
my $omode		= 'h';

system "mkdir -p $Common::OutputDirGraphs/";

make_app_histogram_graphs ();
#make_ometric_histogram_graphs ($support, $confidence, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode);


exit;

###

sub get_best_run_for_application ($$)
{
	my ($application, $data_base) = @_;

	my $RunResult = Common::GetBestRunResult ($application, $data_base);

	$accuracy_hsh{$data_base}{$application}		= $RunResult->{ACCURACY};
	$avg_patterns_hsh{$data_base}{$application}	= $RunResult->{AVG_PATTERNS};
	$avg_rules_hsh{$data_base}{$application}	= $RunResult->{AVG_RULES};

	$accuracy_hsh{'average'}{$application}		+= $RunResult->{ACCURACY};
	$avg_patterns_hsh{'average'}{$application}	+= $RunResult->{AVG_PATTERNS};
	$avg_rules_hsh{'average'}{$application}		+= $RunResult->{AVG_RULES};
}

sub get_best_runs_for_data_base ($)
{
	get_best_run_for_application ('lazy', $_[0]);
	get_best_run_for_application ('classifier_c', $_[0]);
	get_best_run_for_application ('classifier_o', $_[0]);
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

	my $RunResult = Common::GetClassifierORunResult ($_[0], $_[1], $_[2], $_[3], $_[4], $_[5], $_[6], $_[7], $_[8]);

	$ometric_acc_hsh{$data_base}{$ometric} = $RunResult->{ACCURACY};
	$ometric_pat_hsh{$data_base}{$ometric} = $RunResult->{AVG_PATTERNS};
	$ometric_rul_hsh{$data_base}{$ometric} = $RunResult->{AVG_RULES};
}

sub make_app_histogram_graphs ()
{
	print "make_app_histogram_graphs ()\n";

	my $data_base;

	foreach $data_base (@Common::DataBases)
	{
		print "base: $data_base\n";

		get_best_runs_for_data_base ($data_base);
	}

	$accuracy_hsh{'average'}{'lazy'}		/= scalar @Common::DataBases;
	$accuracy_hsh{'average'}{'classifier_c'}	/= scalar @Common::DataBases;
	$accuracy_hsh{'average'}{'classifier_o'}	/= scalar @Common::DataBases;

	$avg_patterns_hsh{'average'}{'lazy'}		/= scalar @Common::DataBases;
	$avg_patterns_hsh{'average'}{'classifier_c'}	/= scalar @Common::DataBases;
	$avg_patterns_hsh{'average'}{'classifier_o'}	/= scalar @Common::DataBases;

	$avg_rules_hsh{'average'}{'lazy'}		/= scalar @Common::DataBases;
	$avg_rules_hsh{'average'}{'classifier_c'}	/= scalar @Common::DataBases;
	$avg_rules_hsh{'average'}{'classifier_o'}	/= scalar @Common::DataBases;

	Common::MakeAppHistogramGraph ('Accuracy Histogram', 'Data Sets', 'Accuracy', 'histogram_acc', \%accuracy_hsh);
	Common::MakeAppHistogramGraph ('Patterns Average Histogram', 'Data Sets', 'Patterns Average', 'histogram_pat', \%avg_patterns_hsh);
	Common::MakeAppHistogramGraph ('Rules Average Histogram', 'Data Sets', 'Rules Average', 'histogram_rul', \%avg_rules_hsh);
}

sub make_ometric_histogram_graphs ($$$$$$$)
{
	print "make_ometric_histogram_graphs ()\n";

	my ($support, $confidence, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode) = @_;

	my $data_base;

	foreach $data_base (@Common::DataBases)
	{
		print "base: $data_base\n";

		get_ometric_runs_for_classifier_o ($data_base, $support, $confidence, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode);
	}

	Common::MakeOMetricHistogramGraph ('Orthogonality Metrics Accuracy Histogram', 'Data Sets', 'Accuracy', 'histogram_ometric_acc', \%ometric_acc_hsh);
	Common::MakeOMetricHistogramGraph ('Orthogonality Metrics Patterns Average Histogram', 'Data Sets', 'Patterns Average', 'histogram_ometric_pat', \%ometric_pat_hsh);
	Common::MakeOMetricHistogramGraph ('Orthogonality Metrics Rules Average Histogram', 'Data Sets', 'Rules Average', 'histogram_ometric_rul', \%ometric_rul_hsh);
}
