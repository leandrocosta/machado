#!/usr/bin/perl -w

use strict;

use Common;

sub make_app_comparison_table ();
sub make_best_runs_table ();
sub make_best_avg_runs_table ();

sub get_comparisons_for_data_base ($$);

my @comparison_array;

system "mkdir -p $Common::OutputDirTables/";

make_app_comparison_table ();
make_best_runs_table ();
make_best_avg_runs_table ();


exit;

###

sub make_app_comparison_table ()
{
	print "make_app_comparison_table ()\n";

	my $db;

	my ($y_classor_y_classo, $y_classor_n_classo, $n_classor_y_classo, $n_classor_n_classo, $y_classor_y_classc, $y_classor_n_classc, $n_classor_y_classc, $n_classor_n_classc, $y_classo_y_classc, $y_classo_n_classc, $n_classo_y_classc, $n_classo_n_classc);

	my $y_classor_y_classo_avg	= 0;
	my $y_classor_n_classo_avg	= 0;
	my $n_classor_y_classo_avg	= 0;
	my $n_classor_n_classo_avg	= 0;
	my $y_classor_y_classc_avg	= 0;
	my $y_classor_n_classc_avg	= 0;
	my $n_classor_y_classc_avg	= 0;
	my $n_classor_n_classc_avg	= 0;
	my $y_classo_y_classc_avg	= 0;
	my $y_classo_n_classc_avg	= 0;
	my $n_classo_y_classc_avg	= 0;
	my $n_classo_n_classc_avg	= 0;

format OUTPUT =
@<<<<<<<<<<<<<@<<<<<<<<<<<<<<<<<@<<<<<<<<<<<<<<<<<@<<<<<<<<<<<<<<<<<@<<<<<<<<<<<<<<<<<@<<<<<<<<<<<<<<<<<<@<<<<<<<<<<<<<<<<<<@<<<<<<<<<<<<<<<<<<@<<<<<<<<<<<<<<<<<<@<<<<<<<<<<<<<<<<<<@<<<<<<<<<<<<<<<<<<@<<<<<<<<<<<<<<<<<<@<<<<<<<<<<<<<<<
$db, $y_classo_y_classc, $y_classo_n_classc, $n_classo_y_classc, $n_classo_n_classc, $y_classor_y_classc, $y_classor_n_classc, $n_classor_y_classc, $n_classor_n_classc, $y_classor_y_classo, $y_classor_n_classo, $n_classor_y_classo, $n_classor_n_classo
.

format OUTPUT_LAC_OLAC =
		@<<<<<<<<<<<<<< & @<<<<<<<<<<<< & @<<<<<<<<<<<<<<<<< & @<<<<<<<<<<<<<<<<<<<<<<< & @<<<<<<<<<<<<<<<<<<<<<<<<<<<< \\
$db, $y_classo_y_classc, $y_classo_n_classc, $n_classo_y_classc, $n_classo_n_classc
		\hline
.

format OUTPUT_LAC_ORIGAMI =
		@<<<<<<<<<<<<<< & @<<<<<<<<<<<< & @<<<<<<<<<<<<<<<<< & @<<<<<<<<<<<<<<<<<<<<<<< & @<<<<<<<<<<<<<<<<<<<<<<<<<<<< \\
$db, $y_classor_y_classc, $y_classor_n_classc, $n_classor_y_classc, $n_classor_n_classc
		\hline
.

format OUTPUT_OLAC_ORIGAMI =
		@<<<<<<<<<<<<<< & @<<<<<<<<<<<< & @<<<<<<<<<<<<<<<<< & @<<<<<<<<<<<<<<<<<<<<<<< & @<<<<<<<<<<<<<<<<<<<<<<<<<<<< \\
$db, $y_classor_y_classo, $n_classor_y_classo, $y_classor_n_classo, $n_classor_n_classo
		\hline
.

	my $out_file			= "$Common::OutputDirTables/comparison.out";
	my $out_lac_olac_file		= "$Common::OutputDirTables/table_comparison_lac_olac.tex";
	my $out_lac_origami_file	= "$Common::OutputDirTables/table_comparison_lac_origami.tex";
	my $out_olac_origami_file	= "$Common::OutputDirTables/table_comparison_olac_origami.tex";

	open OUTPUT, ">$out_file";
	print OUTPUT "data set      y_classo_y_classc y_classo_n_classc n_classo_y_classc n_classo_n_classc y_classor_y_classc y_classor_n_classc n_classor_y_classc n_classor_n_classc y_classor_y_classo y_classor_n_classo n_classor_y_classo n_classor_n_classo\n";

	open OUTPUT_LAC_OLAC, ">$out_lac_olac_file";
#		\\begin{tabular*}{.95\\textwidth}{|l|c|c|c|c|}
#		\\textbf{Data Sets}	& \\textbf{OLAC \\& LAC}	& \\textbf{OLAC \\& \$\\neg\$ LAC} & \\textbf{\$\\neg\$ OLAC \\& LAC}	& \\textbf{\$\\neg\$ OLAC \\& \$\\neg\$ LAC}	\\\\
	print OUTPUT_LAC_OLAC "\\begin{table}[htbp]
	\\centering
		\\begin{tabular}{|l|c|c|c|c|}
		\\hline
				& \\textbf{OLAC}		& \\textbf{OLAC}			& \\textbf{\$\\neg\$ OLAC}	& \\textbf{\$\\neg\$ OLAC}	\\\\
		\\textbf{Data Sets}	& \\textbf{\\&}		& \\textbf{\\&}			& \\textbf{\\&}			& \\textbf{\\&}			\\\\
				& \\textbf{LAC}		& \\textbf{\$\\neg\$ LAC}		& \\textbf{LAC}			& \\textbf{\$\\neg\$ LAC}		\\\\
		\\hline\n";


	open OUTPUT_LAC_ORIGAMI, ">$out_lac_origami_file";
	print OUTPUT_LAC_ORIGAMI "\\begin{table}[htbp]
	\\centering
		\\begin{tabular}{|l|c|c|c|c|}
		\\hline
				& \\textbf{ORIGAMI}	& \\textbf{ORIGAMI}		& \\textbf{\$\\neg\$ ORIGAMI}	& \\textbf{\$\\neg\$ ORIGAMI}	\\\\
		\\textbf{Data Sets}	& \\textbf{\\&}		& \\textbf{\\&}			& \\textbf{\\&}			& \\textbf{\\&}			\\\\
				&  \\textbf{LAC}		& \\textbf{\$\\neg\$ LAC}		& \\textbf{LAC}			& \\textbf{\$\\neg\$ LAC}		\\\\
		\\hline\n";

	open OUTPUT_OLAC_ORIGAMI, ">$out_olac_origami_file";
#		Data Sets	& \\multicolumn{1}{p{2.5cm}}{OLAC \\& ORIGAMI}	& OLAC \\& \$\\neg\$ ORIGAMI & \$\\neg\$ OLAC \\& ORIGAMI	& \$\\neg\$ OLAC \\& \$\\neg\$ ORIGAMI	\\\\
	print OUTPUT_OLAC_ORIGAMI "\\begin{table}[htbp]
	\\centering
		\\begin{tabular}{|l|c|c|c|c|}
		\\hline
				& \\textbf{OLAC}		& \\textbf{OLAC}			& \\textbf{\$\\neg\$ OLAC}	& \\textbf{\$\\neg\$ OLAC}	\\\\
		\\textbf{Data Sets}	& \\textbf{\\&}		& \\textbf{\\&}			& \\textbf{\\&}			& \\textbf{\\&}			\\\\
				& \\textbf{ORIGAMI}	& \\textbf{\$\\neg\$ ORIGAMI}	& \\textbf{ORIGAMI}		& \\textbf{\$\\neg\$ ORIGAMI}	\\\\
		\\hline\n";

	my $data_base;

	foreach $data_base (@Common::DataBases)
	{
		print "base: $data_base\n";

		$db = $data_base;

		my @comparison_array;

		get_comparisons_for_data_base ($data_base, \@comparison_array);

		my $i;

		$y_classor_y_classo = 0;
		$y_classor_n_classo = 0;
		$n_classor_y_classo = 0;
		$n_classor_n_classo = 0;
		$y_classor_y_classc = 0;
		$y_classor_n_classc = 0;
		$n_classor_y_classc = 0;
		$n_classor_n_classc = 0;
		$y_classo_y_classc = 0;
		$y_classo_n_classc = 0;
		$n_classo_y_classc = 0;
		$n_classo_n_classc = 0;

		for ($i = 0; $i < scalar @comparison_array; $i++)
		{
			my $correct_classor	= 0;
			my $correct_classc	= 0;
			my $correct_classo	= 0;

			$correct_classor	= 1 if $comparison_array[$i]->{CLASS_CORRECT} eq $comparison_array[$i]->{CLASS_CLASSIFIER_OR};
			$correct_classc		= 1 if $comparison_array[$i]->{CLASS_CORRECT} eq $comparison_array[$i]->{CLASS_CLASSIFIER_C};
			$correct_classo		= 1 if $comparison_array[$i]->{CLASS_CORRECT} eq $comparison_array[$i]->{CLASS_CLASSIFIER_O};

			$y_classor_y_classo++	if $correct_classor && $correct_classo;
			$y_classor_n_classo++	if $correct_classor && ! $correct_classo;
			$n_classor_y_classo++	if ! $correct_classor && $correct_classo;
			$n_classor_n_classo++	if ! $correct_classor && ! $correct_classo;

			$y_classor_y_classc++	if $correct_classor && $correct_classc;
			$y_classor_n_classc++	if $correct_classor && ! $correct_classc;
			$n_classor_y_classc++	if ! $correct_classor && $correct_classc;
			$n_classor_n_classc++	if ! $correct_classor && ! $correct_classc;

			$y_classo_y_classc++	if $correct_classo && $correct_classc;
			$y_classo_n_classc++	if $correct_classo && ! $correct_classc;
			$n_classo_y_classc++	if ! $correct_classo && $correct_classc;
			$n_classo_n_classc++	if ! $correct_classo && ! $correct_classc;
		}

		write OUTPUT;
		write OUTPUT_LAC_OLAC;
		write OUTPUT_LAC_ORIGAMI;
		write OUTPUT_OLAC_ORIGAMI;

		$y_classor_y_classo_avg	+= $y_classor_y_classo;
		$y_classor_n_classo_avg	+= $y_classor_n_classo;
		$n_classor_y_classo_avg	+= $n_classor_y_classo;
		$n_classor_n_classo_avg	+= $n_classor_n_classo;
		$y_classor_y_classc_avg	+= $y_classor_y_classc;
		$y_classor_n_classc_avg	+= $y_classor_n_classc;
		$n_classor_y_classc_avg	+= $n_classor_y_classc;
		$n_classor_n_classc_avg	+= $n_classor_n_classc;
		$y_classo_y_classc_avg	+= $y_classo_y_classc;
		$y_classo_n_classc_avg	+= $y_classo_n_classc;
		$n_classo_y_classc_avg	+= $n_classo_y_classc;
		$n_classo_n_classc_avg	+= $n_classo_n_classc;
	}


	$y_classor_y_classo	= sprintf ("%0.2f", $y_classor_y_classo_avg / scalar @Common::DataBases);
	$y_classor_n_classo	= sprintf ("%0.2f", $y_classor_n_classo_avg / scalar @Common::DataBases);
	$n_classor_y_classo	= sprintf ("%0.2f", $n_classor_y_classo_avg / scalar @Common::DataBases);
	$n_classor_n_classo	= sprintf ("%0.2f", $n_classor_n_classo_avg / scalar @Common::DataBases);
	$y_classor_y_classc	= sprintf ("%0.2f", $y_classor_y_classc_avg / scalar @Common::DataBases);
	$y_classor_n_classc	= sprintf ("%0.2f", $y_classor_n_classc_avg / scalar @Common::DataBases);
	$n_classor_y_classc	= sprintf ("%0.2f", $n_classor_y_classc_avg / scalar @Common::DataBases);
	$n_classor_n_classc	= sprintf ("%0.2f", $n_classor_n_classc_avg / scalar @Common::DataBases);
	$y_classo_y_classc	= sprintf ("%0.2f", $y_classo_y_classc_avg / scalar @Common::DataBases);
	$y_classo_n_classc	= sprintf ("%0.2f", $y_classo_n_classc_avg / scalar @Common::DataBases);
	$n_classo_y_classc	= sprintf ("%0.2f", $n_classo_y_classc_avg / scalar @Common::DataBases);
	$n_classo_n_classc	= sprintf ("%0.2f", $n_classo_n_classc_avg / scalar @Common::DataBases);

	$db = 'average';

	write OUTPUT;
	write OUTPUT_LAC_OLAC;
	write OUTPUT_LAC_ORIGAMI;
	write OUTPUT_OLAC_ORIGAMI;

#	print OUTPUT_LAC_OLAC "		\\end{tabular*}
	print OUTPUT_LAC_OLAC "		\\end{tabular}
	\\caption{Comparacao entre LAC e OLAC (numero de acertos)}
	\\label{tab:comparison_lac_olac}
\\end{table}";
	print OUTPUT_LAC_ORIGAMI "		\\end{tabular}
	\\caption{Comparacao entre LAC e ORIGAMI (numero de acertos)}
	\\label{tab:comparison_lac_origami}
\\end{table}";
	print OUTPUT_OLAC_ORIGAMI "		\\end{tabular}
	\\caption{Comparacao entre OLAC e ORIGAMI (numero de acertos)}
	\\label{tab:comparison_olac_origami}
\\end{table}";

	close OUTPUT;
	close OUTPUT_LAC_OLAC;
	close OUTPUT_LAC_ORIGAMI;
	close OUTPUT_OLAC_ORIGAMI;
}

