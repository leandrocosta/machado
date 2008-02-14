#!/usr/bin/perl -w

use strict;

my $LAZY = "lazy";
my $APP_LAZY = "./$LAZY";

my $OUTPUT_DIR = "output/lazy";

sub test_data_base ($);
sub run_lazy ($$$$$$$$$);

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
		0.001,	# OK (min_rules = (1, 10))
		0.01,	# OK (min_rules = (1, 10))
		0.1,	# OK (min_rules = (1, 10))
		0.2,	# OK (min_rules = (1, 10))
		0.3,	# OK (min_rules = (1, 10))
		0.4,	# OK (min_rules = (1, 10))
		0.5,	# OK (min_rules = (1, 10))
		0.6,	# OK (min_rules = (1, 10))
		0.7,	# OK (min_rules = (1, 10))
		0.8,	# OK (min_rules = (1, 10))
		0.9,	# OK (min_rules = (1, 10))
		0.95,	# OK (min_rules = (1, 10))
		0.99,	# OK (min_rules = (1, 10))
		1	# OK (min_rules = (1, 10))
	);

	my @min_rules = (
#		1,
#		10,
		50,
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

	my $best_accuracy = 0;

	my $best_out_file = "$OUTPUT_DIR/$data_base/best.out";
	my $best_log_file = "$OUTPUT_DIR/$data_base/best.log";

	if (-e $best_out_file)
	{
		open INPUT, "<$OUTPUT_DIR/$data_base/best.out";

		$best_accuracy = <INPUT>;
		chomp $best_accuracy;
		$best_accuracy =~ s/.*accuracy: (.*)$/$1/g;

		close INPUT;
	}
	else
	{
		print "no best file for $data_base!\n";
	}

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

						my $out_file = "$OUTPUT_DIR/$data_base/s".$support."_c".$confidence."_n".$min_rules."_m".$max_size."_l".$ranking_size.".out";
						my $log_file = "$OUTPUT_DIR/$data_base/s".$support."_c".$confidence."_n".$min_rules."_m".$max_size."_l".$ranking_size.".log";

						run_lazy ($data_base, $training_file, $testing_file, $support, $confidence, $min_rules, $max_size, $ranking_size, $log_file);

						system "mkdir -p $OUTPUT_DIR/$data_base/";

						open OUTPUT, ">$out_file";
						print OUTPUT "support: $support, confidence: $confidence, min_rules: $min_rules, max_size: $max_size, ranking_size: $ranking_size, avg_patterns: $avg_patterns, avg_rules: $avg_rules, accuracy: $accuracy\n";
						close OUTPUT;

						if ($accuracy > $best_accuracy)
						{
							system "cp $out_file $best_out_file";
							system "cp $log_file $best_log_file";

							$best_accuracy = $accuracy;
						}
					}
				}
			}
		}
	}
}

sub run_lazy ($$$$$$$$$)
{
	my ($data_base, $training_file, $testing_file, $support, $confidence, $min_rules, $max_rule, $ranking_size, $log_file) = @_;

	print "$APP_LAZY -i $training_file -t $testing_file -s $support -c $confidence -n $min_rules -m $max_rule -l $ranking_size 2&>$log_file\n";
	system "$APP_LAZY -i $training_file -t $testing_file -s $support -c $confidence -n $min_rules -m $max_rule -l $ranking_size 2&>$log_file";

	open INPUT, "<$log_file";

	$accuracy = "";

	while (<INPUT>)
	{
		$accuracy = $_;
	}

	close INPUT;

	chomp $accuracy;

	$avg_patterns = $accuracy;
	$avg_rules = $accuracy;

	$accuracy	=~ s/^accuracy \[(.*)\], average patterns.*/$1/;
	$avg_patterns	=~ s/.*average patterns \[(.*)\], average rules.*/$1/;
	$avg_rules	=~ s/.*average rules \[(.*)\]$/$1/;

	print "accuracy: $accuracy, average patterns: $avg_patterns, average rules: $avg_rules\n";
}
