#!/usr/bin/perl -w

use strict;

my $LAZY = "lazy_new";
my $APP_LAZY = "./$LAZY";

my $OUTPUT_DIR = "output/lazy";

sub test_data_base ($);
sub run_lazy ($$$$$$$$);

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

###

my ($support, $confidence, $min_rules, $max_size, $ranking_size, $avg_patterns, $avg_rules, $accuracy);

format OUTPUT =
support: @<<<<, confidence: @<<<<, min_rules: @<<<<<<, max_size: @<<<<<<, ranking_size: @<<<<<<, avg_patterns: @######.######, avg_rules: @######.######, accuracy: @.######
         $support,          $confidence,      $min_rules,        $max_size,             $ranking_size,         $avg_patterns,      $avg_rules,        $accuracy
.

###

sub test_data_base ($)
{
	my $data_base = $_[0];

	my $training_file	= "../data/adrianov/$data_base.training";
	my $testing_file	= "../data/adrianov/$data_base.testing";

	my @support = (
		1,
#		10,
#		100,
#		1000
	);

	my @confidence = (
#		0.001,
#		0.01,
#		0.1,
#		0.2,
#		0.3,
#		0.4,
		0.5,
#		0.6,
#		0.7,
#		0.8,
		0.9,
#		0.95,
#		0.99,
#		1
	);

	my @min_rules = (
		1,
#		10,
#		50,
#		100
	);

	my @max_size = (
		2,
#		3,
#		4,
#		5
	);

	my @ranking_size = (
		1,
#		2,
#		3
	);

	my ($best_support, $best_confidence, $best_min_rules, $best_max_size, $best_ranking_size, $best_avg_patterns, $best_avg_rules, $best_accuracy);

	$best_accuracy = 0;

	open OUTPUT, ">$OUTPUT_DIR/$data_base.out";

	my ($s, $c, $n, $m, $l);

	for ($s = 0; $s < scalar @support; $s++)
	{
		$support = $support[$s];

		for ($c = 0; $c < scalar @confidence; $c++)
		{
			$confidence = $confidence[$c];

			for ($n = 0; $n < scalar @min_rules; $n++)
			{
				$min_rules = $min_rules[$n];

				for ($m = 0; $m < scalar @max_size; $m++)
				{
					$max_size = $max_size[$m];

					for ($l = 0; $l < scalar @ranking_size; $l++)
					{
						$ranking_size = $ranking_size[$l];

						run_lazy ($data_base, $training_file, $testing_file, $support, $confidence, $min_rules, $max_size, $ranking_size);

						write OUTPUT;

						if ($accuracy > $best_accuracy)
						{
							$best_support		= $support;
							$best_confidence	= $confidence;
							$best_min_rules		= $min_rules;
							$best_max_size		= $max_size;
							$best_ranking_size	= $ranking_size;
							$best_avg_patterns	= $avg_patterns;
							$best_avg_rules		= $avg_rules;
							$best_accuracy		= $accuracy;
						}
					}
				}
			}
		}
	}

	print OUTPUT "\n";
	print OUTPUT "best: support [$best_support], confidence [$best_confidence], min_rules [$best_min_rules], max_size [$best_max_size], ranking_size [$best_ranking_size], avg_patterns [$best_avg_patterns], avg_rules [$best_avg_rules], accuracy [$best_accuracy]\n";

	close OUTPUT;
}

sub run_lazy ($$$$$$$$)
{
	my ($data_base, $training_file, $testing_file, $support, $confidence, $min_rules, $max_rule, $ranking_size) = @_;

	print "$APP_LAZY -i $training_file -t $testing_file -s $support -c $confidence -n $min_rules -m $max_rule -l $ranking_size\n";
	my @result = `$APP_LAZY -i $training_file -t $testing_file -s $support -c $confidence -n $min_rules -m $max_rule -l $ranking_size`;

	$accuracy = pop @result;

	chomp $accuracy;

	$avg_patterns = $accuracy;
	$avg_rules = $accuracy;

	$accuracy	=~ s/^accuracy \[(.*)\], average patterns.*/$1/;
	$avg_patterns	=~ s/.*average patterns \[(.*)\], average rules.*/$1/;
	$avg_rules	=~ s/.*average rules \[(.*)\]$/$1/;

	print "accuracy: $accuracy, average patterns: $avg_patterns, average rules: $avg_rules\n";
}
