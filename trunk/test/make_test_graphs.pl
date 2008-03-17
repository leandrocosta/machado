#!/usr/bin/perl -w

use strict;
use Common;

sub make_best_app_histogram_graphs ();
sub make_avg_app_histogram_graphs ();
sub make_ometric_histogram_graphs ($$$$$$$);
sub make_avg_ometric_histogram_graphs ();
sub make_lazy_compare_histogram_graphs ();

sub get_best_runs_for_data_base ($);
sub get_ometric_runs_for_classifier_o ($$$$$$$$$$$);
sub get_ometric_run_for_classifier_o ($$$$$$$$$$$$);

my %best_run_for_each_db_acc_hsh;
my %best_run_for_each_db_pat_hsh;
my %best_run_for_each_db_rul_hsh;
my %best_run_for_each_db_tim_hsh;

my %ometric_acc_hsh;
my %ometric_pat_hsh;
my %ometric_rul_hsh;
my %ometric_tim_hsh;

my %best_run_for_avg_db_acc_hsh;
my %best_run_for_avg_db_pat_hsh;
my %best_run_for_avg_db_rul_hsh;
my %best_run_for_avg_db_tim_hsh;

my %best_run_for_lac_avg_db_acc_hsh;
my %best_run_for_lac_avg_db_pat_hsh;
my %best_run_for_lac_avg_db_rul_hsh;
my %best_run_for_lac_avg_db_tim_hsh;

my $db;
my $i;

my $support		= '0.0001';	#'0.1';
my $confidence		= '0.0001';	#'0.9';
my $min_num_rules	= '1';
my $max_num_rank_rules	= '100';	#'1';
my $min_rule_len	= '1';
my $max_rule_len	= '2';
my $omode		= 'h';

system "mkdir -p $Common::OutputDirGraphs/";

make_best_app_histogram_graphs ();
make_avg_app_histogram_graphs ();
make_avg_ometric_histogram_graphs ();
#make_lazy_compare_histogram_graphs ();

### make_ometric_histogram_graphs ($support, $confidence, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode);


exit;

###

sub get_best_run_for_application ($$)
{
	my ($application, $data_base) = @_;

	my $RunResult = Common::GetBestRunResult ($application, $data_base);

	$best_run_for_each_db_acc_hsh{$data_base}{$application}	= $RunResult->{ACCURACY};
	$best_run_for_each_db_pat_hsh{$data_base}{$application}	= $RunResult->{AVG_PATTERNS};
	$best_run_for_each_db_rul_hsh{$data_base}{$application}	= $RunResult->{AVG_RULES};
	$best_run_for_each_db_tim_hsh{$data_base}{$application}	= $RunResult->{AVG_TIME};

	$best_run_for_each_db_acc_hsh{'average'}{$application}	+= $RunResult->{ACCURACY};
	$best_run_for_each_db_pat_hsh{'average'}{$application}	+= $RunResult->{AVG_PATTERNS};
	$best_run_for_each_db_rul_hsh{'average'}{$application}	+= $RunResult->{AVG_RULES};
	$best_run_for_each_db_tim_hsh{'average'}{$application}	+= $RunResult->{AVG_TIME};
}

sub get_best_runs_for_data_base ($)
{
	get_best_run_for_application ('lazy', $_[0]);
	get_best_run_for_application ('classifier_c', $_[0]);
	get_best_run_for_application ('classifier_o', $_[0]);
	get_best_run_for_application ('classifier_or', $_[0]);
}

sub get_ometric_runs_for_classifier_o ($$$$$$$$$$$)
{
	my ($data_base, $support, $confidence, $pattern_set, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode, $omethod, $oordering) = @_;

	get_ometric_run_for_classifier_o ($data_base, $support, $confidence, $pattern_set, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode, 's', $omethod, $oordering);
	get_ometric_run_for_classifier_o ($data_base, $support, $confidence, $pattern_set, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode, 'c', $omethod, $oordering);
	get_ometric_run_for_classifier_o ($data_base, $support, $confidence, $pattern_set, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode, 'l', $omethod, $oordering);
	get_ometric_run_for_classifier_o ($data_base, $support, $confidence, $pattern_set, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode, 'a', $omethod, $oordering);
}

