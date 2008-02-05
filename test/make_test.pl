#!/usr/bin/perl -w

my $LAZY = "lazy";
my $APP_LAZY = "../../testing_10_per_cent/$LAZY";

my $CLASSIFIER = "classifier";
my $APP_CLASSIFIER = "../src/$CLASSIFIER";

my $OUTPUT_DIR = "output";

sub test_data_base ($);
sub run_all ($$$$$$);
sub run_lazy ($$$$$$$$);
sub run_classifier_c ($$$$$$$);
sub run_classifier_o ($$$$$$$$$);
sub compare_all ($$$$$$$$$);

my @data_bases = (
	'tic-tac.ac',
	'lymph.ac',
	'austra.ac',
	'breast.ac',
	'iris.ac',
	'labor.ac',
	'wine.ac'
);

my $data_base;

foreach $data_base (sort @data_bases)
{
	print "base: $data_base\n";

	test_data_base ($data_base);
}

exit;


my ($first, $second, $yes_yes, $yes_no, $no_yes, $no_no_y, $no_no_n);

format OUTPUT =
------------------------------------------------
|            | YES @<<<<<<<< | NO @<<<<<<<<<<< |
$second,       $second
------------------------------------------------
|YES @<<<<<< | @<<<<<<<<<<<< | @<<<<<<<<<<<<<< |
$first,   $yes_yes,       $yes_no
------------------------------------------------
|NO @<<<<<<< | @<<<<<<<<<<<< | @<<<<< | @<<<<< |
$first,    $no_yes,        $no_no_y,$no_no_n
------------------------------------------------
.


sub test_data_base ($)
{
	my $support_i = 0.9;
	my $support_f = 0.1;
	my $support_s = 0.1;

	my $confidence_i = 0.9;
	my $confidence_f = 0.1;
	my $confidence_s = 0.1;

	my $data_base = $_[0];

	my $training_file	= "../data/adrianov/$data_base.training";
	my $testing_file	= "../data/adrianov/$data_base.testing";
	my $class_file		= "../data/adrianov/$data_base.out";

	my ($support, $confidene);

	for ($support = $support_i; $support >= $support_f; $support -= $support_s)
	{
		for ($confidence = $confidence_i; $confidence >= $confidence_f; $confidence -= $confidence_s)
		{
#			run_all ($data_base, $training_file, $testing_file, $support, $confidence, 's');
#			run_all ($data_base, $training_file, $testing_file, $support, $confidence, 'c');
#			run_all ($data_base, $training_file, $testing_file, $support, $confidence, 'b');
#			run_all ($data_base, $training_file, $testing_file, $support, $confidence, 'a');

			my $lazy_out = run_lazy ($data_base, $training_file, $testing_file, 1, $confidence, 1, 1000, 1000);
			my $cl_c_out = run_classifier_c ($data_base, $training_file, $testing_file, $support, $confidence, 1, 1000);
			my $cl_s_out = run_classifier_o ($data_base, $training_file, $testing_file, $support, $confidence, 1, 1000, 'h', 's');
			my $cl_o_out = run_classifier_o ($data_base, $training_file, $testing_file, $support, $confidence, 1, 1000, 'h', 'c');
			my $cl_b_out = run_classifier_o ($data_base, $training_file, $testing_file, $support, $confidence, 1, 1000, 'h', 'b');
			my $cl_a_out = run_classifier_o ($data_base, $training_file, $testing_file, $support, $confidence, 1, 1000, 'h', 'a');


			my $compare_file = "output/$data_base.s$support.c$confidence.compare";

			compare_all ($data_base, $compare_file, $class_file, $lazy_out, $cl_c_out, $cl_s_out, $cl_o_out, $cl_b_out, $cl_a_out);
		}
	}
}

