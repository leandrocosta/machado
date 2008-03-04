package Common;

$AppLazy		= './lazy';
$AppClassifier		= './classifier';

$MachadoDir		= '..';
$DataDir		= "$MachadoDir/data/adrianov/fold";

$OutputDir		= 'output';
$OutputDirLazy		= "$OutputDir/lazy";
$OutputDirClassifierC	= "$OutputDir/classifier_c";
$OutputDirClassifierO	= "$OutputDir/classifier_o";
$OutputDirClassifierOR	= "$OutputDir/classifier_or";
$OutputDirGraphs	= "$OutputDir/graphs";
$OutputDirTables	= "$OutputDir/tables";

$GnuPlotApp		= '~/local/gnuplot/bin/gnuplot';

@DataBases = (
	'anneal.ac',
	'austra.ac',
	'auto.ac',
	'breast.ac',
	'cleve.ac',
	'crx.ac',
	'diabetes.ac',
	'german.ac',
	'glass.ac',
	'heart.ac',
	'hepati.ac',
	'horse.ac',
	'hypo.ac',
	'ionosphere.ac',
	'iris.ac',
	'labor.ac',
	'led7.ac',
	'lymph.ac',
	'pima.ac',
	'sick.ac',
	'sonar.ac',
	'tic-tac.ac',
	'vehicle.ac',
	'waveform.ac',
	'wine.ac',
	'zoo.ac'
);

@Confidences = (
	0.0001,
#	0.001,
#	0.01,
	0.1,
#	0.2,
	0.3,
#	0.4,
	0.5,
#	0.6,
	0.7,
#	0.8,
	0.9,
	0.95,
#	0.99,
#	1
);

@MinNumRules = (
	1,
#	10,
#	100,
#	1000
);

@MaxNumRankRules = (
#	1,
	10,
	100,
	1000,
#	10000,
#	100000,
#	1000000
);

@LazySupports = (
	1
);

@LazyMaxSizes = (
	2,
#	3,
#	4,
#	5
);

@ClassifierSupports = (
	0.0001,
#	0.001,
#	0.01,
	0.1,
#	0.2,
	0.3,
#	0.4,
	0.5,
#	0.6,
#	0.7,
#	0.8,
#	0.9,
#	0.95,
#	0.99,
#	1
);

@ClassifierPatternSets = (
	'f',
#	'm',
	'r'
);

@ClassifierMinRuleLens = (
	1,
);

@ClassifierMaxRuleLens = (
	1,
#	2,
#	3,
#	4,
#	5
);

@ClassifierOModes = (
	'h',
#	'p'
);

@ClassifierOMetrics = (
#	's',
#	'c',
	'l',
#	'a'
);

@ClassifierOMethods = (
	's',
#	'p',
#	'a'
);

@ClassifierOOrderings = (
	's',
#	'r',
#	'i',
#	'z',
#	'n'
);

@ORIGAMIAlphas = (
#	0.1,
	0.2,
#	0.3,
#	0.4,
#	0.5,
#	0.6,
#	0.7,
#	0.8,
#	0.9
);

@ORIGAMIBetas = (
#	0.1,
#	0.2,
#	0.3,
#	0.4,
#	0.5,
#	0.6,
#	0.7,
	0.8,
#	0.9
);

$NumFolds = 10;

###

sub GetBestAccuracy ($$)
{
	my ($application, $data_base) = @_;

	my $out_file = GetBestOutputFile ($application, $data_base);

	my $accuracy = 0;

	if (-e $out_file)
	{
		$accuracy = `cat $out_file`;
		chomp $accuracy;
		$accuracy =~ s/.*accuracy \[([^\]]*)\].*/$1/g;
	}
	else
	{
		print "no best file for $data_base!\n";
	}

	return $accuracy;
}

sub GetTrainingFile ($$)
{
	my ($data_base, $fold) = @_;

	return "$DataDir/$data_base/$data_base.$fold.training";
}

sub GetTestingFile ($$)
{
	my ($data_base, $fold) = @_;

	return "$DataDir/$data_base/$data_base.$fold.testing";
}

sub GetBestFile ($$$$)
{
	my ($application, $data_base, $fold, $type) = @_;

	my $out_file = "$OutputDir/$application/$data_base/best.$fold.$type";

	return $out_file;
}

sub GetBestOutputFile ($$)
{
	my ($application, $data_base) = @_;

	return "$OutputDir/$application/$data_base/best.out";
}

sub GetBestLogFile ($$$)
{
	my ($application, $data_base, $fold) = @_;

	my $out_file = "$OutputDir/$application/$data_base/best.$fold.log";
}