sub get_ometric_run_for_classifier_o ($$$$$$$$$$$$)
{
	my ($data_base, $support, $confidence, $pattern_set, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode, $ometric, $omethod, $oordering) = @_;

	my $RunResult = Common::GetClassifierORunResult ($_[0], $_[1], $_[2], $_[3], $_[4], $_[5], $_[6], $_[7], $_[8], $_[9], $_[10], $_[11]);

	$ometric_acc_hsh{$data_base}{$ometric} = $RunResult->{ACCURACY};
	$ometric_pat_hsh{$data_base}{$ometric} = $RunResult->{AVG_PATTERNS};
	$ometric_rul_hsh{$data_base}{$ometric} = $RunResult->{AVG_RULES};
	$ometric_tim_hsh{$data_base}{$ometric} = $RunResult->{AVG_TIME};

	$ometric_acc_hsh{'average'}{$ometric}	+= $RunResult->{ACCURACY};
	$ometric_pat_hsh{'average'}{$ometric}	+= $RunResult->{AVG_PATTERNS};
	$ometric_rul_hsh{'average'}{$ometric}	+= $RunResult->{AVG_RULES};
	$ometric_tim_hsh{'average'}{$ometric}	+= $RunResult->{AVG_TIME};
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

	$best_run_for_each_db_acc_hsh{'average'}{'lazy'}		/= scalar @Common::DataBases;
	$best_run_for_each_db_acc_hsh{'average'}{'classifier_c'}	/= scalar @Common::DataBases;
	$best_run_for_each_db_acc_hsh{'average'}{'classifier_o'}	/= scalar @Common::DataBases;
	$best_run_for_each_db_acc_hsh{'average'}{'classifier_or'}	/= scalar @Common::DataBases;

	$best_run_for_each_db_pat_hsh{'average'}{'lazy'}		/= scalar @Common::DataBases;
	$best_run_for_each_db_pat_hsh{'average'}{'classifier_c'}	/= scalar @Common::DataBases;
	$best_run_for_each_db_pat_hsh{'average'}{'classifier_o'}	/= scalar @Common::DataBases;
	$best_run_for_each_db_pat_hsh{'average'}{'classifier_or'}	/= scalar @Common::DataBases;

	$best_run_for_each_db_rul_hsh{'average'}{'lazy'}		/= scalar @Common::DataBases;
	$best_run_for_each_db_rul_hsh{'average'}{'classifier_c'}	/= scalar @Common::DataBases;
	$best_run_for_each_db_rul_hsh{'average'}{'classifier_o'}	/= scalar @Common::DataBases;
	$best_run_for_each_db_rul_hsh{'average'}{'classifier_or'}	/= scalar @Common::DataBases;

	$best_run_for_each_db_tim_hsh{'average'}{'lazy'}		/= scalar @Common::DataBases;
	$best_run_for_each_db_tim_hsh{'average'}{'classifier_c'}	/= scalar @Common::DataBases;
	$best_run_for_each_db_tim_hsh{'average'}{'classifier_o'}	/= scalar @Common::DataBases;
	$best_run_for_each_db_tim_hsh{'average'}{'classifier_or'}	/= scalar @Common::DataBases;

	Common::MakeAppHistogramGraph ('Accuracy Histogram', 'Data Sets', 'Accuracy', 'histogram_best_run_for_each_db_acc', \%best_run_for_each_db_acc_hsh, 0);
	Common::MakeAppHistogramGraph ('Patterns Average Histogram', 'Data Sets', 'Patterns Average', 'histogram_best_run_for_each_db_pat', \%best_run_for_each_db_pat_hsh, 1);
	Common::MakeAppHistogramGraph ('Rules Average Histogram', 'Data Sets', 'Rules Average', 'histogram_best_run_for_each_db_rul', \%best_run_for_each_db_rul_hsh, 1);
	Common::MakeAppHistogramGraph ('Classification Time Average Histogram', 'Data Sets', 'Classification Time Average', 'histogram_best_run_for_each_db_tim', \%best_run_for_each_db_tim_hsh, 1);
}