sub make_best_runs_table ()
{
	print "make_best_runs_table ()\n";

#	make_best_runs_table_for_lazy ();
	make_best_runs_table_for_classifier_c ();
	make_best_runs_table_for_classifier_o ();
	make_best_runs_table_for_classifier_or ();
}

=comment
sub make_best_runs_table_for_lazy ()
{
	my $db;

	my ($support, $confidence, $min_rules, $max_size, $ranking_size, $avg_patterns, $avg_rules, $accuracy);

format OUTPUT_BEST_RUNS_LAZY =
@<<<<<<<<<<<<<@<<<<<<<@<<<<<<<<<<@<<<<<<<<<@<<<<<<<<@<<<<<<<<<<<<@<<<<<<<<<<<<<@<<<<<<<<<<<<<@<<<<<<<<<<<<<
$db,                $support,     $confidence,  $min_rules,   $max_size,    $ranking_size, $avg_patterns, $avg_rules, $accuracy
.

	my $out_file = "$Common::OutputDirTables/table_best_runs_lazy.out";

	open OUTPUT_BEST_RUNS_LAZY, ">$out_file";
	print OUTPUT_BEST_RUNS_LAZY "data set      support confidence min_rules max_size ranking_size avg_patterns  avg_rules     accuracy\n";

	my $data_base;

	foreach $data_base (@Common::DataBases)
	{
		print "base: $data_base\n";

		$db = $data_base;

		my $file_best_run = Common::GetBestOutputFile ('lazy', $data_base);

		open INPUT, "<$file_best_run";

		$support = <INPUT>;

		chomp $support;

		$confidence	= $support;
		$min_rules	= $support;
		$max_size	= $support;
		$ranking_size	= $support;
		$avg_patterns	= $support;
		$avg_rules	= $support;
		$accuracy	= $support;

		$support	=~ s/.*support \[([^\]]*)\].*/$1/;
		$confidence	=~ s/.*confidence \[([^\]]*)\].*/$1/;
		$min_rules	=~ s/.*min_rules \[([^\]]*)\].*/$1/;
		$max_size	=~ s/.*max_size \[([^\]]*)\].*/$1/;
		$ranking_size	=~ s/.*ranking_size \[([^\]]*)\].*/$1/;
		$avg_patterns	=~ s/.*avg_patterns \[([^\]]*)\].*/$1/;
		$avg_rules	=~ s/.*avg_rules \[([^\]]*)\].*/$1/;
		$accuracy	=~ s/.*accuracy \[([^\]]*)\].*/$1/;

		write OUTPUT_BEST_RUNS_LAZY;
	}

	close OUTPUT_BEST_RUNS_LAZY;
}
=cut

