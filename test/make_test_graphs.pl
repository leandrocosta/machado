#!/usr/bin/perl -w

use strict;
use Common;

sub make_best_app_histogram_graphs ();
sub make_avg_app_histogram_graphs ();
sub make_ometric_histogram_graphs ($$$$$$$);

sub get_best_runs_for_data_base ($);
sub get_ometric_runs_for_classifier_o ($$$$$$$$);
sub get_ometric_run_for_classifier_o ($$$$$$$$$$$$);

my %accuracy_hsh;
my %avg_patterns_hsh;
my %avg_rules_hsh;

my %ometric_acc_hsh;
my %ometric_pat_hsh;
my %ometric_rul_hsh;

my %avg_acc_hsh;
my %avg_pat_hsh;
my %avg_rul_hsh;

my $db;
my $i;

my $support		= '0.001';	#'0.1';
my $confidence		= '0.001';	#'0.9';
my $min_num_rules	= '1';
my $max_num_rank_rules	= '100';	#'1';
my $min_rule_len	= '1';
my $max_rule_len	= '1';
my $omode		= 'h';

system "mkdir -p $Common::OutputDirGraphs/";

make_best_app_histogram_graphs ();
make_avg_app_histogram_graphs ();
make_ometric_histogram_graphs ($support, $confidence, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode);


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
	get_best_run_for_application ('classifier_or', $_[0]);
}

sub get_ometric_runs_for_classifier_o ($$$$$$$$)
{
	my ($data_base, $support, $confidence, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode) = @_;

	get_ometric_run_for_classifier_o ($data_base, $support, $confidence, 'f', $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode, 's', 's', 'n');
	get_ometric_run_for_classifier_o ($data_base, $support, $confidence, 'f', $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode, 'c', 's', 'n');
	get_ometric_run_for_classifier_o ($data_base, $support, $confidence, 'f', $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode, 'l', 's', 'n');
	get_ometric_run_for_classifier_o ($data_base, $support, $confidence, 'f', $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode, 'm', 's', 'n');
}

sub get_ometric_run_for_classifier_o ($$$$$$$$$$$$)
{
	my ($data_base, $support, $confidence, $pattern_set, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode, $ometric, $omethod, $oordering) = @_;

	my $RunResult = Common::GetClassifierORunResult ($_[0], $_[1], $_[2], $_[3], $_[4], $_[5], $_[6], $_[7], $_[8], $_[9], $_[10], $_[11]);

	$ometric_acc_hsh{$data_base}{$ometric} = $RunResult->{ACCURACY};
	$ometric_pat_hsh{$data_base}{$ometric} = $RunResult->{AVG_PATTERNS};
	$ometric_rul_hsh{$data_base}{$ometric} = $RunResult->{AVG_RULES};

	$ometric_acc_hsh{'average'}{$ometric}	+= $RunResult->{ACCURACY};
	$ometric_pat_hsh{'average'}{$ometric}	+= $RunResult->{AVG_PATTERNS};
	$ometric_rul_hsh{'average'}{$ometric}	+= $RunResult->{AVG_RULES};
}

sub make_best_app_histogram_graphs ()
{
	print "make_best_app_histogram_graphs ()\n";

	my $data_base;

	foreach $data_base (@Common::DataBases)
	{
		print "base: $data_base\n";

		get_best_runs_for_data_base ($data_base);
	}

	$accuracy_hsh{'average'}{'lazy'}		/= scalar @Common::DataBases;
	$accuracy_hsh{'average'}{'classifier_c'}	/= scalar @Common::DataBases;
	$accuracy_hsh{'average'}{'classifier_o'}	/= scalar @Common::DataBases;
	$accuracy_hsh{'average'}{'classifier_or'}	/= scalar @Common::DataBases;

	$avg_patterns_hsh{'average'}{'lazy'}		/= scalar @Common::DataBases;
	$avg_patterns_hsh{'average'}{'classifier_c'}	/= scalar @Common::DataBases;
	$avg_patterns_hsh{'average'}{'classifier_o'}	/= scalar @Common::DataBases;
	$avg_patterns_hsh{'average'}{'classifier_or'}	/= scalar @Common::DataBases;

	$avg_rules_hsh{'average'}{'lazy'}		/= scalar @Common::DataBases;
	$avg_rules_hsh{'average'}{'classifier_c'}	/= scalar @Common::DataBases;
	$avg_rules_hsh{'average'}{'classifier_o'}	/= scalar @Common::DataBases;
	$avg_rules_hsh{'average'}{'classifier_or'}	/= scalar @Common::DataBases;

	Common::MakeAppHistogramGraph ('Accuracy Histogram', 'Data Sets', 'Accuracy', 'histogram_acc', \%accuracy_hsh, 0);
	Common::MakeAppHistogramGraph ('Patterns Average Histogram', 'Data Sets', 'Patterns Average', 'histogram_pat', \%avg_patterns_hsh, 1);
	Common::MakeAppHistogramGraph ('Rules Average Histogram', 'Data Sets', 'Rules Average', 'histogram_rul', \%avg_rules_hsh, 1);
}