sub make_avg_app_histogram_graphs ()
{
	print "make_avg_app_histogram_graphs ()\n";

	my $data_base;

	my $ParmsLazy = Common::GetBestAverageAccParms ('lazy');

	foreach $data_base (@Common::DataBases)
	{
		print "base: $data_base\n";

		my $RunResult = Common::GetLazyRunResult ($data_base, $ParmsLazy->{SUPPORT}, $ParmsLazy->{CONFIDENCE}, $ParmsLazy->{MIN_RULES}, $ParmsLazy->{MAX_SIZE}, $ParmsLazy->{RANKING_SIZE});

		$best_run_for_avg_db_acc_hsh{$data_base}{'lazy'} = $RunResult->{ACCURACY};
		$best_run_for_avg_db_pat_hsh{$data_base}{'lazy'} = $RunResult->{AVG_PATTERNS};
		$best_run_for_avg_db_rul_hsh{$data_base}{'lazy'} = $RunResult->{AVG_RULES};
		$best_run_for_avg_db_tim_hsh{$data_base}{'lazy'} = $RunResult->{AVG_TIME};
	}

	$best_run_for_avg_db_acc_hsh{'average'}{'lazy'} = $ParmsLazy->{ACCURACY};
	$best_run_for_avg_db_pat_hsh{'average'}{'lazy'} = $ParmsLazy->{AVG_PATTERNS};
	$best_run_for_avg_db_rul_hsh{'average'}{'lazy'} = $ParmsLazy->{AVG_RULES};
	$best_run_for_avg_db_tim_hsh{'average'}{'lazy'} = $ParmsLazy->{AVG_TIME};

	my $ParmsClassifierC = Common::GetBestAverageAccParms ('classifier_c');

	foreach $data_base (@Common::DataBases)
	{
		my $RunResult = Common::GetClassifierCRunResult ($data_base, $ParmsClassifierC->{SUPPORT}, $ParmsClassifierC->{CONFIDENCE}, $ParmsClassifierC->{MIN_NUM_RULES}, $ParmsClassifierC->{MAX_NUM_RANK_RULES}, $ParmsClassifierC->{MIN_RULE_LEN}, $ParmsClassifierC->{MAX_RULE_LEN});

		$best_run_for_avg_db_acc_hsh{$data_base}{'classifier_c'} = $RunResult->{ACCURACY};
		$best_run_for_avg_db_pat_hsh{$data_base}{'classifier_c'} = $RunResult->{AVG_PATTERNS};
		$best_run_for_avg_db_rul_hsh{$data_base}{'classifier_c'} = $RunResult->{AVG_RULES};
		$best_run_for_avg_db_tim_hsh{$data_base}{'classifier_c'} = $RunResult->{AVG_TIME};
	}

	$best_run_for_avg_db_acc_hsh{'average'}{'classifier_c'} = $ParmsClassifierC->{ACCURACY};
	$best_run_for_avg_db_pat_hsh{'average'}{'classifier_c'} = $ParmsClassifierC->{AVG_PATTERNS};
	$best_run_for_avg_db_rul_hsh{'average'}{'classifier_c'} = $ParmsClassifierC->{AVG_RULES};
	$best_run_for_avg_db_tim_hsh{'average'}{'classifier_c'} = $ParmsClassifierC->{AVG_TIME};

	my $ParmsClassifierO = Common::GetBestAverageAccParms ('classifier_o');

	foreach $data_base (@Common::DataBases)
	{
		my $RunResult = Common::GetClassifierORunResult ($data_base, $ParmsClassifierO->{SUPPORT}, $ParmsClassifierO->{CONFIDENCE}, $ParmsClassifierO->{PATTERN_SET}, $ParmsClassifierO->{MIN_NUM_RULES}, $ParmsClassifierO->{MAX_NUM_RANK_RULES}, $ParmsClassifierO->{MIN_RULE_LEN}, $ParmsClassifierO->{MAX_RULE_LEN}, $ParmsClassifierO->{OMODE}, $ParmsClassifierO->{OMETRIC}, $ParmsClassifierO->{OMETHOD}, $ParmsClassifierO->{OORDERING});

		$best_run_for_avg_db_acc_hsh{$data_base}{'classifier_o'} = $RunResult->{ACCURACY};
		$best_run_for_avg_db_pat_hsh{$data_base}{'classifier_o'} = $RunResult->{AVG_PATTERNS};
		$best_run_for_avg_db_rul_hsh{$data_base}{'classifier_o'} = $RunResult->{AVG_RULES};
		$best_run_for_avg_db_tim_hsh{$data_base}{'classifier_o'} = $RunResult->{AVG_TIME};
	}

	$best_run_for_avg_db_acc_hsh{'average'}{'classifier_o'} = $ParmsClassifierO->{ACCURACY};
	$best_run_for_avg_db_pat_hsh{'average'}{'classifier_o'} = $ParmsClassifierO->{AVG_PATTERNS};
	$best_run_for_avg_db_rul_hsh{'average'}{'classifier_o'} = $ParmsClassifierO->{AVG_RULES};
	$best_run_for_avg_db_tim_hsh{'average'}{'classifier_o'} = $ParmsClassifierO->{AVG_TIME};

	my $ParmsClassifierOR = Common::GetBestAverageAccParms ('classifier_or');

	foreach $data_base (@Common::DataBases)
	{
		my $RunResult = Common::GetClassifierORRunResult ($data_base, $ParmsClassifierOR->{SUPPORT}, $ParmsClassifierOR->{CONFIDENCE}, $ParmsClassifierOR->{MIN_NUM_RULES}, $ParmsClassifierOR->{MAX_NUM_RANK_RULES}, $ParmsClassifierOR->{OMETRIC}, $ParmsClassifierOR->{ALPHA}, $ParmsClassifierOR->{BETA});

		$best_run_for_avg_db_acc_hsh{$data_base}{'classifier_or'} = $RunResult->{ACCURACY};
		$best_run_for_avg_db_pat_hsh{$data_base}{'classifier_or'} = $RunResult->{AVG_PATTERNS};
		$best_run_for_avg_db_rul_hsh{$data_base}{'classifier_or'} = $RunResult->{AVG_RULES};
		$best_run_for_avg_db_tim_hsh{$data_base}{'classifier_or'} = $RunResult->{AVG_TIME};
	}

	$best_run_for_avg_db_acc_hsh{'average'}{'classifier_or'} = $ParmsClassifierOR->{ACCURACY};
	$best_run_for_avg_db_pat_hsh{'average'}{'classifier_or'} = $ParmsClassifierOR->{AVG_PATTERNS};
	$best_run_for_avg_db_rul_hsh{'average'}{'classifier_or'} = $ParmsClassifierOR->{AVG_RULES};
	$best_run_for_avg_db_tim_hsh{'average'}{'classifier_or'} = $ParmsClassifierOR->{AVG_TIME};

	Common::MakeAppHistogramGraph ('Best Parameters Accuracy Histogram', 'Data Sets', 'Accuracy', 'histogram_best_run_for_avg_db_acc', \%best_run_for_avg_db_acc_hsh, 0);
	Common::MakeAppHistogramGraph ('Best Parameters Patterns Average Histogram', 'Data Sets', 'Patterns Average', 'histogram_best_run_for_avg_db_pat', \%best_run_for_avg_db_pat_hsh, 1);
	Common::MakeAppHistogramGraph ('Best Parameters Rules Average Histogram', 'Data Sets', 'Rules Average', 'histogram_best_run_for_avg_db_rul', \%best_run_for_avg_db_rul_hsh, 1);
	Common::MakeAppHistogramGraph ('Best Parameters Classification Time Average Histogram', 'Data Sets', 'Classification Time Average', 'histogram_best_run_for_avg_db_tim', \%best_run_for_avg_db_tim_hsh, 1);
}