sub make_best_runs_table_for_classifier_c ()
{
	my $db;

	my ($support, $confidence, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $rule_measure, $avg_time, $avg_patterns, $avg_rules, $accuracy);

format OUTPUT_BEST_RUNS_CLASSC =
		@<<<<<<<<<<<<< & @<<<<<<< & @<<<<<<<<<< & @<<<<<<<<<<<<< & @<<<<<<<<<<<<<<<<<< & @<<<<<<<<<<<< & @<<<<< & @<<<<<<<<<<<< & @<<<<<<<<<<<<< & @<<<<<<<<<<<<< & @<<<<<<<<<<<<< & @<<<<<<<<<<<<< \\
$db,                $support,     $confidence,  $min_num_rules,$max_num_rank_rules,$min_rule_len,$max_rule_len,$rule_measure,$avg_patterns,$avg_rules,$avg_time,$accuracy
		\hline
.

	my $out_file = "$Common::OutputDirTables/table_best_runs_lac.tex";

	open OUTPUT_BEST_RUNS_CLASSC, ">$out_file";
#	print OUTPUT_BEST_RUNS_CLASSC "data set      support confidence min_num_rules max_num_rank_rules min_rule_len max_rule_len avg_time      avg_patterns  avg_rules     accuracy      \n";
	print OUTPUT_BEST_RUNS_CLASSC "\\begin{table}[htbp]
	\\centering
		\\begin{tabular}{|l|c|c|c|c|c|c|c||c|c|c|c|}
		\\hline
		\\textbf{dataset}	& \\textbf{s}	& \\textbf{c}	& \\textbf{n}	& \\textbf{l}	& \\textbf{m}	& \\textbf{x}	& \\textbf{u}	& \\textbf{pat.}	& \\textbf{rul.}	& \\textbf{tim.}	& \\textbf{acc.}	\\\\
		\\hline\n";

	my $data_base;

	foreach $data_base (@Common::DataBases)
	{
		print "base: $data_base\n";

		$db = $data_base;

		my $file_best_run = Common::GetBestOutputFile ('classifier_c', $data_base);

		open INPUT, "<$file_best_run";

		$support = <INPUT>;

		chomp $support;

		$confidence		= $support;
		$min_num_rules		= $support;
		$max_num_rank_rules	= $support;
		$min_rule_len		= $support;
		$max_rule_len		= $support;
		$rule_measure		= $support;
		$avg_time		= $support;
		$avg_patterns		= $support;
		$avg_rules		= $support;
		$accuracy		= $support;

		$support		=~ s/.*support \[([^\]]*)\].*/$1/;
		$confidence		=~ s/.*confidence \[([^\]]*)\].*/$1/;
		$min_num_rules		=~ s/.*min_num_rules \[([^\]]*)\].*/$1/;
		$max_num_rank_rules	=~ s/.*max_num_rank_rules \[([^\]]*)\].*/$1/;
		$min_rule_len		=~ s/.*min_rule_len \[([^\]]*)\].*/$1/;
		$max_rule_len		=~ s/.*max_rule_len \[([^\]]*)\].*/$1/;
		$rule_measure		=~ s/.*rule_measure \[([^\]]*)\].*/$1/;
		$avg_time		=~ s/.*avg_time \[([^\]]*)\].*/$1/;
		$avg_patterns		=~ s/.*avg_patterns \[([^\]]*)\].*/$1/;
		$avg_rules		=~ s/.*avg_rules \[([^\]]*)\].*/$1/;
		$accuracy		=~ s/.*accuracy \[([^\]]*)\],*/$1/;

		$avg_time	= sprintf ("%0.2f", $avg_time);
		$avg_patterns	= sprintf ("%0.2f", $avg_patterns);
		$avg_rules	= sprintf ("%0.2f", $avg_rules);
		$accuracy	= sprintf ("%0.2f", $accuracy);

		write OUTPUT_BEST_RUNS_CLASSC;
	}

	print OUTPUT_BEST_RUNS_CLASSC "		\\end{tabular}
	\\caption{Melhores Parametros e Resultados para cada Base de Dados (para LAC)}
	\\label{tab:best_runs_for_each_db_lac}