sub run_all ($$$$$$)
{
	my ($data_base, $training_file, $testing_file, $support, $confidence, $ort_metric) = @_;

	run_lazy ($data_base, $training_file, $testing_file, $support, $confidence, 1, 1000, 1000);
	run_classifier_c ($data_base, $training_file, $testing_file, $support, $confidence, 1, 1000);
	run_classifier_o ($data_base, $training_file, $testing_file, $support, $confidence, 1, 1000, 'h', $ort_metric);
}

sub run_lazy ($$$$$$$$)
{
	my ($data_base, $training_file, $testing_file, $support, $confidence, $min_rules, $max_rule, $ranking_size) = @_;

	my $output_file = "$OUTPUT_DIR/$LAZY.$data_base.s$support.c$confidence.n$min_rules.m$max_rule.l$ranking_size.out";

	print "$APP_LAZY -i $training_file -t $testing_file -s $support -c $confidence -n $min_rules -m $max_rule -l $ranking_size >$output_file\n";
	system "$APP_LAZY -i $training_file -t $testing_file -s $support -c $confidence -n $min_rules -m $max_rule -l $ranking_size >$output_file";

	$output_file;
}

sub run_classifier_c ($$$$$$$)
{
	my ($data_base, $training_file, $testing_file, $support, $confidence, $min_rule_len, $max_rule_len) = @_;

	my $output_file = "$OUTPUT_DIR/$CLASSIFIER.c.$data_base.s$support.c$confidence.m$min_rule_len.a$max_rule_len.out";

	print "$APP_CLASSIFIER -i $training_file -t $testing_file -s $support -c $confidence -m $min_rule_len -a $max_rule_len -r c >$output_file\n";
	system "$APP_CLASSIFIER -i $training_file -t $testing_file -s $support -c $confidence -m $min_rule_len -a $max_rule_len -r c >$output_file";

	$output_file;
}

sub run_classifier_o ($$$$$$$$$)
{
	my ($data_base, $training_file, $testing_file, $support, $confidence, $min_rule_len, $max_rule_len, $ort_mode, $ort_metric) = @_;

	my $output_file = "$OUTPUT_DIR/$CLASSIFIER.o.$data_base.s$support.c$confidence.m$min_rule_len.a$max_rule_len.o$ort_mode.e$ort_metric.out";

	print "$APP_CLASSIFIER -i $training_file -t $testing_file -s $support -c $confidence -m $min_rule_len -a $max_rule_len -r o -o $ort_mode -e $ort_metric >$output_file\n";
	system "$APP_CLASSIFIER -i $training_file -t $testing_file -s $support -c $confidence -m $min_rule_len -a $max_rule_len -r o -o $ort_mode -e $ort_metric >$output_file";

	$output_file;
}

