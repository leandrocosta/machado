#!/usr/bin/perl -w

use strict;

my $CLASSIFIER = "classifier";
my $APP_CLASSIFIER = "./$CLASSIFIER";

my $OUTPUT_DIR = "output/classifier_o";

sub test_data_base ($);
sub run_classifier_o ($$$$$$$$$$$$);

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
		0.001,	# OK (confidence = 0.9)
		0.01,	# OK (confidence = 0.9)
		0.1,	# OK (confidence = 0.9)
		0.2,	# OK (confidence = 0.9)
		0.3,	# OK (confidence = 0.9)
		0.4,	# OK (confidence = 0.9)
		0.5,	# OK (confidence = 0.9)
		0.6,	# OK (confidence = 0.9)
		0.7,	# OK (confidence = 0.9)
		0.8,	# OK (confidence = 0.9)
		0.9,	# OK (confidence = 0.9)
		0.95,	# OK (confidence = 0.9)
		0.99,	# OK (confidence = 0.9)
		1	# OK (confidence = 0.9)
	);

	my @confidence = (
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
# OK		0.9,
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

									my $out_file = "$OUTPUT_DIR/$data_base/s".$support."_c".$confidence."_n".$min_num_rules."_l".$max_num_rank_rules."_m".$min_rule_len."_a".$max_rule_len."_o".$omode."_e".$ometric.".out";
									my $log_file = "$OUTPUT_DIR/$data_base/s".$support."_c".$confidence."_n".$min_num_rules."_l".$max_num_rank_rules."_m".$min_rule_len."_a".$max_rule_len."_o".$omode."_e".$ometric.".log";

									run_classifier_o ($data_base, $training_file, $testing_file, $support, $confidence, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode, $ometric, $log_file);

									system "mkdir -p $OUTPUT_DIR/$data_base/";

									open OUTPUT, ">$out_file";
									print OUTPUT "support: $support, confidence: $confidence, min_num_rules: $min_num_rules, max_num_rank_rules: $max_num_rank_rules, min_rule_len: $min_rule_len, omode: $omode, ometric: $ometric, avg_patterns: $avg_patterns, avg_rules: $avg_rules, accuracy: $accuracy\n";
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
		}
	}
}

sub run_classifier_o ($$$$$$$$$$$$)
{
	my ($data_base, $training_file, $testing_file, $support, $confidence, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $omode, $ometric, $log_file) = @_;

	print "$APP_CLASSIFIER -i $training_file -t $testing_file -s $support -c $confidence -n $min_num_rules -l $max_num_rank_rules -m $min_rule_len -a $max_rule_len -r o -o $omode -e $ometric -d -1 2&>$log_file\n";
	system "$APP_CLASSIFIER -i $training_file -t $testing_file -s $support -c $confidence -n $min_num_rules -l $max_num_rank_rules -m $min_rule_len -a $max_rule_len -r o -o $omode -e $ometric -d -1 2&>$log_file";

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