\\end{table}";

	close OUTPUT_BEST_RUNS_CLASSC;
}

sub make_best_runs_table_for_classifier_o ()
{
	my $db;

	my ($support, $confidence, $min_num_rules, $max_num_rank_rules, $min_rule_len, $max_rule_len, $rule_measure, $ometric, $omethod, $oordering, $avg_time, $avg_patterns, $avg_rules, $accuracy);

format OUTPUT_BEST_RUNS_CLASSO =
		@<<<<<<<<<<<<< & @<<<<<<< & @<<<<<<<<<< & @<<<<<<<<<<<<< & @<<<<<<<<<<<<<<<<<< & @<<<<<<<<<<<< & @<<<<<<< & @<<<<<<<<<<<< & @<<<<< & @<<<<<<< & @<<<<<<< & @<<<<<<<<<<<<< & @<<<<<<<<<<<<< & @<<<<<<<<<<<<< & @<<<<<<<<<<<<< \\
$db,          $support,$confidence,$min_num_rules,$max_num_rank_rules,$min_rule_len,$max_rule_len,$rule_measure,$ometric,$omethod,$oordering,$avg_patterns,$avg_rules,$avg_time,$accuracy
		\hline
.

	my $out_file = "$Common::OutputDirTables/table_best_runs_olac.tex";

	open OUTPUT_BEST_RUNS_CLASSO, ">$out_file";
#	print OUTPUT_BEST_RUNS_CLASSO "data set      support confidence min_num_rules max_num_rank_rules min_rule_len max_rule_len omode ometric avg_time      avg_patterns  avg_rules     accuracy      \n";
	print OUTPUT_BEST_RUNS_CLASSO "\\begin{table}[htbp]
	\\centering
		\\begin{tabular}{|l|c|c|c|c|c|c|c|c|c|c||c|c|c|c|}
		\\hline
		\\textbf{dataset}	& \\textbf{s}	& \\textbf{c}	& \\textbf{n}	& \\textbf{l}	& \\textbf{m}	& \\textbf{x}	& \\textbf{u}	& \\textbf{e} & \\textbf{w} & \\textbf{g} & \\textbf{pat.}	& \\textbf{rul.}	& \\textbf{tim.}	& \\textbf{acc.}	\\\\
		\\hline\n";

	my $data_base;

	foreach $data_base (@Common::DataBases)
	{
		print "base: $data_base\n";

		$db = $data_base;

		my $file_best_run = Common::GetBestOutputFile ('classifier_o', $data_base);

		open INPUT, "<$file_best_run";

		$support = <INPUT>;

		chomp $support;

		$confidence		= $support;
		$min_num_rules		= $support;
		$max_num_rank_rules	= $support;
		$min_rule_len		= $support;
		$max_rule_len		= $support;
		$rule_measure		= $support;
		$ometric		= $support;
		$omethod		= $support;
		$oordering		= $support;
		$avg_time		= $support;
		$avg_patterns		= $support;
		$avg_rules		= $support;
		$accuracy		= $support;

		$support		=~ s/.*support \[([^\]]*)\].*/$1/;
		$confidence		=~ s/.*confidence \[([^\]]*)\].*/$1/;
		$min_num_rules		=~ s/.*min_num_rules \[([^\]]*)\].*/$1/;
		$max_num_rank_rules	=~ s/.*max_num_rank_rules \[([^\]]*)\].*/$1/;
		$min_rule_len		=~ s/.*min_rule_len \[([^\]]*)\].*/$1/;
		$max_rule_len		=~ s/.*max_rule_len \[([^\]]*)\].*/$1/;
		$rule_measure		=~ s/.*rule_measure \[([^\]\}]*)(\]|\}).*/$1/;
		$ometric		=~ s/.*ometric \[([^\]]*)\].*/$1/;
		$omethod		=~ s/.*omethod \[([^\]]*)\].*/$1/;
		$oordering		=~ s/.*oordering \[([^\]]*)\].*/$1/;
		$avg_time		=~ s/.*avg_time \[([^\]]*)\].*/$1/;
		$avg_patterns		=~ s/.*avg_patterns \[([^\]]*)\].*/$1/;
		$avg_rules		=~ s/.*avg_rules \[([^\]]*)\].*/$1/;
		$accuracy		=~ s/.*accuracy \[([^\]]*)\].*/$1/;

		$avg_time	= sprintf ("%0.2f", $avg_time);
		$avg_patterns	= sprintf ("%0.2f", $avg_patterns);
		$avg_rules	= sprintf ("%0.2f", $avg_rules);
		$accuracy	= sprintf ("%0.2f", $accuracy);

		write OUTPUT_BEST_RUNS_CLASSO;
	}

	print OUTPUT_BEST_RUNS_CLASSO "		\\end{tabular}
	\\caption{Melhores Parametros e Resultados para cada Base de Dados (para OLAC)}
	\\label{tab:best_runs_for_each_db_olac}