sub compare_all ($$$$$$$$$)
{
	my ($data_base, $compare_file, $class_file, $lazy_out, $cl_c_out, $cl_s_out, $cl_o_out, $cl_b_out, $cl_a_out) = @_;

	open TESTING,		"<$class_file";
	open LAZY,		"<$lazy_out";
	open CLASSIFIER_C,	"<$cl_c_out";
	open CLASSIFIER_O,	"<$cl_b_out";

	my $tests = 0;

	my $lazy_correct = 0;	# classificações corretas do lazy
	my $freq_correct = 0;	# classificações corretas do frequence
	my $orth_correct = 0;	# classificações corretas do orthogonality

	my $yeslazy_yesfreq = 0;	# lazy e frequence corretos
	my $yeslazy_nofreq = 0;		# lazy correto e frequence incorreto
	my $nolazy_yesfreq = 0;		# lazy incorreto e frequence correto
	my $nolazy_nofreq_coinc = 0;	# lazy e frequence incorretos iguais
	my $nolazy_nofreq_nocoinc = 0;	# lazy e frequence incorretos diferentes

	my $yeslazy_yesorth = 0;	# lazy e orthogonality corretos
	my $yeslazy_noorth = 0;		# lazy correto e orthogonality incorreto
	my $nolazy_yesorth = 0;		# lazy incorreto e orthogonality correto
	my $nolazy_noorth_coinc = 0;	# lazy e orthogonality incorretos iguais
	my $nolazy_noorth_nocoinc = 0;	# lazy e orthogonality incorretos diferentes

	my $yesfreq_yesorth = 0;	# frequence e orthogonality corretos
	my $yesfreq_noorth = 0;		# frequence correto e orthogonality incorreto
	my $nofreq_yesorth = 0;		# frequence incorreto e orthogonality correto
	my $nofreq_noorth_coinc = 0;	# frequence e orthogonality incorretos iguais
	my $nofreq_noorth_nocoinc = 0;	# frequence e orthogonality incorretos diferentes

	while (<TESTING>)
	{
		my $class_testing	= $_;
		my $class_lazy		= <LAZY>;
		my $class_freq		= <CLASSIFIER_C>;
		my $class_orth		= <CLASSIFIER_O>;

		chomp $class_testing;
		chomp $class_lazy;
		chomp $class_freq;
		chomp $class_orth;

		$tests++;

		$lazy_correct++ if $class_testing eq $class_lazy;
		$freq_correct++ if $class_testing eq $class_freq;
		$orth_correct++ if $class_testing eq $class_orth;

		$yeslazy_yesfreq++ if $class_testing eq $class_lazy and $class_testing eq $class_freq;
		$yeslazy_nofreq++ if $class_testing eq $class_lazy and $class_testing ne $class_freq;
		$nolazy_yesfreq++ if $class_testing ne $class_lazy and $class_testing eq $class_freq;
		$nolazy_nofreq_coinc++ if $class_testing ne $class_lazy and $class_testing ne $class_freq and $class_lazy eq $class_freq;
		$nolazy_nofreq_nocoinc++ if $class_testing ne $class_lazy and $class_testing ne $class_freq and $class_lazy ne $class_freq;

		$yeslazy_yesorth++ if $class_testing eq $class_lazy and $class_testing eq $class_orth;
		$yeslazy_noorth++ if $class_testing eq $class_lazy and $class_testing ne $class_orth;
		$nolazy_yesorth++ if $class_testing ne $class_lazy and $class_testing eq $class_orth;
		$nolazy_noorth_coinc++ if $class_testing ne $class_lazy and $class_testing ne $class_orth and $class_lazy eq $class_orth;
		$nolazy_noorth_nocoinc++ if $class_testing ne $class_lazy and $class_testing ne $class_orth and $class_lazy ne $class_orth;

		$yesfreq_yesorth++ if $class_testing eq $class_freq and $class_testing eq $class_orth;
		$yesfreq_noorth++ if $class_testing eq $class_freq and $class_testing ne $class_orth;
		$nofreq_yesorth++ if $class_testing ne $class_freq and $class_testing eq $class_orth;
		$nofreq_noorth_coinc++ if $class_testing ne $class_freq and $class_testing ne $class_orth and $class_freq eq $class_orth;
		$nofreq_noorth_nocoinc++ if $class_testing ne $class_freq and $class_testing ne $class_orth and $class_freq ne $class_orth;
	}

	close TESTING;
	close LAZY;
	close CLASSIFIER_C;
	close CLASSIFIER_O;

	open OUTPUT, ">$compare_file";

	print OUTPUT "base: $data_base\n";
	print OUTPUT "tests: $tests\n";
	print OUTPUT "---\n";
	print OUTPUT "lazy_correct: $lazy_correct\n";
	print OUTPUT "freq_correct: $freq_correct\n";
	print OUTPUT "orth_correct: $orth_correct\n";

	if ($lazy_correct > $freq_correct && $lazy_correct > $orth_correct)
	{
		print OUTPUT "best: lazy\n";
	}
	elsif ($freq_correct > $orth_correct)
	{
		print OUTPUT "best: freq\n";
	}
	else
	{
		print OUTPUT "best: orth\n";
	}

	print OUTPUT "---\n";

	$first  = 'LAZY';
	$second = 'FREQ';
	$yes_yes = $yeslazy_yesfreq;
	$yes_no = $yeslazy_nofreq;
	$no_yes = $nolazy_yesfreq;
	$no_no_y = $nolazy_nofreq_coinc;
	$no_no_n = $nolazy_nofreq_nocoinc;

#	$~ = 'OUTPUT';
	write OUTPUT;

	$first  = 'LAZY';
	$second = 'ORTH';
	$yes_yes = $yeslazy_yesorth;
	$yes_no = $yeslazy_noorth;
	$no_yes = $nolazy_yesorth;
	$no_no_y = $nolazy_noorth_coinc;
	$no_no_n = $nolazy_noorth_nocoinc;

#	$~ = 'OUTPUT';
	write OUTPUT;

	$first  = 'FREQ';
	$second = 'ORTH';
	$yes_yes = $yesfreq_yesorth;
	$yes_no = $yesfreq_noorth;
	$no_yes = $nofreq_yesorth;
	$no_no_y = $nofreq_noorth_coinc;
	$no_no_n = $nofreq_noorth_nocoinc;

#	$~ = 'OUTPUT';
	write OUTPUT;

#	print "---\n";

	close OUTPUT;
}