sub GetLazyOutputFile ($$$$$$)
{
	my ($data_base, $support, $confidence, $min_rules, $max_size, $ranking_size) = @_;

	# s1_c0.0001_n1_m2_l10.0.log
	my $out_file = "$OutputDirLazy/$data_base/s".$support."_c".$confidence."_n".$min_rules."_m".$max_size."_l".$ranking_size.".out";

	return $out_file;
}

sub GetClassifierCOutputFile ($$$$$$$)
{
	my ($data_base, $support, $confidence, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len) = @_;

	# s0.0001_c0.0001_n1_l10_m1_x1.0.log
	my $out_file = "$OutputDirClassifierC/$data_base/s".$support."_c".$confidence."_n".$min_num_rules."_l".$max_num_rank_rules."_m".$min_rule_len."_x".$max_rule_len.".out";

	return $out_file;
}

sub GetClassifierOOutputFile ($$$$$$$$$$$$)
{
	my ($data_base, $support, $confidence, $pattern_set, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode, $ometric, $omethod, $oordering) = @_;

	# s0.0001_c0.0001_pf_n1_l10_m1_x1_oh_es_wp_gs.0.log
	my $out_file = "$OutputDirClassifierO/$data_base/s".$support."_c".$confidence."_p".$pattern_set."_n".$min_num_rules."_l".$max_num_rank_rules."_m".$min_rule_len."_x".$max_rule_len."_o".$omode."_e".$ometric."_w".$omethod."_g".$oordering.".out";

	return $out_file;
}

sub GetRunResultFromLogFile ($$$$)
{
	my ($log_file, $accuracy, $avg_patterns, $avg_rules) = @_;

	$$accuracy = `tail -1 $log_file`;
	chomp $$accuracy;

	$$avg_patterns = $$accuracy;
	$$avg_rules = $$accuracy;

	$$accuracy	=~ s/.*accuracy \[([^\]]*)\].*/$1/;
	$$avg_patterns	=~ s/.*average patterns \[([^\]]*)\].*/$1/;
	$$avg_rules	=~ s/.*average rules \[([^\]]*)\].*/$1/;

	print "accuracy: $$accuracy, average patterns: $$avg_patterns, average rules: $$avg_rules\n";
}

sub GetRunResultFromOutputFile ($)
{
	my $out_file = $_[0];

	my $accuracy = 0;
	my $avg_patterns = 0;
	my $avg_rules = 0;

	if (-e $out_file)
	{
		open INPUT, "<$out_file";

		$accuracy = <INPUT>;
		$avg_patterns = $accuracy;
		$avg_rules = $accuracy;

		chomp $accuracy;
		chomp $avg_patterns;
		chomp $avg_rules;

		$accuracy	=~ s/.*accuracy \[([^\]]*)\].*/$1/;
		$avg_patterns	=~ s/.*avg_patterns \[([^\]]*)\].*/$1/;
		$avg_rules	=~ s/.*avg_rules \[([^\]]*)\].*/$1/;

		close INPUT;
	}

	my $RunResult = {
		ACCURACY	=> $accuracy,
		AVG_PATTERNS	=> $avg_patterns,
		AVG_RULES	=> $avg_rules
	};

	return $RunResult;
}

sub GetBestRunResult ($$)
{
	return GetRunResultFromOutputFile (GetBestOutputFile ($_[0], $_[1]));
}

sub GetLazyRunResult ($$$$$$)
{
	return GetRunResultFromOutputFile (GetLazyOutputFile ($_[0], $_[1], $_[2], $_[3], $_[4], $_[5]));
}

sub GetClassifierCRunResult ($$$$$$$)
{
	return GetRunResultFromOutputFile (GetClassifierCOutputFile ($_[0], $_[1], $_[2], $_[3], $_[4], $_[5], $_[6]));
}

sub GetClassifierORunResult ($$$$$$$$$$$$)
{
	return GetRunResultFromOutputFile (GetClassifierOOutputFile ($_[0], $_[1], $_[2], $_[3], $_[4], $_[5], $_[6], $_[7], $_[8], $_[9], $_[10], $_[11]));
}

