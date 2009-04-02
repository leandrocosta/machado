#!/usr/bin/perl -w

use strict;
use Common;

sub make_ttest_best_table ();
sub make_ttest_best_avg_table ();
sub get_best_results_for_application ($$);
sub get_best_avg_results_for_application ($$);
sub make_ttest_best_tables_for_data_base ($$$$);
sub make_ttest_best_avg_tables_for_data_base ($$$$);
sub make_ttest ($$$$$);

make_ttest_best_table ();
make_ttest_best_avg_table ();

exit;

###

sub make_ttest_best_table ()
{
	my ($db, $t_c_o, $t_c_or, $t_o_or);

format OUTPUT_TTEST_BEST =
		@<<<<<<<<<<<<<<<<<<<<<< & @<<<<<<<<<<<<<<<<<<<< & @<<<<<<<<<<<<<<<<<<<< & @<<<<<<<<<<<<<<<<<<<< \\
$db, $t_c_o, $t_c_or, $t_o_or
		\hline
.

	system "mkdir -p $Common::OutputDirTTests/";

	my $file_ttest_best = "$Common::OutputDirTables/table_ttest_best.tex";

	open OUTPUT_TTEST_BEST, ">$file_ttest_best";

	print OUTPUT_TTEST_BEST "\\begin{table}[htbp]
	\\centering
		\\renewcommand{\\tabcolsep}{1.8mm}
		\\begin{tabular}{|l|c|c|c|}
		\\hline
		\\textbf{Bases de Dados}	& \\textbf{LAC x OLAC}	& \\textbf{LAC x ORIGAMI}	& \\textbf{OLAC x ORIGAMI}	\\\\
		\\hline\n";

	my $data_base;

	foreach $data_base (@Common::DataBases)
	{
		print "base: $data_base\n";

		make_ttest_best_tables_for_data_base ($data_base, \$t_c_o, \$t_c_or, \$t_o_or);

		$db = $data_base;

		write OUTPUT_TTEST_BEST;
	}

	print OUTPUT_TTEST_BEST "		\\end{tabular}
	\\caption{Resultado do teste-t (melhores parâmetros para cada arquivo e abordagem)}
	\\label{tab:ttest_best}
\\end{table}";

	close OUTPUT_TTEST_BEST;
}

sub make_ttest_best_avg_table ()
{
	my ($db, $t_c_o, $t_c_or, $t_o_or);

format OUTPUT_TTEST_BEST_AVG =
		@<<<<<<<<<<<<<<<<<<<<<< & @<<<<<<<<<<<<<<<<<<<< & @<<<<<<<<<<<<<<<<<<<< & @<<<<<<<<<<<<<<<<<<<< \\
$db, $t_c_o, $t_c_or, $t_o_or
		\hline
.

	system "mkdir -p $Common::OutputDirTTests/";

	my $file_ttest_best = "$Common::OutputDirTables/table_ttest_best_avg.tex";

	open OUTPUT_TTEST_BEST_AVG, ">$file_ttest_best";

	print OUTPUT_TTEST_BEST_AVG "\\begin{table}[htbp]
	\\centering
		\\renewcommand{\\tabcolsep}{1.8mm}
		\\begin{tabular}{|l|c|c|c|}
		\\hline
		\\textbf{Bases de Dados}	& \\textbf{LAC x OLAC}	& \\textbf{LAC x ORIGAMI}	& \\textbf{OLAC x ORIGAMI}	\\\\
		\\hline\n";


	my $data_base;

	foreach $data_base (@Common::DataBases)
	{
		print "base: $data_base\n";

		make_ttest_best_avg_tables_for_data_base ($data_base, \$t_c_o, \$t_c_or, \$t_o_or);

		$db = $data_base;

		write OUTPUT_TTEST_BEST_AVG;
	}

	print OUTPUT_TTEST_BEST_AVG "		\\end{tabular}
	\\caption{Resultado do teste-t (melhores parâmetros para cada abordagem)}
	\\label{tab:ttest_best}
\\end{table}";

	close OUTPUT_TTEST_BEST_AVG;
}