=comment




















#SUPPORT=0.1
#CONFIDENCE=0.9
#SUPPORT=0.1
#CONFIDENCE=0.1
#SUPPORT=0.9
#CONFIDENCE=0.1
SUPPORT=$1
CONFIDENCE=$1

MIN_RULE_LEN=1
MAX_RULE_LEN=2

###

TEST_FILE=test_s$SUPPORT.c$CONFIDENCE.m$MIN_RULE_LEN.a$MAX_RULE_LEN.out

CLASSIFIER=classifier
APP_CLASSIFIER=../src/$CLASSIFIER

LAZY=lazy
APP_LAZY=../../testing_10_per_cent/$LAZY

APP_COMPARE=./compare.pl

TIC_TAC_BASE=tic-tac.ac
TIC_TAC_TRAINING=../data/adrianov/$TIC_TAC_BASE.training
TIC_TAC_TESTING=../data/adrianov/$TIC_TAC_BASE.testing

TIC_TAC_BASE_CLASS=../data/adrianov/$TIC_TAC_BASE.out
TIC_TAC_LAZY_CLASS=$LAZY.$TIC_TAC_BASE.out
TIC_TAC_C_CLASS=$CLASSIFIER.c.$TIC_TAC_BASE.out
TIC_TAC_O_CLASS=$CLASSIFIER.o.$TIC_TAC_BASE.out

LYMPH_BASE=lymph.ac
LYMPH_TRAINING=../data/adrianov/$LYMPH_BASE.training
LYMPH_TESTING=../data/adrianov/$LYMPH_BASE.testing

LYMPH_BASE_CLASS=../data/adrianov/$LYMPH_BASE.out
LYMPH_LAZY_CLASS=$LAZY.$LYMPH_BASE.out
LYMPH_C_CLASS=$CLASSIFIER.c.$LYMPH_BASE.out
LYMPH_O_CLASS=$CLASSIFIER.o.$LYMPH_BASE.out

AUSTRA_BASE=austra.ac
AUSTRA_TRAINING=../data/adrianov/$AUSTRA_BASE.training
AUSTRA_TESTING=../data/adrianov/$AUSTRA_BASE.testing

AUSTRA_BASE_CLASS=../data/adrianov/$AUSTRA_BASE.out
AUSTRA_LAZY_CLASS=$LAZY.$AUSTRA_BASE.out
AUSTRA_C_CLASS=$CLASSIFIER.c.$AUSTRA_BASE.out
AUSTRA_O_CLASS=$CLASSIFIER.o.$AUSTRA_BASE.out

BREAST_BASE=breast.ac
BREAST_TRAINING=../data/adrianov/$BREAST_BASE.training
BREAST_TESTING=../data/adrianov/$BREAST_BASE.testing