sub make_ometric_histogram_graphs ($$$$$$$)
{
	print "make_ometric_histogram_graphs ()\n";

	my ($support, $confidence, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode) = @_;

	my $data_base;

	foreach $data_base (@Common::DataBases)
	{
		print "base: $data_base\n";

		get_ometric_runs_for_classifier_o ($data_base, $support, $confidence, 'f', $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode, 's', 's');
	}

	$ometric_acc_hsh{'average'}{'s'} /= scalar @Common::DataBases;
	$ometric_acc_hsh{'average'}{'c'} /= scalar @Common::DataBases;
	$ometric_acc_hsh{'average'}{'l'} /= scalar @Common::DataBases;
	$ometric_acc_hsh{'average'}{'a'} /= scalar @Common::DataBases;

	$ometric_pat_hsh{'average'}{'s'} /= scalar @Common::DataBases;
	$ometric_pat_hsh{'average'}{'c'} /= scalar @Common::DataBases;
	$ometric_pat_hsh{'average'}{'l'} /= scalar @Common::DataBases;
	$ometric_pat_hsh{'average'}{'a'} /= scalar @Common::DataBases;

	$ometric_rul_hsh{'average'}{'s'} /= scalar @Common::DataBases;
	$ometric_rul_hsh{'average'}{'c'} /= scalar @Common::DataBases;
	$ometric_rul_hsh{'average'}{'l'} /= scalar @Common::DataBases;
	$ometric_rul_hsh{'average'}{'a'} /= scalar @Common::DataBases;

	$ometric_tim_hsh{'average'}{'s'} /= scalar @Common::DataBases;
	$ometric_tim_hsh{'average'}{'c'} /= scalar @Common::DataBases;
	$ometric_tim_hsh{'average'}{'l'} /= scalar @Common::DataBases;
	$ometric_tim_hsh{'average'}{'a'} /= scalar @Common::DataBases;

	Common::MakeOMetricHistogramGraph ('Orthogonality Metrics Accuracy Histogram', 'Data Sets', 'Accuracy', 'histogram_ometric_acc', \%ometric_acc_hsh);
	Common::MakeOMetricHistogramGraph ('Orthogonality Metrics Patterns Average Histogram', 'Data Sets', 'Patterns Average', 'histogram_ometric_pat', \%ometric_pat_hsh);
	Common::MakeOMetricHistogramGraph ('Orthogonality Metrics Rules Average Histogram', 'Data Sets', 'Rules Average', 'histogram_ometric_rul', \%ometric_rul_hsh);
	Common::MakeOMetricHistogramGraph ('Orthogonality Metrics Classification Time Average Histogram', 'Data Sets', 'Classification Time Average', 'histogram_ometric_tim', \%ometric_tim_hsh);
}