sub MakeAppHistogramGraph ($$$$$)
{
	my ($title, $xlabel, $ylabel, $graph, $accuracy_hash) = @_;

	my ($db);

format OUTPUT_APP =
@<<<<<<<<<<<<<<<<<<<@<<<<<<<<<<<<<<<<<<<@<<<<<<<<<<<<<<<<<<<@<<<<<<<<<<<<<<<<<<<@<<<<<<<<<<<<<<<<<<<
$db,$$accuracy_hash{$db}{'lazy'},$$accuracy_hash{$db}{'classifier_c'},$$accuracy_hash{$db}{'classifier_o'},$$accuracy_hash{$db}{'classifier_or'}
.

	my $gnu_file = "$OutputDirGraphs/$graph.gnu";
	my $eps_file = "$OutputDirGraphs/$graph.eps";
	my $dat_file = "$OutputDirGraphs/$graph.dat";

	open OUTPUT_APP, ">$dat_file";

	print OUTPUT_APP "data_file           lazy                classifier_c        classifier_o        classifier_or       \n";

	foreach $data_base (@DataBases)
	{
		$db = $data_base;
		write OUTPUT_APP;
	}

	$db = 'average';
	write OUTPUT_APP;

	close OUTPUT_APP;

	open OUTPUT, ">$gnu_file";

	print OUTPUT "set term postscript eps enhanced color\n";
	print OUTPUT "set output '$eps_file'\n";
	print OUTPUT "set boxwidth 0.9 absolute\n";
	print OUTPUT "set style fill solid 1.00 border -1\n";
	print OUTPUT "set style histogram clustered gap 1 title offset character 0, 0, 0\n";
	print OUTPUT "set datafile missing '-'\n";
	print OUTPUT "set style data histograms\n";
	print OUTPUT "set xtics border in scale 1,0.5 nomirror rotate by -45 offset character 0, 0, 0 \n";
	print OUTPUT "set xtics (";

	$i = 0;

	foreach $data_base (@DataBases)
	{
		print OUTPUT "\"$data_base\" $i, ";
		$i++;
	}

	print OUTPUT "\"average\" $i";

	print OUTPUT ")\n";
	print OUTPUT "set title \"$title\"\n";
	print OUTPUT "set xlabel \"$xlabel\"\n";
	print OUTPUT "set ylabel \"$ylabel\"\n";
#	print OUTPUT "set yrange [0:1] noreverse nowriteback\n";
	print OUTPUT "plot '$dat_file' using 2:xtic(1) ti col, '' u 3 ti col, '' u 4 ti col, '' u 5 ti col";
	close OUTPUT;

	system "$GnuPlotApp $gnu_file";
}

sub MakeOMetricHistogramGraph ($$$$$)
{
	my ($title, $xlabel, $ylabel, $graph, $accuracy_hash) = @_;

	my ($db);

format OUTPUT_OMETRIC =
@<<<<<<<<<<<<<<<<<<<@<<<<<<<<<<<<<<<<<<<@<<<<<<<<<<<<<<<<<<<@<<<<<<<<<<<<<<<<<<<@<<<<<<<<<<<<<<<<<<<
$db,$$accuracy_hash{$db}{'s'},$$accuracy_hash{$db}{'c'},$$accuracy_hash{$db}{'l'},$$accuracy_hash{$db}{'m'}
.

	my $gnu_file = "$OutputDirGraphs/$graph.gnu";
	my $eps_file = "$OutputDirGraphs/$graph.eps";
	my $dat_file = "$OutputDirGraphs/$graph.dat";

	open OUTPUT_OMETRIC, ">$dat_file";

	print OUTPUT_OMETRIC "data_file           similarity          coverage            class_coverage      pair_class_coverage \n";

	foreach $data_base (@DataBases)
	{
		$db = $data_base;
		write OUTPUT_OMETRIC;
	}

	$db = 'average';
	write OUTPUT_OMETRIC;

	close OUTPUT_OMETRIC;

	open OUTPUT, ">$gnu_file";

	print OUTPUT "set term postscript eps enhanced color\n";
	print OUTPUT "set output '$eps_file'\n";
	print OUTPUT "set boxwidth 0.9 absolute\n";
	print OUTPUT "set style fill solid 1.00 border -1\n";
	print OUTPUT "set style histogram clustered gap 1 title offset character 0, 0, 0\n";
	print OUTPUT "set datafile missing '-'\n";
	print OUTPUT "set style data histograms\n";
	print OUTPUT "set xtics border in scale 1,0.5 nomirror rotate by -45 offset character 0, 0, 0 \n";
	print OUTPUT "set xtics (";

	$i = 0;

	foreach $data_base (@Common::DataBases)
	{
		print OUTPUT "\"$data_base\" $i, ";
		$i++;
	}

	print OUTPUT "\"average\" $i";

	print OUTPUT ")\n";
	print OUTPUT "set title \"$title\"\n";
	print OUTPUT "set xlabel \"$xlabel\"\n";
	print OUTPUT "set ylabel \"$ylabel\"\n";
#	print OUTPUT "set yrange [0:1] noreverse nowriteback\n";
	print OUTPUT "plot '$dat_file' using 2:xtic(1) ti col, '' u 3 ti col, '' u 4 ti col, '' u 5 ti col";
	close OUTPUT;

	system "$GnuPlotApp $gnu_file";
}