\\end{table}";

	close OUTPUT_BEST_RUNS_CLASSO;
}

sub make_best_runs_table_for_classifier_or ()
{
	my $db;

	my ($support, $confidence, $min_num_rules, $max_num_rank_rules, $rule_measure, $ometric, $alpha, $beta, $avg_time, $avg_patterns, $avg_rules, $accuracy);

format OUTPUT_BEST_RUNS_CLASSOR =
		@<<<<<<<<<<<<< & @<<<<<<< & @<<<<<<<<<< & @<<<<<<<<<<<<< & @<<<<<<< & @<<<<<<<<<<<<<<<<<< & @<<<<<<< & @<<<<< & @<<<< & @<<<<<<<<<<<<< & @<<<<<<<<<<<<< & @<<<<<<<<<<<<< & @<<<<<<<<<<<<< \\
$db,          $support,$confidence,$min_num_rules,$max_num_rank_rules,$rule_measure,$ometric,$alpha,$beta,$avg_patterns,$avg_rules,$avg_time,$accuracy
		\hline
.

	my $out_file = "$Common::OutputDirTables/table_best_runs_origami.tex";

	open OUTPUT_BEST_RUNS_CLASSOR, ">$out_file";
#	print OUTPUT_BEST_RUNS_CLASSOR "data set      support confidence min_num_rules max_num_rank_rules ometric alpha beta avg_time      avg_patterns  avg_rules     accuracy      \n";
	print OUTPUT_BEST_RUNS_CLASSOR "\\begin{table}[htbp]
	\\centering
		\\begin{tabular}{|l|c|c|c|c|c|c|c|c||c|c|c|c|}
		\\hline
		\\textbf{dataset}	& \\textbf{s}	& \\textbf{c}	& \\textbf{n}	& \\textbf{l}	& \\textbf{u}	& \\textbf{e} & \\textbf{a} & \\textbf{b} & \\textbf{pat.}	& \\textbf{rul.}	& \\textbf{tim.}	& \\textbf{acc.}	\\\\
		\\hline\n";

	my $data_base;

	foreach $data_base (@Common::DataBases)
	{
		print "base: $data_base\n";

		$db = $data_base;

		my $file_best_run = Common::GetBestOutputFile ('classifier_or', $data_base);

		open INPUT, "<$file_best_run";

		$support = <INPUT>;

		chomp $support;

		$confidence		= $support;
		$min_num_rules		= $support;
		$max_num_rank_rules	= $support;
		$rule_measure	= $support;
		$ometric		= $support;
		$alpha			= $support;
		$beta			= $support;
		$avg_time		= $support;
		$avg_patterns		= $support;
		$avg_rules		= $support;
		$accuracy		= $support;

		$support		=~ s/.*support \[([^\]]*)\].*/$1/;
		$confidence		=~ s/.*confidence \[([^\]]*)\].*/$1/;
		$min_num_rules		=~ s/.*min_num_rules \[([^\]]*)\].*/$1/;
		$max_num_rank_rules	=~ s/.*max_num_rank_rules \[([^\]]*)\].*/$1/;
		$rule_measure		=~ s/.*rule_measure \[([^\]]*)\].*/$1/;
		$ometric		=~ s/.*ometric \[([^\]]*)\].*/$1/;
		$alpha			=~ s/.*alpha \[([^\]]*)\].*/$1/;
		$beta			=~ s/.*beta \[([^\]]*)\].*/$1/;
		$avg_time		=~ s/.*avg_time \[([^\]]*)\].*/$1/;
		$avg_patterns		=~ s/.*avg_patterns \[([^\]]*)\].*/$1/;
		$avg_rules		=~ s/.*avg_rules \[([^\]]*)\].*/$1/;
		$accuracy		=~ s/.*accuracy \[([^\]]*)\].*/$1/;

		$avg_time	= sprintf ("%0.2f", $avg_time);
		$avg_patterns	= sprintf ("%0.2f", $avg_patterns);
		$avg_rules	= sprintf ("%0.2f", $avg_rules);
		$accuracy	= sprintf ("%0.2f", $accuracy);

		write OUTPUT_BEST_RUNS_CLASSOR;
	}

	print OUTPUT_BEST_RUNS_CLASSOR "		\\end{tabular}
	\\caption{Melhores Parametros e Resultados para cada Base de Dados (para ORIGAMI)}
	\\label{tab:best_runs_for_each_db_origami}