sub make_avg_ometric_histogram_graphs ()
{
	print "make_avg_ometric_histogram_graphs ()\n";

	my $ParmsClassifierO = Common::GetBestAverageAccParms ('classifier_o');

	my $support		= $ParmsClassifierO->{SUPPORT};
	my $confidence		= $ParmsClassifierO->{CONFIDENCE};
	my $pattern_set		= $ParmsClassifierO->{PATTERN_SET};
	my $min_num_rules	= $ParmsClassifierO->{MIN_NUM_RULES};
	my $max_num_rank_rules	= $ParmsClassifierO->{MAX_NUM_RANK_RULES};
	my $min_rule_len	= $ParmsClassifierO->{MIN_RULE_LEN};
	my $max_rule_len	= $ParmsClassifierO->{MAX_RULE_LEN};
	my $omode		= $ParmsClassifierO->{OMODE};
	my $omethod		= $ParmsClassifierO->{OMETHOD};
	my $oordering		= $ParmsClassifierO->{OORDERING};

	my $data_base;

	foreach $data_base (@Common::DataBases)
	{
		print "base: $data_base\n";

		get_ometric_runs_for_classifier_o ($data_base, $support, $confidence, $pattern_set, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode, $omethod, $oordering);
	}

	$ometric_acc_hsh{'average'}{'s'} /= scalar @Common::DataBases;
	$ometric_acc_hsh{'average'}{'c'} /= scalar @Common::DataBases;
	$ometric_acc_hsh{'average'}{'l'} /= scalar @Common::DataBases;
	$ometric_acc_hsh{'average'}{'a'} /= scalar @Common::DataBases;

	$ometric_pat_hsh{'average'}{'s'} /= scalar @Common::DataBases;
	$ometric_pat_hsh{'average'}{'c'} /= scalar @Common::DataBases;
	$ometric_pat_hsh{'average'}{'l'} /= scalar @Common::DataBases;
	$ometric_pat_hsh{'average'}{'a'} /= scalar @Common::DataBases;

	$ometric_rul_hsh{'average'}{'s'} /= scalar @Common::DataBases;
	$ometric_rul_hsh{'average'}{'c'} /= scalar @Common::DataBases;
	$ometric_rul_hsh{'average'}{'l'} /= scalar @Common::DataBases;
	$ometric_rul_hsh{'average'}{'a'} /= scalar @Common::DataBases;

	$ometric_tim_hsh{'average'}{'s'} /= scalar @Common::DataBases;
	$ometric_tim_hsh{'average'}{'c'} /= scalar @Common::DataBases;
	$ometric_tim_hsh{'average'}{'l'} /= scalar @Common::DataBases;
	$ometric_tim_hsh{'average'}{'a'} /= scalar @Common::DataBases;

	Common::MakeOMetricHistogramGraph ('Orthogonality Metrics Accuracy Histogram', 'Data Sets', 'Accuracy', 'histogram_best_run_for_avg_db_ometric_acc', \%ometric_acc_hsh);
	Common::MakeOMetricHistogramGraph ('Orthogonality Metrics Patterns Average Histogram', 'Data Sets', 'Patterns Average', 'histogram_best_run_for_avg_db_ometric_pat', \%ometric_pat_hsh);
	Common::MakeOMetricHistogramGraph ('Orthogonality Metrics Rules Average Histogram', 'Data Sets', 'Rules Average', 'histogram_best_run_for_avg_db_ometric_rul', \%ometric_rul_hsh);
	Common::MakeOMetricHistogramGraph ('Orthogonality Metrics Classification Time Average Histogram', 'Data Sets', 'Classification Time Average', 'histogram_best_run_for_avg_db_ometric_tim', \%ometric_tim_hsh);
}