BREAST_BASE_CLASS=../data/adrianov/$BREAST_BASE.out
BREAST_LAZY_CLASS=$LAZY.$BREAST_BASE.out
BREAST_C_CLASS=$CLASSIFIER.c.$BREAST_BASE.out
BREAST_O_CLASS=$CLASSIFIER.o.$BREAST_BASE.out

IRIS_BASE=iris.ac
IRIS_TRAINING=../data/adrianov/$IRIS_BASE.training
IRIS_TESTING=../data/adrianov/$IRIS_BASE.testing

IRIS_BASE_CLASS=../data/adrianov/$IRIS_BASE.out
IRIS_LAZY_CLASS=$LAZY.$IRIS_BASE.out
IRIS_C_CLASS=$CLASSIFIER.c.$IRIS_BASE.out
IRIS_O_CLASS=$CLASSIFIER.o.$IRIS_BASE.out

LABOR_BASE=labor.ac
LABOR_TRAINING=../data/adrianov/$LABOR_BASE.training
LABOR_TESTING=../data/adrianov/$LABOR_BASE.testing

LABOR_BASE_CLASS=../data/adrianov/$LABOR_BASE.out
LABOR_LAZY_CLASS=$LAZY.$LABOR_BASE.out
LABOR_C_CLASS=$CLASSIFIER.c.$LABOR_BASE.out
LABOR_O_CLASS=$CLASSIFIER.o.$LABOR_BASE.out

WINE_BASE=wine.ac
WINE_TRAINING=../data/adrianov/$WINE_BASE.training
WINE_TESTING=../data/adrianov/$WINE_BASE.testing

WINE_BASE_CLASS=../data/adrianov/$WINE_BASE.out
WINE_LAZY_CLASS=$LAZY.$WINE_BASE.out
WINE_C_CLASS=$CLASSIFIER.c.$WINE_BASE.out
WINE_O_CLASS=$CLASSIFIER.o.$WINE_BASE.out

rm -rf $TEST_FILE

$APP_LAZY -i $TIC_TAC_TRAINING -t $TIC_TAC_TESTING -s 1 -c $CONFIDENCE -m 10 >$TIC_TAC_LAZY_CLASS
$APP_CLASSIFIER -i $TIC_TAC_TRAINING -t $TIC_TAC_TESTING -s $SUPPORT -c $CONFIDENCE -m $MIN_RULE_LEN -a $MAX_RULE_LEN -r c -d -1 >$TIC_TAC_C_CLASS
$APP_CLASSIFIER -i $TIC_TAC_TRAINING -t $TIC_TAC_TESTING -s $SUPPORT -c $CONFIDENCE -m $MIN_RULE_LEN -a $MAX_RULE_LEN -r o -d -1 >$TIC_TAC_O_CLASS
$APP_COMPARE $TIC_TAC_BASE $TIC_TAC_BASE_CLASS $TIC_TAC_LAZY_CLASS $TIC_TAC_C_CLASS $TIC_TAC_O_CLASS >>$TEST_FILE

$APP_LAZY -i $LYMPH_TRAINING -t $LYMPH_TESTING -s 1 -c $CONFIDENCE -m 10 >$LYMPH_LAZY_CLASS
$APP_CLASSIFIER -i $LYMPH_TRAINING -t $LYMPH_TESTING -s $SUPPORT -c $CONFIDENCE -m $MIN_RULE_LEN -a $MAX_RULE_LEN -r c -d -1 >$LYMPH_C_CLASS
$APP_CLASSIFIER -i $LYMPH_TRAINING -t $LYMPH_TESTING -s $SUPPORT -c $CONFIDENCE -m $MIN_RULE_LEN -a $MAX_RULE_LEN -r o -d -1 >$LYMPH_O_CLASS
$APP_COMPARE $LYMPH_BASE $LYMPH_BASE_CLASS $LYMPH_LAZY_CLASS $LYMPH_C_CLASS $LYMPH_O_CLASS >>$TEST_FILE

