#!/usr/bin/perl -w

use strict;

my $CLASSIFIER = "classifier";
my $APP_CLASSIFIER = "./$CLASSIFIER";

my $OUTPUT_DIR = "output/classifier";

sub test_data_base ($);
sub run_classifier_c ($$$$$$$$$);
sub run_classifier_o ($$$$$$$$$$$);

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

#system "cp ../src/$CLASSIFIER .";

foreach $data_base (sort @data_bases)
{
	print "base: $data_base\n";

	test_data_base ($data_base);
}

exit;

###

my ($support, $confidence, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode, $ometric, $avg_patterns, $avg_rules, $accuracy);

format OUTPUT =
support: @<<<<, confidence: @<<<<, min_num_rules: @<<<, max_num_rank_rules: @<<<, min_rule_len: @<, max_rule_len: @<<<<<<, omode: @<, ometric: @<, avg_patterns: @######.######, avg_rules: @######.######, accuracy: @#####.###
         $support,         $confidence,         $min_num_rules,           $max_num_rank_rules, $min_rule_len,   $max_rule_len,  $omode,      $ometric,           $avg_patterns,             $avg_rules,               $accuracy
.

###

sub test_data_base ($)
{
	my $data_base = $_[0];

	my $training_file	= "../data/adrianov/$data_base.training";
	my $testing_file	= "../data/adrianov/$data_base.testing";

	my @support = (
		0.001,
#		0.01,
#		0.1,
#		0.2,
#		0.3,
#		0.4,
#		0.5,
#		0.6,
#		0.7,
#		0.8,
#		0.9,
#		0.95,
#		0.99,
#		1
	);

	my @confidence = (
#		0.001,
#		0.01,
#		0.1,
#		0.2,
#		0.3,
#		0.4,
#		0.5,
#		0.6,
#		0.7,
#		0.8,
		0.9,
#		0.95,
#		0.99,
#		1
	);

	my @min_num_rules = (
		1,
#		10,
#		50,
#		100
	);

	my @max_num_rank_rules = (
		1,
#		10,
#		100,
#		1000
	);

	my @min_rule_len = (
		1,
#		2,
#		3
	);

	my @max_rule_len = (
		1,
#		2,
#		3,
#		4,
#		5
	);

	my @omode = (
		'h',
#		'p'
	);

	my @ometric = (
		's',
		'c',
#		'b',
		'l',
		'm',
#		'a'
	);

	my ($best_support, $best_confidence, $best_min_num_rules, $best_max_num_rank_rules, $best_min_rule_len, $best_max_rule_len, $best_omode, $best_ometric, $best_avg_patterns, $best_avg_rules, $best_accuracy);

	$best_accuracy = 0;

	open OUTPUT, ">$OUTPUT_DIR/$CLASSIFIER.$data_base.out";

	my ($s, $c, $n, $l, $m, $a, $o, $e);

	for ($s = 0; $s < scalar @support; $s++)
	{
		$support = $support[$s];

		for ($c = 0; $c < scalar @confidence; $c++)
		{
			$confidence = $confidence[$c];

			for ($n = 0; $n < scalar @min_num_rules; $n++)
			{
				$min_num_rules = $min_num_rules[$n];

				for ($l = 0; $l < scalar @max_num_rank_rules; $l++)
				{
					$max_num_rank_rules = $max_num_rank_rules[$l];

					for ($m = 0; $m < scalar @min_rule_len; $m++)
					{
						$min_rule_len = $min_rule_len[$m];

						for ($a = 0; $a < scalar @max_rule_len; $a++)
						{
							$max_rule_len = $max_rule_len[$a];

							for ($o = 0; $o < scalar @omode; $o++)
							{
								$omode = $omode[$o];

								for ($e = 0; $e < scalar @ometric; $e++)
								{
									$ometric = $ometric[$e];

									run_classifier_o ($data_base, $training_file, $testing_file, $support, $confidence, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode, $ometric);

									write OUTPUT;

									if ($accuracy > $best_accuracy)
									{
										$best_support			= $support;
										$best_confidence		= $confidence;
										$best_min_num_rules		= $min_num_rules;
										$best_max_num_rank_rules	= $max_num_rank_rules;
										$best_min_rule_len		= $min_rule_len;
										$best_max_rule_len		= $max_rule_len;
										$best_omode			= $omode;
										$best_ometric			= $ometric;
										$best_avg_patterns		= $avg_patterns;
										$best_avg_rules			= $avg_rules;
										$best_accuracy			= $accuracy;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	print OUTPUT "best: support [$best_support], confidence [$best_confidence], min_num_rules [$best_min_num_rules], max_num_rank_rules [$best_max_num_rank_rules], min_rule_len [$best_min_rule_len], max_rule_len [$best_max_rule_len], omode [$best_omode], ometric [$best_ometric], avg_patterns [$best_avg_patterns], avg_rules [$best_avg_rules], accuracy [$best_accuracy]\n";

	close OUTPUT;
}

sub run_classifier_o ($$$$$$$$$$$)
{
	my ($data_base, $training_file, $testing_file, $support, $confidence, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode, $ometric) = @_;

	print "$APP_CLASSIFIER -i $training_file -t $testing_file -s $support -c $confidence -n $min_num_rules -l $max_num_rank_rules -m $min_rule_len -a $max_rule_len -r o -o $omode -e $ometric -d -1\n";
	my @result = `$APP_CLASSIFIER -i $training_file -t $testing_file -s $support -c $confidence -n $min_num_rules -l $max_num_rank_rules -m $min_rule_len -a $max_rule_len -r o -o $omode -e $ometric -d -1`;

	$accuracy = pop @result;

	chomp $accuracy;

	$avg_patterns = $accuracy;
	$avg_rules = $accuracy;

	$accuracy	=~ s/^accuracy \[(.*)\], average patterns.*/$1/;
	$avg_patterns	=~ s/.*average patterns \[(.*)\], average rules.*/$1/;
	$avg_rules	=~ s/.*average rules \[(.*)\]$/$1/;

	print "accuracy: $accuracy, average patterns: $avg_patterns, average rules: $avg_rules\n";
}
