#!/usr/bin/perl -w

my $LAZY = "lazy";
my $APP_LAZY = "./lazy_class";

my $CLASSIFIER = "classifier";
my $APP_CLASSIFIER = "../src/$CLASSIFIER";

my $OUTPUT_DIR = "output/compare";

sub test_data_base ($);
sub run_all ($$$$$$);
sub run_lazy ($$$$$$$$);
sub run_classifier_c ($$$$$$$);
sub run_classifier_o ($$$$$$$$$);
sub compare_all ($$$$$$$$$);

my @data_bases = (
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
	my $support_i = 0.1;
	my $support_f = 1;
	my $support_s = 0.3;

	my $confidence_i = 0.1;
	my $confidence_f = 1;
	my $confidence_s = 0.3;

	my $min_rules		= 1000000;
	my $max_size		= 10;
	my $ranking_size	= 1000000;
	my $min_rule_len	= 1;
	my $max_rule_len	= 10;

	my $data_base = $_[0];

	my $training_file	= "../data/adrianov/$data_base.training";
	my $testing_file	= "../data/adrianov/$data_base.testing";
	my $class_file		= "../data/adrianov/$data_base.out";

	my ($support, $confidene);

	for ($support = $support_i; $support <= $support_f; $support += $support_s)
	{
		for ($confidence = $confidence_i; $confidence <= $confidence_f; $confidence += $confidence_s)
		{
#			run_all ($data_base, $training_file, $testing_file, $support, $confidence, 's');
#			run_all ($data_base, $training_file, $testing_file, $support, $confidence, 'c');
#			run_all ($data_base, $training_file, $testing_file, $support, $confidence, 'b');
#			run_all ($data_base, $training_file, $testing_file, $support, $confidence, 'a');

			my $lazy_out = run_lazy ($data_base, $training_file, $testing_file, 1, $confidence, $min_rules, $max_size, $ranking_size);
			my $cl_c_out = run_classifier_c ($data_base, $training_file, $testing_file, $support, $confidence, $min_rule_len, $max_rule_len);
			my $cl_s_out = run_classifier_o ($data_base, $training_file, $testing_file, $support, $confidence, $min_rule_len, $max_rule_len, 'h', 's');
			my $cl_o_out = run_classifier_o ($data_base, $training_file, $testing_file, $support, $confidence, $min_rule_len, $max_rule_len, 'h', 'c');
			my $cl_b_out = run_classifier_o ($data_base, $training_file, $testing_file, $support, $confidence, $min_rule_len, $max_rule_len, 'h', 'b');
			my $cl_a_out = run_classifier_o ($data_base, $training_file, $testing_file, $support, $confidence, $min_rule_len, $max_rule_len, 'h', 'a');

			my $compare_file = "$OUTPUT_DIR/$data_base.s$support.c$confidence.compare";

			compare_all ($data_base, $compare_file, $class_file, $lazy_out, $cl_c_out, $cl_s_out, $cl_o_out, $cl_b_out, $cl_a_out);
		}
	}
}

sub run_all ($$$$$$)
{
	my ($data_base, $training_file, $testing_file, $support, $confidence, $ort_metric) = @_;

	run_lazy ($data_base, $training_file, $testing_file, $support, $confidence, $min_rules, $max_size, $ranking_size);
	run_classifier_c ($data_base, $training_file, $testing_file, $support, $confidence, $min_rule_len, $max_rule_len);
	run_classifier_o ($data_base, $training_file, $testing_file, $support, $confidence, $min_rule_len, $max_rule_len, 'h', $ort_metric);
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