sub make_lazy_compare_histogram_graphs ()
{
	print "make_lazy_compare_histogram_graphs ()\n";

	my $data_base;

	my $ParmsLazy = Common::GetBestAverageAccParms ('lazy');

	foreach $data_base (@Common::DataBases)
	{
		print "base: $data_base\n";

		my $RunResult = Common::GetLazyRunResult ($data_base, $ParmsLazy->{SUPPORT}, $ParmsLazy->{CONFIDENCE}, $ParmsLazy->{MIN_RULES}, $ParmsLazy->{MAX_SIZE}, $ParmsLazy->{RANKING_SIZE});

		$best_run_for_lac_avg_db_acc_hsh{$data_base}{'lazy'} = $RunResult->{ACCURACY};
		$best_run_for_lac_avg_db_pat_hsh{$data_base}{'lazy'} = $RunResult->{AVG_PATTERNS};
		$best_run_for_lac_avg_db_rul_hsh{$data_base}{'lazy'} = $RunResult->{AVG_RULES};
		$best_run_for_lac_avg_db_tim_hsh{$data_base}{'lazy'} = $RunResult->{AVG_TIME};
	}

	$best_run_for_lac_avg_db_acc_hsh{'average'}{'lazy'} = $ParmsLazy->{ACCURACY};
	$best_run_for_lac_avg_db_pat_hsh{'average'}{'lazy'} = $ParmsLazy->{AVG_PATTERNS};
	$best_run_for_lac_avg_db_rul_hsh{'average'}{'lazy'} = $ParmsLazy->{AVG_RULES};
	$best_run_for_lac_avg_db_tim_hsh{'average'}{'lazy'} = $ParmsLazy->{AVG_TIME};

	my $ParmsClassifierC = Common::GetBestAverageAccParms ('classifier_c');

	foreach $data_base (@Common::DataBases)
	{
		my $RunResult = Common::GetClassifierCRunResult ($data_base, 0.0001, $ParmsLazy->{CONFIDENCE}, $ParmsLazy->{MIN_RULES}, $ParmsLazy->{RANKING_SIZE}, 1, $ParmsLazy->{MAX_SIZE}-1);

		$best_run_for_lac_avg_db_acc_hsh{$data_base}{'classifier_c'} = $RunResult->{ACCURACY};
		$best_run_for_lac_avg_db_pat_hsh{$data_base}{'classifier_c'} = $RunResult->{AVG_PATTERNS};
		$best_run_for_lac_avg_db_rul_hsh{$data_base}{'classifier_c'} = $RunResult->{AVG_RULES};
		$best_run_for_lac_avg_db_tim_hsh{$data_base}{'classifier_c'} = $RunResult->{AVG_TIME};
	}

	$best_run_for_lac_avg_db_acc_hsh{'average'}{'classifier_c'} = $ParmsClassifierC->{ACCURACY};
	$best_run_for_lac_avg_db_pat_hsh{'average'}{'classifier_c'} = $ParmsClassifierC->{AVG_PATTERNS};
	$best_run_for_lac_avg_db_rul_hsh{'average'}{'classifier_c'} = $ParmsClassifierC->{AVG_RULES};
	$best_run_for_lac_avg_db_tim_hsh{'average'}{'classifier_c'} = $ParmsClassifierC->{AVG_TIME};

	my $ParmsClassifierO = Common::GetBestAverageAccParms ('classifier_o');

	foreach $data_base (@Common::DataBases)
	{
		my $RunResult = Common::GetClassifierORunResult ($data_base, 0.0001, $ParmsLazy->{CONFIDENCE}, $ParmsClassifierO->{PATTERN_SET}, $ParmsLazy->{MIN_RULES}, $ParmsLazy->{RANKING_SIZE}, 1, $ParmsLazy->{MAX_SIZE}, $ParmsClassifierO->{OMODE}, $ParmsClassifierO->{OMETRIC}, $ParmsClassifierO->{OMETHOD}, $ParmsClassifierO->{OORDERING});

		$best_run_for_lac_avg_db_acc_hsh{$data_base}{'classifier_o'} = $RunResult->{ACCURACY};
		$best_run_for_lac_avg_db_pat_hsh{$data_base}{'classifier_o'} = $RunResult->{AVG_PATTERNS};
		$best_run_for_lac_avg_db_rul_hsh{$data_base}{'classifier_o'} = $RunResult->{AVG_RULES};
		$best_run_for_lac_avg_db_tim_hsh{$data_base}{'classifier_o'} = $RunResult->{AVG_TIME};
	}

	$best_run_for_lac_avg_db_acc_hsh{'average'}{'classifier_o'} = $ParmsClassifierO->{ACCURACY};
	$best_run_for_lac_avg_db_pat_hsh{'average'}{'classifier_o'} = $ParmsClassifierO->{AVG_PATTERNS};
	$best_run_for_lac_avg_db_rul_hsh{'average'}{'classifier_o'} = $ParmsClassifierO->{AVG_RULES};
	$best_run_for_lac_avg_db_tim_hsh{'average'}{'classifier_o'} = $ParmsClassifierO->{AVG_TIME};

	my $ParmsClassifierOR = Common::GetBestAverageAccParms ('classifier_or');

	foreach $data_base (@Common::DataBases)
	{
		my $RunResult = Common::GetClassifierORRunResult ($data_base, 0.0001, $ParmsLazy->{CONFIDENCE}, $ParmsLazy->{MIN_RULES}, $ParmsLazy->{RANKING_SIZE}, $ParmsClassifierOR->{OMETRIC}, $ParmsClassifierOR->{ALPHA}, $ParmsClassifierOR->{BETA});

		$best_run_for_lac_avg_db_acc_hsh{$data_base}{'classifier_or'} = $RunResult->{ACCURACY};
		$best_run_for_lac_avg_db_pat_hsh{$data_base}{'classifier_or'} = $RunResult->{AVG_PATTERNS};
		$best_run_for_lac_avg_db_rul_hsh{$data_base}{'classifier_or'} = $RunResult->{AVG_RULES};
		$best_run_for_lac_avg_db_tim_hsh{$data_base}{'classifier_or'} = $RunResult->{AVG_TIME};
	}

	$best_run_for_lac_avg_db_acc_hsh{'average'}{'classifier_or'} = $ParmsClassifierOR->{ACCURACY};
	$best_run_for_lac_avg_db_pat_hsh{'average'}{'classifier_or'} = $ParmsClassifierOR->{AVG_PATTERNS};
	$best_run_for_lac_avg_db_rul_hsh{'average'}{'classifier_or'} = $ParmsClassifierOR->{AVG_RULES};
	$best_run_for_lac_avg_db_tim_hsh{'average'}{'classifier_or'} = $ParmsClassifierOR->{AVG_TIME};

	Common::MakeAppHistogramGraph ('Best LAC Parameters Accuracy Histogram', 'Data Sets', 'Accuracy', 'histogram_best_run_for_lac_avg_db_acc', \%best_run_for_lac_avg_db_acc_hsh, 0);
	Common::MakeAppHistogramGraph ('Best LAC Parameters Patterns Average Histogram', 'Data Sets', 'Patterns Average', 'histogram_best_run_for_lac_avg_db_pat', \%best_run_for_lac_avg_db_pat_hsh, 1);
	Common::MakeAppHistogramGraph ('Best LAC Parameters Rules Average Histogram', 'Data Sets', 'Rules Average', 'histogram_best_run_for_lac_avg_db_rul', \%best_run_for_lac_avg_db_rul_hsh, 1);
	Common::MakeAppHistogramGraph ('Best LAC Parameters Classification Time Average Histogram', 'Data Sets', 'Classification Time Average', 'histogram_best_run_for_lac_avg_db_tim', \%best_run_for_lac_avg_db_tim_hsh, 1);
}
=cut