sub make_avg_app_histogram_graphs ()
{
	print "make_avg_app_histogram_graphs ()\n";

	my $data_base;

	my $ParmsLazy = Common::GetBestAverageAccParms ('lazy');

	foreach $data_base (@Common::DataBases)
	{
		my $RunResult = Common::GetLazyRunResult ($data_base, $ParmsLazy->{SUPPORT}, $ParmsLazy->{CONFIDENCE}, $ParmsLazy->{MIN_RULES}, $ParmsLazy->{MAX_SIZE}, $ParmsLazy->{RANKING_SIZE});

		$avg_acc_hsh{$data_base}{'lazy'} = $RunResult->{ACCURACY};
		$avg_pat_hsh{$data_base}{'lazy'} = $RunResult->{AVG_PATTERNS};
		$avg_rul_hsh{$data_base}{'lazy'} = $RunResult->{AVG_RULES};
	}

	$avg_acc_hsh{'average'}{'lazy'} = $ParmsLazy->{ACCURACY};
	$avg_pat_hsh{'average'}{'lazy'} = $ParmsLazy->{AVG_PATTERNS};
	$avg_rul_hsh{'average'}{'lazy'} = $ParmsLazy->{AVG_RULES};

	my $ParmsClassifierC = Common::GetBestAverageAccParms ('classifier_c');

	foreach $data_base (@Common::DataBases)
	{
		my $RunResult = Common::GetClassifierCRunResult ($data_base, $ParmsClassifierC->{SUPPORT}, $ParmsClassifierC->{CONFIDENCE}, $ParmsClassifierC->{MIN_NUM_RULES}, $ParmsClassifierC->{MAX_NUM_RANK_RULES}, $ParmsClassifierC->{MIN_RULE_LEN}, $ParmsClassifierC->{MAX_RULE_LEN});

		$avg_acc_hsh{$data_base}{'classifier_c'} = $RunResult->{ACCURACY};
		$avg_pat_hsh{$data_base}{'classifier_c'} = $RunResult->{AVG_PATTERNS};
		$avg_rul_hsh{$data_base}{'classifier_c'} = $RunResult->{AVG_RULES};
	}

	$avg_acc_hsh{'average'}{'classifier_c'} = $ParmsClassifierC->{ACCURACY};
	$avg_pat_hsh{'average'}{'classifier_c'} = $ParmsClassifierC->{AVG_PATTERNS};
	$avg_rul_hsh{'average'}{'classifier_c'} = $ParmsClassifierC->{AVG_RULES};

	my $ParmsClassifierO = Common::GetBestAverageAccParms ('classifier_o');

	foreach $data_base (@Common::DataBases)
	{
		my $RunResult = Common::GetClassifierORunResult ($data_base, $ParmsClassifierO->{SUPPORT}, $ParmsClassifierO->{CONFIDENCE}, $ParmsClassifierO->{PATTERN_SET}, $ParmsClassifierO->{MIN_NUM_RULES}, $ParmsClassifierO->{MAX_NUM_RANK_RULES}, $ParmsClassifierO->{MIN_RULE_LEN}, $ParmsClassifierO->{MAX_RULE_LEN}, $ParmsClassifierO->{OMODE}, $ParmsClassifierO->{OMETRIC}, $ParmsClassifierO->{OMETHOD}, $ParmsClassifierO->{OORDERING});

		$avg_acc_hsh{$data_base}{'classifier_o'} = $RunResult->{ACCURACY};
		$avg_pat_hsh{$data_base}{'classifier_o'} = $RunResult->{AVG_PATTERNS};
		$avg_rul_hsh{$data_base}{'classifier_o'} = $RunResult->{AVG_RULES};
	}

	$avg_acc_hsh{'average'}{'classifier_o'} = $ParmsClassifierO->{ACCURACY};
	$avg_pat_hsh{'average'}{'classifier_o'} = $ParmsClassifierO->{AVG_PATTERNS};
	$avg_rul_hsh{'average'}{'classifier_o'} = $ParmsClassifierO->{AVG_RULES};

	my $ParmsClassifierOR = Common::GetBestAverageAccParms ('classifier_or');

	foreach $data_base (@Common::DataBases)
	{
		my $RunResult = Common::GetClassifierORRunResult ($data_base, $ParmsClassifierOR->{SUPPORT}, $ParmsClassifierOR->{CONFIDENCE}, $ParmsClassifierOR->{MIN_NUM_RULES}, $ParmsClassifierOR->{MAX_NUM_RANK_RULES}, $ParmsClassifierOR->{OMETRIC}, $ParmsClassifierOR->{ALPHA}, $ParmsClassifierOR->{BETA});

		$avg_acc_hsh{$data_base}{'classifier_or'} = $RunResult->{ACCURACY};
		$avg_pat_hsh{$data_base}{'classifier_or'} = $RunResult->{AVG_PATTERNS};
		$avg_rul_hsh{$data_base}{'classifier_or'} = $RunResult->{AVG_RULES};
	}

	$avg_acc_hsh{'average'}{'classifier_or'} = $ParmsClassifierOR->{ACCURACY};
	$avg_pat_hsh{'average'}{'classifier_or'} = $ParmsClassifierOR->{AVG_PATTERNS};
	$avg_rul_hsh{'average'}{'classifier_or'} = $ParmsClassifierOR->{AVG_RULES};

	Common::MakeAppHistogramGraph ('Best Parameters Accuracy Histogram', 'Data Sets', 'Accuracy', 'bst_histogram_acc', \%avg_acc_hsh, 0);
	Common::MakeAppHistogramGraph ('Best Parameters Patterns Average Histogram', 'Data Sets', 'Patterns Average', 'bst_histogram_pat', \%avg_pat_hsh, 1);
	Common::MakeAppHistogramGraph ('Best Parameters Rules Average Histogram', 'Data Sets', 'Rules Average', 'bst_histogram_rul', \%avg_rul_hsh, 1);
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

	$ometric_acc_hsh{'average'}{'s'} /= scalar @Common::DataBases;
	$ometric_acc_hsh{'average'}{'c'} /= scalar @Common::DataBases;
	$ometric_acc_hsh{'average'}{'l'} /= scalar @Common::DataBases;
	$ometric_acc_hsh{'average'}{'m'} /= scalar @Common::DataBases;

	$ometric_pat_hsh{'average'}{'s'} /= scalar @Common::DataBases;
	$ometric_pat_hsh{'average'}{'c'} /= scalar @Common::DataBases;
	$ometric_pat_hsh{'average'}{'l'} /= scalar @Common::DataBases;
	$ometric_pat_hsh{'average'}{'m'} /= scalar @Common::DataBases;

	$ometric_rul_hsh{'average'}{'s'} /= scalar @Common::DataBases;
	$ometric_rul_hsh{'average'}{'c'} /= scalar @Common::DataBases;
	$ometric_rul_hsh{'average'}{'l'} /= scalar @Common::DataBases;
	$ometric_rul_hsh{'average'}{'m'} /= scalar @Common::DataBases;

	Common::MakeOMetricHistogramGraph ('Orthogonality Metrics Accuracy Histogram', 'Data Sets', 'Accuracy', 'histogram_ometric_acc', \%ometric_acc_hsh);
	Common::MakeOMetricHistogramGraph ('Orthogonality Metrics Patterns Average Histogram', 'Data Sets', 'Patterns Average', 'histogram_ometric_pat', \%ometric_pat_hsh);
	Common::MakeOMetricHistogramGraph ('Orthogonality Metrics Rules Average Histogram', 'Data Sets', 'Rules Average', 'histogram_ometric_rul', \%ometric_rul_hsh);
}