# (BETTER - SUPPORT 0.1, CONFIDENCE 0.1)
$APP_LAZY -i $AUSTRA_TRAINING -t $AUSTRA_TESTING -s 1 -c $CONFIDENCE -m 10 >$AUSTRA_LAZY_CLASS
$APP_CLASSIFIER -i $AUSTRA_TRAINING -t $AUSTRA_TESTING -s $SUPPORT -c $CONFIDENCE -m $MIN_RULE_LEN -a $MAX_RULE_LEN -r c -d -1 >$AUSTRA_C_CLASS
$APP_CLASSIFIER -i $AUSTRA_TRAINING -t $AUSTRA_TESTING -s $SUPPORT -c $CONFIDENCE -m $MIN_RULE_LEN -a $MAX_RULE_LEN -r o -d -1 >$AUSTRA_O_CLASS
$APP_COMPARE $AUSTRA_BASE $AUSTRA_BASE_CLASS $AUSTRA_LAZY_CLASS $AUSTRA_C_CLASS $AUSTRA_O_CLASS >>$TEST_FILE

$APP_LAZY -i $BREAST_TRAINING -t $BREAST_TESTING -s 1 -c $CONFIDENCE -m 10 >$BREAST_LAZY_CLASS
$APP_CLASSIFIER -i $BREAST_TRAINING -t $BREAST_TESTING -s $SUPPORT -c $CONFIDENCE -m $MIN_RULE_LEN -a $MAX_RULE_LEN -r c -d -1 >$BREAST_C_CLASS
$APP_CLASSIFIER -i $BREAST_TRAINING -t $BREAST_TESTING -s $SUPPORT -c $CONFIDENCE -m $MIN_RULE_LEN -a $MAX_RULE_LEN -r o -d -1 >$BREAST_O_CLASS
$APP_COMPARE $BREAST_BASE $BREAST_BASE_CLASS $BREAST_LAZY_CLASS $BREAST_C_CLASS $BREAST_O_CLASS >>$TEST_FILE

$APP_LAZY -i $IRIS_TRAINING -t $IRIS_TESTING -s 1 -c $CONFIDENCE -m 10 >$IRIS_LAZY_CLASS
$APP_CLASSIFIER -i $IRIS_TRAINING -t $IRIS_TESTING -s $SUPPORT -c $CONFIDENCE -m $MIN_RULE_LEN -a $MAX_RULE_LEN -r c -d -1 >$IRIS_C_CLASS
$APP_CLASSIFIER -i $IRIS_TRAINING -t $IRIS_TESTING -s $SUPPORT -c $CONFIDENCE -m $MIN_RULE_LEN -a $MAX_RULE_LEN -r o -d -1 >$IRIS_O_CLASS
$APP_COMPARE $IRIS_BASE $IRIS_BASE_CLASS $IRIS_LAZY_CLASS $IRIS_C_CLASS $IRIS_O_CLASS >>$TEST_FILE

$APP_LAZY -i $LABOR_TRAINING -t $LABOR_TESTING -s 1 -c $CONFIDENCE -m 10 >$LABOR_LAZY_CLASS
$APP_CLASSIFIER -i $LABOR_TRAINING -t $LABOR_TESTING -s $SUPPORT -c $CONFIDENCE -m $MIN_RULE_LEN -a $MAX_RULE_LEN -r c -d -1 >$LABOR_C_CLASS
$APP_CLASSIFIER -i $LABOR_TRAINING -t $LABOR_TESTING -s $SUPPORT -c $CONFIDENCE -m $MIN_RULE_LEN -a $MAX_RULE_LEN -r o -d -1 >$LABOR_O_CLASS
$APP_COMPARE $LABOR_BASE $LABOR_BASE_CLASS $LABOR_LAZY_CLASS $LABOR_C_CLASS $LABOR_O_CLASS >>$TEST_FILE