\\end{table}";

	close OUTPUT_BEST_RUNS_CLASSOR;
}

sub get_comparisons_for_data_base ($$)
{
	my ($data_base, $comparison_array) = @_;

	my $ParmsClassifierC = Common::GetBestAverageAccParms ('classifier_c');
	my $ParmsClassifierO = Common::GetBestAverageAccParms ('classifier_o');
	my $ParmsClassifierOR = Common::GetBestAverageAccParms ('classifier_or');

	my $LAC_S	= $ParmsClassifierC->{SUPPORT};
	my $LAC_C	= $ParmsClassifierC->{CONFIDENCE};
	my $LAC_N	= $ParmsClassifierC->{MIN_NUM_RULES};
	my $LAC_L	= $ParmsClassifierC->{MAX_NUM_RANK_RULES};
	my $LAC_M	= $ParmsClassifierC->{MIN_RULE_LEN};
	my $LAC_X	= $ParmsClassifierC->{MAX_RULE_LEN};
	my $LAC_U	= $ParmsClassifierC->{RULE_MEASURE};

	my $OLAC_S	= $ParmsClassifierO->{SUPPORT};
	my $OLAC_C	= $ParmsClassifierO->{CONFIDENCE};
	my $OLAC_N	= $ParmsClassifierO->{MIN_NUM_RULES};
	my $OLAC_L	= $ParmsClassifierO->{MAX_NUM_RANK_RULES};
	my $OLAC_M	= $ParmsClassifierO->{MIN_RULE_LEN};
	my $OLAC_X	= $ParmsClassifierO->{MAX_RULE_LEN};
	my $OLAC_U	= $ParmsClassifierO->{RULE_MEASURE};
	my $OLAC_E	= $ParmsClassifierO->{OMETRIC};
	my $OLAC_W	= $ParmsClassifierO->{OMETHOD};
	my $OLAC_G	= $ParmsClassifierO->{OORDERING};

	my $ORIGAMI_S	= $ParmsClassifierOR->{SUPPORT};
	my $ORIGAMI_C	= $ParmsClassifierOR->{CONFIDENCE};
	my $ORIGAMI_N	= $ParmsClassifierOR->{MIN_NUM_RULES};
	my $ORIGAMI_L	= $ParmsClassifierOR->{MAX_NUM_RANK_RULES};
	my $ORIGAMI_U	= $ParmsClassifierOR->{RULE_MEASURE};
	my $ORIGAMI_E	= $ParmsClassifierOR->{OMETRIC};
	my $ORIGAMI_A	= $ParmsClassifierOR->{ALPHA};
	my $ORIGAMI_B	= $ParmsClassifierOR->{BETA};

	my $fold;

	for ($fold = 0; $fold < $Common::NumFolds; $fold++)
	{
#		my $log_file_classifier_or	= Common::GetBestLogFile ('classifier_or', $data_base, $fold);
		my $log_file_classifier_or	= Common::GetClassifierORLogFile ($data_base, $ORIGAMI_S, $ORIGAMI_C, $ORIGAMI_N, $ORIGAMI_L, $ORIGAMI_U, $ORIGAMI_E, $ORIGAMI_A, $ORIGAMI_B, $fold);
#		my $log_file_classifier_c	= Common::GetBestLogFile ('classifier_c', $data_base, $fold);
		my $log_file_classifier_c	= Common::GetClassifierCLogFile ($data_base, $LAC_S, $LAC_C, $LAC_N, $LAC_L, $LAC_M, $LAC_X, $LAC_U, $fold);
#		my $log_file_classifier_o	= Common::GetBestLogFile ('classifier_o', $data_base, $fold);
		my $log_file_classifier_o	= Common::GetClassifierOLogFile ($data_base, $OLAC_S, $OLAC_C, 'f', $OLAC_N, $OLAC_L, $OLAC_M, $OLAC_X, $OLAC_U, 'h', $OLAC_E, $OLAC_W, $OLAC_G, $fold);

#		print "classifier_or: $log_file_classifier_or\n";
#		print "classifier_c: $log_file_classifier_c\n";
#		print "classifier_o: $log_file_classifier_o\n";

		open LOG_FILE_CLASSIFIER_OR, "<$log_file_classifier_or";
		open LOG_FILE_CLASSIFIER_C, "<$log_file_classifier_c";
		open LOG_FILE_CLASSIFIER_O, "<$log_file_classifier_o";

		while (<LOG_FILE_CLASSIFIER_OR>)
		{
			my $test_classifier_c	= <LOG_FILE_CLASSIFIER_C>;
			my $test_classifier_o	= <LOG_FILE_CLASSIFIER_O>;

			if ($_ =~ m/^transaction/)
			{
				my $test_classifier_or	= $_;

#				my $class_correct	= $test_classifier_or;
				my $class_correct	= $test_classifier_o;
#				my $class_correct	= $test_classifier_c;
				my $class_classifier_or	= $test_classifier_or;
				my $class_classifier_c	= $test_classifier_c;
				my $class_classifier_o	= $test_classifier_o;

				$class_correct		=~ s/.*class \[([^\]]*)\].*/$1/;
				$class_classifier_or	=~ s/.*guess \[([^\]]*)\].*/$1/;
				$class_classifier_c	=~ s/.*guess \[([^\]]*)\].*/$1/;
				$class_classifier_o	=~ s/.*guess \[([^\]]*)\].*/$1/;

				my $comparison = {
					CLASS_CORRECT		=> $class_correct,
					CLASS_CLASSIFIER_OR	=> $class_classifier_or,
					CLASS_CLASSIFIER_C	=> $class_classifier_c,
					CLASS_CLASSIFIER_O	=> $class_classifier_o
				};

				push @{$comparison_array}, $comparison;
			}
		}

		close LOG_FILE_CLASSIFIER_O;
		close LOG_FILE_CLASSIFIER_C;
		close LOG_FILE_CLASSIFIER_OR;
	}
}

