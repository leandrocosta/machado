#!/usr/bin/perl -w

my $LAZY = "lazy";
my $APP_LAZY = "./lazy_acc";

my $CLASSIFIER = "classifier";
my $APP_CLASSIFIER = "../src/$CLASSIFIER";

my $OUTPUT_DIR = "output/lazy";

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
#	'led7.ac',
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

###

my ($confidence, $min_rules, $max_size, $ranking_size, $accuracy);

format OUTPUT =
confidence: @<<<, min_rules: @<<<<<<, max_size: @<, ranking_size: @<<<<<<, accuracy: @#####.###
            $confidence,     $min_rules,        $max_size,        $ranking_size,     $accuracy
.

###

sub test_data_base ($)
{
	my $confidence_i	= 0.1;
	my $confidence_f	= 1;
	my $confidence_s	= 0.3;

	my $min_rules_i	= 1000;
	my $min_rules_f	= 1000000;
	my $min_rules_s	= 1000;

	my $max_size_i	= 2;
	my $max_size_f	= 4;
	my $max_size_s	= 1;

#	my $ranking_size_i	= 1000;
#	my $ranking_size_f	= 1000000;
#	my $ranking_size_s	= 1000;

	my $data_base = $_[0];

	my $training_file	= "../data/adrianov/$data_base.training";
	my $testing_file	= "../data/adrianov/$data_base.testing";
	my $class_file		= "../data/adrianov/$data_base.out";

	my ($best_confidence, $best_min_rules, $best_max_size, $best_ranking_size, $best_accuracy);
	$best_accuracy = 0;

	open OUTPUT, ">$OUTPUT_DIR/lazy_acc.$data_base.out";

	for ($confidence = $confidence_i; $confidence <= $confidence_f; $confidence += $confidence_s)
	{
		for ($min_rules = $min_rules_i; $min_rules <= $min_rules_f; $min_rules *= $min_rules_s)
		{
			for ($max_size = $max_size_i; $max_size <= $max_size_f; $max_size += $max_size_s)
			{
#				for ($ranking_size = $ranking_size_i; $ranking_size <= $ranking_size_f; $ranking_size *= $ranking_size_s)
#				{
					$ranking_size = $min_rules;

					$accuracy = run_lazy ($data_base, $training_file, $testing_file, 1, $confidence, $min_rules, $max_size, $ranking_size);

#					print OUTPUT "confidence: $confidence, min_rules: %min_rules, max_size: $max_size, ranking_size: $ranking_size, accuracy: $lazy_acc\n";
					write OUTPUT;

					if ($accuracy > $best_accuracy)
					{
						$best_confidence	= $confidence;
						$best_min_rules		= $min_rules;
						$best_max_size		= $max_size;
						$best_ranking_size	= $ranking_size;
						$best_accuracy		= $accuracy;
					}
#				}
			}
		}
	}

	print OUTPUT "best:\n";
	print OUTPUT "best: confidence: $best_confidence, min_rules: $best_min_rules, max_size: $best_max_size, ranking_size: $best_ranking_size, accuracy: $best_accuracy\n";

	close OUTPUT;
}

sub run_lazy ($$$$$$$$)
{
	my ($data_base, $training_file, $testing_file, $support, $confidence, $min_rules, $max_rule, $ranking_size) = @_;

	my $output_file = "$OUTPUT_DIR/$LAZY.$data_base.s$support.c$confidence.n$min_rules.m$max_rule.l$ranking_size.out";

	print "$APP_LAZY -i $training_file -t $testing_file -s $support -c $confidence -n $min_rules -m $max_rule -l $ranking_size\n";
	my $accuracy = `$APP_LAZY -i $training_file -t $testing_file -s $support -c $confidence -n $min_rules -m $max_rule -l $ranking_size`;

	chomp $accuracy;

	print "accuracy: $accuracy\n";

	$accuracy;
}