sub make_ttest_best_tables_for_data_base ($$$$)
{
	my ($data_base, $t_c_o, $t_c_or, $t_o_or) = @_;

	get_best_results_for_application ('classifier_c', $data_base);
	get_best_results_for_application ('classifier_o', $data_base);
	get_best_results_for_application ('classifier_or', $data_base);

	my $ttest_dat_c		= "$Common::OutputDirTTests/ttest_${data_base}_best_classifier_c.dat";
	my $ttest_dat_o		= "$Common::OutputDirTTests/ttest_${data_base}_best_classifier_o.dat";
	my $ttest_dat_or	= "$Common::OutputDirTTests/ttest_${data_base}_best_classifier_or.dat";

	my $ttest_R_c_o 	= "$Common::OutputDirTTests/ttest_${data_base}_best_c_o.R";
	my $ttest_R_c_or 	= "$Common::OutputDirTTests/ttest_${data_base}_best_c_or.R";
	my $ttest_R_o_or 	= "$Common::OutputDirTTests/ttest_${data_base}_best_o_or.R";

	my $ttest_out_c_o	= "$Common::OutputDirTTests/ttest_${data_base}_best_c_o.out";
	my $ttest_out_c_or	= "$Common::OutputDirTTests/ttest_${data_base}_best_c_or.out";
	my $ttest_out_o_or	= "$Common::OutputDirTTests/ttest_${data_base}_best_o_or.out";

	make_ttest ($ttest_dat_c, $ttest_dat_o, $ttest_R_c_o, $ttest_out_c_o, $t_c_o);
	make_ttest ($ttest_dat_c, $ttest_dat_or, $ttest_R_c_or, $ttest_out_c_or, $t_c_or);
	make_ttest ($ttest_dat_o, $ttest_dat_or, $ttest_R_o_or, $ttest_out_o_or, $t_o_or);
}

sub make_ttest_best_avg_tables_for_data_base ($$$$)
{
	my ($data_base, $t_c_o, $t_c_or, $t_o_or) = @_;

	get_best_avg_results_for_application ('classifier_c', $data_base);
	get_best_avg_results_for_application ('classifier_o', $data_base);
	get_best_avg_results_for_application ('classifier_or', $data_base);

	my $ttest_dat_c		= "$Common::OutputDirTTests/ttest_${data_base}_best_avg_classifier_c.dat";
	my $ttest_dat_o		= "$Common::OutputDirTTests/ttest_${data_base}_best_avg_classifier_o.dat";
	my $ttest_dat_or	= "$Common::OutputDirTTests/ttest_${data_base}_best_avg_classifier_or.dat";

	my $ttest_R_c_o 	= "$Common::OutputDirTTests/ttest_${data_base}_best_avg_c_o.R";
	my $ttest_R_c_or 	= "$Common::OutputDirTTests/ttest_${data_base}_best_avg_c_or.R";
	my $ttest_R_o_or 	= "$Common::OutputDirTTests/ttest_${data_base}_best_avg_o_or.R";

	my $ttest_out_c_o	= "$Common::OutputDirTTests/ttest_${data_base}_best_avg_c_o.out";
	my $ttest_out_c_or	= "$Common::OutputDirTTests/ttest_${data_base}_best_avg_c_or.out";
	my $ttest_out_o_or	= "$Common::OutputDirTTests/ttest_${data_base}_best_avg_o_or.out";

	make_ttest ($ttest_dat_c, $ttest_dat_o, $ttest_R_c_o, $ttest_out_c_o, $t_c_o);
	make_ttest ($ttest_dat_c, $ttest_dat_or, $ttest_R_c_or, $ttest_out_c_or, $t_c_or);
	make_ttest ($ttest_dat_o, $ttest_dat_or, $ttest_R_o_or, $ttest_out_o_or, $t_o_or);
}