sub make_best_avg_runs_table ()
{
	my $ParmsClassifierC = Common::GetBestAverageAccParms ('classifier_c');
	my $ParmsClassifierO = Common::GetBestAverageAccParms ('classifier_o');
	my $ParmsClassifierOR = Common::GetBestAverageAccParms ('classifier_or');

	my $LAC_S	= $ParmsClassifierC->{SUPPORT};
	my $LAC_C	= $ParmsClassifierC->{CONFIDENCE};
	my $LAC_N	= $ParmsClassifierC->{MIN_NUM_RULES};
	my $LAC_L	= $ParmsClassifierC->{MAX_NUM_RANK_RULES};
	my $LAC_M	= $ParmsClassifierC->{MIN_RULE_LEN};
	my $LAC_X	= $ParmsClassifierC->{MAX_RULE_LEN};
	my $LAC_U	= $ParmsClassifierC->{RULE_MEASURE};

	my $OLAC_S	= $ParmsClassifierO->{SUPPORT};
	my $OLAC_C	= $ParmsClassifierO->{CONFIDENCE};
	my $OLAC_N	= $ParmsClassifierO->{MIN_NUM_RULES};
	my $OLAC_L	= $ParmsClassifierO->{MAX_NUM_RANK_RULES};
	my $OLAC_M	= $ParmsClassifierO->{MIN_RULE_LEN};
	my $OLAC_X	= $ParmsClassifierO->{MAX_RULE_LEN};
	my $OLAC_U	= $ParmsClassifierO->{RULE_MEASURE};
	my $OLAC_E	= $ParmsClassifierO->{OMETRIC};
	my $OLAC_W	= $ParmsClassifierO->{OMETHOD};
	my $OLAC_G	= $ParmsClassifierO->{OORDERING};

	my $ORIGAMI_S	= $ParmsClassifierOR->{SUPPORT};
	my $ORIGAMI_C	= $ParmsClassifierOR->{CONFIDENCE};
	my $ORIGAMI_N	= $ParmsClassifierOR->{MIN_NUM_RULES};
	my $ORIGAMI_L	= $ParmsClassifierOR->{MAX_NUM_RANK_RULES};
	my $ORIGAMI_U	= $ParmsClassifierOR->{RULE_MEASURE};
	my $ORIGAMI_E	= $ParmsClassifierOR->{OMETRIC};
	my $ORIGAMI_A	= $ParmsClassifierOR->{ALPHA};
	my $ORIGAMI_B	= $ParmsClassifierOR->{BETA};

	open OUTPUT, ">output/tables/table_best_parms_for_avg_db.tex";

	print OUTPUT "\\begin{table}[htbp]
	\\centering
		\\begin{tabular}{|l|c|c|c|}
		\\hline
					& \\textbf{LAC}	& \\textbf{OLAC}	& \\textbf{ORIGAMI}	\\\\
		\\hline\n";

	print OUTPUT "		support			& $LAC_S	& $OLAC_S	& $ORIGAMI_S		\\\\\n";
	print OUTPUT "		\\hline\n";
	print OUTPUT "		confidence		& $LAC_C		& $OLAC_C	& $ORIGAMI_C		\\\\\n";
	print OUTPUT "		\\hline\n";
	print OUTPUT "		min-num-rules		& $LAC_N		& $OLAC_N		& $ORIGAMI_N			\\\\\n";
	print OUTPUT "		\\hline\n";
	print OUTPUT "		max-num-rank-rules	& $LAC_L		& $OLAC_L		& $ORIGAMI_L			\\\\\n";
	print OUTPUT "		\\hline\n";
	print OUTPUT "		min-rule-len		& $LAC_M		& $OLAC_M		& -			\\\\\n";
	print OUTPUT "		\\hline\n";
	print OUTPUT "		max-rule-len		& $LAC_X		& $OLAC_X		& -			\\\\\n";
	print OUTPUT "		\\hline\n";
	print OUTPUT "		rule-measure		& $LAC_U		& $OLAC_U		& $ORIGAMI_U			\\\\\n";
	print OUTPUT "		\\hline\n";
	print OUTPUT "		orth-metric		& -		& $OLAC_E		& $ORIGAMI_E			\\\\\n";
	print OUTPUT "		\\hline\n";
	print OUTPUT "		orth-method		& -		& $OLAC_W		& -			\\\\\n";
	print OUTPUT "		\\hline\n";
	print OUTPUT "		orth-pat-ordering	& -		& $OLAC_G		& -			\\\\\n";
	print OUTPUT "		\\hline\n";
	print OUTPUT "		origami-alpha		& -		& -		& $ORIGAMI_A			\\\\\n";
	print OUTPUT "		\\hline\n";
	print OUTPUT "		origami-beta		& -		& -		& $ORIGAMI_B			\\\\\n";
	print OUTPUT "		\\hline\n";

	print OUTPUT "		\\end{tabular}
	\\caption{Melhores Parametros para cada Execucao}
	\\label{tab:best_parms_for_avg_db}
\\end{table}";
}
