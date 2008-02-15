package Common;


$OutputDir		= 'output';
$OutputDirLazy		= "$OutputDir/lazy";
$OutputDirClassifierC	= "$OutputDir/classifier_c";
$OutputDirClassifierO	= "$OutputDir/classifier_o";
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

$RunResult = {
	ACCURACY	=> $proc,
	AVG_PATTERNS	=> $strategy,
	AVG_RULES	=> $num,
};

sub GetBestFile ($$$)
{
	my ($application, $data_base, $type) = @_;

	my $out_file = "$OutputDir/$application/$data_base/best.$type";

	return $out_file;
}

sub GetBestOutputFile ($$)
{
	return GetBestFile ($_[0], $_[1], 'out');
}

sub GetBestLogFile ($$)
{
	return GetBestFile ($_[0], $_[1], 'log');
}

sub GetLazyOutputFile ($$$$$$)
{
	my ($data_base, $support, $confidence, $min_rules, $max_size, $ranking_size) = @_;

	my $out_file = "$OutputDirLazy/$data_base/s".$support."_c".$confidence."_n".$min_rules."_m".$max_size."_l".$ranking_size.".out";

	return $out_file;
}

sub GetClassifierCOutputFile ($$$$$$$)
{
	my ($data_base, $support, $confidence, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len) = @_;

	my $out_file = "$OutputDirClassifierC/$data_base/s".$support."_c".$confidence."_n".$min_num_rules."_l".$max_num_rank_rules."_m".$min_rule_len."_a".$max_rule_len.".out";

	return $out_file;
}

sub GetClassifierOOutputFile ($$$$$$$$$)
{
	my ($data_base, $support, $confidence, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode, $ometric) = @_;

	my $out_file = "$OutputDirClassifierO/$data_base/s".$support."_c".$confidence."_n".$min_num_rules."_l".$max_num_rank_rules."_m".$min_rule_len."_a".$max_rule_len."_o".$omode."_e".$ometric.".out";

	return $out_file;
}

sub GetRunResultFromFile ($)
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
		$accuracy =~ s/.*accuracy: (.*)$/$1/;

		chomp $avg_patterns;
		$avg_patterns =~ s/.*avg_patterns: (.*), avg_rules.*$/$1/;

		chomp $avg_rules;
		$avg_rules =~ s/.*avg_rules: (.*), accuracy.*$/$1/;

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
	return GetRunResultFromFile (GetBestOutputFile ($_[0], $_[1]));
}

sub GetLazyRunResult ($$$$$$)
{
	return GetRunResultFromFile (GetLazyOutputFile ($_[0], $_[1], $_[2], $_[3], $_[4], $_[5]));
}

sub GetClassifierCRunResult ($$$$$$$)
{
	return GetRunResultFromFile (GetClassifierCOutputFile ($_[0], $_[1], $_[2], $_[3], $_[4], $_[5], $_[6]));
}

sub GetClassifierORunResult ($$$$$$$$$)
{
	return GetRunResultFromFile (GetClassifierOOutputFile ($_[0], $_[1], $_[2], $_[3], $_[4], $_[5], $_[6], $_[7], $_[8]));
}

sub MakeAppHistogramGraph ($$$$$)
{
	my ($title, $xlabel, $ylabel, $graph, $accuracy_hash) = @_;

	my ($db);

format OUTPUT_APP =
@<<<<<<<<<<<<<<<<<<<@<<<<<<<<<<<<<<<<<<<@<<<<<<<<<<<<<<<<<<<@<<<<<<<<<<<<<<<<<<<
$db,$$accuracy_hash{$db}{'lazy'},$$accuracy_hash{$db}{'classifier_c'},$$accuracy_hash{$db}{'classifier_o'}
.

	my $gnu_file = "$OutputDirGraphs/$graph.gnu";
	my $eps_file = "$OutputDirGraphs/$graph.eps";
	my $dat_file = "$OutputDirGraphs/$graph.dat";

	open OUTPUT_APP, ">$dat_file";

	print OUTPUT_APP "data_file           lazy                classifier_c        classifier_o        \n";

	foreach $data_base (@DataBases)
	{
		$db = $data_base;
		write OUTPUT_APP;
	}

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
		print OUTPUT ", " if $i != 0;
		print OUTPUT "\"$data_base\" $i";
		$i++;
	}

	print OUTPUT ")\n";
	print OUTPUT "set title \"$title\"\n";
	print OUTPUT "set xlabel \"$xlabel\"\n";
	print OUTPUT "set ylabel \"$ylabel\"\n";
#	print OUTPUT "set yrange [0:1] noreverse nowriteback\n";
	print OUTPUT "plot '$dat_file' using 2:xtic(1) ti col, '' u 3 ti col, '' u 4 ti col";
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
		print OUTPUT ", " if $i != 0;
		print OUTPUT "\"$data_base\" $i";
		$i++;
	}

	print OUTPUT ")\n";
	print OUTPUT "set title \"$title\"\n";
	print OUTPUT "set xlabel \"$xlabel\"\n";
	print OUTPUT "set ylabel \"$ylabel\"\n";
#	print OUTPUT "set yrange [0:1] noreverse nowriteback\n";
	print OUTPUT "plot '$dat_file' using 2:xtic(1) ti col, '' u 3 ti col, '' u 4 ti col, '' u 5 ti col";
	close OUTPUT;

	system "$GnuPlotApp $gnu_file";
}