sub get_best_results_for_application ($$)
{
	my ($application, $data_base) = @_;

	print "application: $application\n";

	my $out_file = "$Common::OutputDirTTests/ttest_${data_base}_best_$application.dat";

	open OUTPUT, ">$out_file";

	my $fold;

	for ($fold = 0; $fold < $Common::NumFolds; $fold++)
	{
		my $log_file = Common::GetBestLogFile ($application, $data_base, $fold);

		my ($accuracy, $avg_patterns, $avg_rules, $avg_time);

		Common::GetRunResultFromLogFile ($log_file, \$accuracy, \$avg_patterns, \$avg_rules, \$avg_time);

		print OUTPUT "$accuracy\n";
	}

	close OUTPUT;
}

sub get_best_avg_results_for_application ($$)
{
	my ($application, $data_base) = @_;

	print "application: $application\n";

	my $Parms = Common::GetBestAverageAccParms ($application);

	my $out_file = "$Common::OutputDirTTests/ttest_${data_base}_best_avg_$application.dat";

	open OUTPUT, ">$out_file";

	my $fold;

	for ($fold = 0; $fold < $Common::NumFolds; $fold++)
	{
		my $log_file;

		$log_file = Common::GetClassifierCLogFile ($data_base, $Parms->{SUPPORT}, $Parms->{CONFIDENCE}, $Parms->{MIN_NUM_RULES}, $Parms->{MAX_NUM_RANK_RULES}, $Parms->{MIN_RULE_LEN}, $Parms->{MAX_RULE_LEN}, $Parms->{RULE_MEASURE}, $fold) if $application eq 'classifier_c';
		$log_file = Common::GetClassifierOLogFile ($data_base, $Parms->{SUPPORT}, $Parms->{CONFIDENCE}, $Parms->{PATTERN_SET}, $Parms->{MIN_NUM_RULES}, $Parms->{MAX_NUM_RANK_RULES}, $Parms->{MIN_RULE_LEN}, $Parms->{MAX_RULE_LEN}, $Parms->{RULE_MEASURE}, $Parms->{OMODE}, $Parms->{OMETRIC}, $Parms->{OMETHOD}, $Parms->{OORDERING}, $fold) if $application eq 'classifier_o';
		$log_file = Common::GetClassifierORLogFile ($data_base, $Parms->{SUPPORT}, $Parms->{CONFIDENCE}, $Parms->{MIN_NUM_RULES}, $Parms->{MAX_NUM_RANK_RULES}, $Parms->{RULE_MEASURE}, $Parms->{OMETRIC}, $Parms->{ALPHA}, $Parms->{BETA}, $fold) if $application eq 'classifier_or';

		my ($accuracy, $avg_patterns, $avg_rules, $avg_time);

		Common::GetRunResultFromLogFile ($log_file, \$accuracy, \$avg_patterns, \$avg_rules, \$avg_time);

		print OUTPUT "$accuracy\n";
	}

	close OUTPUT;
}

sub make_ttest ($$$$$)
{
	my ($ttest_dat_1, $ttest_dat_2, $file_R_input, $file_R_output, $diff) = @_;

	open OUTPUT, ">$file_R_input";
	print OUTPUT "c <- scan('$ttest_dat_1');\n";
	print OUTPUT "o <- scan('$ttest_dat_2');\n";
	print OUTPUT "t.test(c,o,var.equal=TRUE,var.paired=TRUE,conf.level=0.99);\n";
	close OUTPUT;

	system "R --no-restore --no-save --no-readline <$file_R_input 2>/dev/null | grep -A9 \"Two Sample t-test\" | grep -v \"data: \" >$file_R_output";
	my $t = `grep "t = " $file_R_output | sed -s 's/t = \\(.*\\), df = .*/\\1/g'`;
	chomp $t;

	$$diff = '=';
	$$diff = '>' if $t > 2.88;
	$$diff = '<' if $t < -2.88;
}
