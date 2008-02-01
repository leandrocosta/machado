#!/usr/bin/perl -w

use strict;

my $name		= shift or die 'Error!';
my $testing_file	= shift or die 'Error!';
my $lazy_file		= shift or die 'Error!';
my $freq_file		= shift or die 'Error!';
my $orth_file		= shift or die 'Error!';

open TESTING,		"<$testing_file";
open LAZY,		"<$lazy_file";
open CLASSIFIER_C,	"<$freq_file";
open CLASSIFIER_O,	"<$orth_file";

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

my ($first, $second, $yes_yes, $yes_no, $no_yes, $no_no_y, $no_no_n);

format PRINT =
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

while (<TESTING>)
{
	my $class_testing	= $_;
	my $class_lazy		= <LAZY>;
	my $class_freq	= <CLASSIFIER_C>;
	my $class_orth	= <CLASSIFIER_O>;

	chomp $class_testing;
	chomp $class_lazy;
	chomp $class_freq;
	chomp $class_orth;

#	print "$class_testing $class_lazy $class_freq $class_orth\n";

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

print "base: $name\n";
print "tests: $tests\n";
print "---\n";
print "lazy_correct: $lazy_correct\n";
print "freq_correct: $freq_correct\n";
print "orth_correct: $orth_correct\n";
print "---\n";

$first  = 'LAZY';
$second = 'FREQ';
$yes_yes = $yeslazy_yesfreq;
$yes_no = $yeslazy_nofreq;
$no_yes = $nolazy_yesfreq;
$no_no_y = $nolazy_nofreq_coinc;
$no_no_n = $nolazy_nofreq_nocoinc;

$~ = 'PRINT';
write;

$first  = 'LAZY';
$second = 'ORTH';
$yes_yes = $yeslazy_yesorth;
$yes_no = $yeslazy_noorth;
$no_yes = $nolazy_yesorth;
$no_no_y = $nolazy_noorth_coinc;
$no_no_n = $nolazy_noorth_nocoinc;

$~ = 'PRINT';
write;

$first  = 'FREQ';
$second = 'ORTH';
$yes_yes = $yesfreq_yesorth;
$yes_no = $yesfreq_noorth;
$no_yes = $nofreq_yesorth;
$no_no_y = $nofreq_noorth_coinc;
$no_no_n = $nofreq_noorth_nocoinc;

$~ = 'PRINT';
write;

print "---\n";


=comment
print "yeslazy_yesfreq $yeslazy_yesfreq\n";
print "yeslazy_nofreq $yeslazy_nofreq\n";
print "nolazy_yesfreq $nolazy_yesfreq\n";
print "nolazy_nofreq_coinc $nolazy_nofreq_coinc\n";
print "nolazy_nofreq_nocoinc $nolazy_nofreq_nocoinc\n";
print "---\n";
print "yeslazy_yesorth $yeslazy_yesorth\n";
print "yeslazy_noorth $yeslazy_noorth\n";
print "nolazy_yesorth $nolazy_yesorth\n";
print "nolazy_noorth_coinc $nolazy_noorth_coinc\n";
print "nolazy_noorth_nocoinc $nolazy_noorth_nocoinc\n";
print "---\n";
print "yesfreq_yesorth $yesfreq_yesorth\n";
print "yesfreq_noorth $yesfreq_noorth\n";
print "nofreq_yesorth $nofreq_yesorth\n";
print "nofreq_noorth_coinc $nofreq_noorth_coinc\n";
print "nofreq_noorth_nocoinc $nofreq_noorth_nocoinc\n";
print "---\n";
=cut

exit;
