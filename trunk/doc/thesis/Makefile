DOCUMENT=thesis
DOCUMENT_TEX=${DOCUMENT}.tex

all: pdf

graphs_all:
	make -C graphs/ all
graphs_clean:
	make -C graphs/ clean

img_all:
	make -C img/ all
img_clean:
	make -C img/ clean

THESIS_TABLES = tables/table_best_parms_for_avg_db.tex tables/table_best_parms_for_avg_db_uc.tex tables/table_best_runs_lac.tex tables/table_best_runs_lac_uc.tex tables/table_best_runs_olac.tex tables/table_best_runs_olac_uc.tex tables/table_best_runs_origami.tex tables/table_best_runs_origami_uc.tex tables/table_comparison_lac_olac.tex tables/table_comparison_lac_olac_uc.tex tables/table_comparison_lac_origami.tex tables/table_comparison_lac_origami_uc.tex tables/table_comparison_olac_origami.tex tables/table_comparison_olac_origami_uc.tex tables/table_example_run_parms.tex tables/table_example_training_db.tex tables/table_lac_x_olac_austra.ac.lac_patterns.tex tables/table_lac_x_olac_austra.ac.lac_rules.tex tables/table_lac_x_olac_austra.ac.olac_patterns.tex tables/table_lac_x_olac_austra.ac.olac_rules.tex tables/table_lac_x_olac_austra.ac.test_instance_1.tex tables/table_lac_x_olac_breast.ac.lac_patterns.tex tables/table_lac_x_olac_breast.ac.lac_rules.tex tables/table_lac_x_olac_breast.ac.olac_patterns.tex tables/table_lac_x_olac_breast.ac.olac_rules.tex tables/table_lac_x_olac_breast.ac.test_instance_1.tex tables/table_lac_x_origami_labor.ac.lac_patterns.tex tables/table_lac_x_origami_labor.ac.lac_rules.tex tables/table_lac_x_origami_labor.ac.origami_patterns.tex tables/table_lac_x_origami_labor.ac.origami_rules.tex tables/table_lac_x_origami_labor.ac.test_instance_1.tex tables/table_test_parms.tex

THESIS_EPS_GRAPHS = graphs/histogram_best_run_for_avg_db_acc.eps graphs/histogram_best_run_for_avg_db_acc_uc.eps graphs/histogram_best_run_for_avg_db_ometric_acc.eps graphs/histogram_best_run_for_avg_db_ometric_acc_uc.eps graphs/histogram_best_run_for_avg_db_ometric_pat.eps graphs/histogram_best_run_for_avg_db_ometric_pat_uc.eps graphs/histogram_best_run_for_avg_db_ometric_rul.eps graphs/histogram_best_run_for_avg_db_ometric_rul_uc.eps graphs/histogram_best_run_for_avg_db_ometric_tim.eps graphs/histogram_best_run_for_avg_db_ometric_tim_uc.eps graphs/histogram_best_run_for_avg_db_pat.eps graphs/histogram_best_run_for_avg_db_pat_uc.eps graphs/histogram_best_run_for_avg_db_rul.eps graphs/histogram_best_run_for_avg_db_rul_uc.eps graphs/histogram_best_run_for_avg_db_tim.eps graphs/histogram_best_run_for_avg_db_tim_uc.eps graphs/histogram_best_run_for_each_db_acc.eps graphs/histogram_best_run_for_each_db_acc_uc.eps graphs/histogram_best_run_for_each_db_pat.eps graphs/histogram_best_run_for_each_db_pat_uc.eps graphs/histogram_best_run_for_each_db_rul.eps graphs/histogram_best_run_for_each_db_rul_uc.eps graphs/histogram_best_run_for_each_db_tim.eps graphs/histogram_best_run_for_each_db_tim_uc.eps graphs/histogram_best_run_for_lac_avg_db_acc.eps graphs/histogram_best_run_for_lac_avg_db_pat.eps graphs/histogram_best_run_for_lac_avg_db_rul.eps graphs/histogram_best_run_for_lac_avg_db_tim.eps

THESIS_PDF_GRAPHS = graphs/histogram_best_run_for_avg_db_acc.pdf graphs/histogram_best_run_for_avg_db_acc_uc.pdf graphs/histogram_best_run_for_avg_db_ometric_acc.pdf graphs/histogram_best_run_for_avg_db_ometric_acc_uc.pdf graphs/histogram_best_run_for_avg_db_ometric_pat.pdf graphs/histogram_best_run_for_avg_db_ometric_pat_uc.pdf graphs/histogram_best_run_for_avg_db_ometric_rul.pdf graphs/histogram_best_run_for_avg_db_ometric_rul_uc.pdf graphs/histogram_best_run_for_avg_db_ometric_tim.pdf graphs/histogram_best_run_for_avg_db_ometric_tim_uc.pdf graphs/histogram_best_run_for_avg_db_pat.pdf graphs/histogram_best_run_for_avg_db_pat_uc.pdf graphs/histogram_best_run_for_avg_db_rul.pdf graphs/histogram_best_run_for_avg_db_rul_uc.pdf graphs/histogram_best_run_for_avg_db_tim.pdf graphs/histogram_best_run_for_avg_db_tim_uc.pdf graphs/histogram_best_run_for_each_db_acc.pdf graphs/histogram_best_run_for_each_db_acc_uc.pdf graphs/histogram_best_run_for_each_db_pat.pdf graphs/histogram_best_run_for_each_db_pat_uc.pdf graphs/histogram_best_run_for_each_db_rul.pdf graphs/histogram_best_run_for_each_db_rul_uc.pdf graphs/histogram_best_run_for_each_db_tim.pdf graphs/histogram_best_run_for_each_db_tim_uc.pdf

thesis.pdf: thesis.tex agradecimentos.tex classificacao.tex conclusao.tex dedicatoria.tex introducao.tex ortogonalidade.tex resultados.tex resumo.tex ${THESIS_TABLES} ${THESIS_PDF_GRAPHS}
thesis.ps: thesis.tex agradecimentos.tex classificacao.tex conclusao.tex dedicatoria.tex introducao.tex ortogonalidade.tex resultados.tex resumo.tex ${THESIS_TABLES} ${THESIS_EPS_GRAPHS}
thesis.dvi: thesis.tex agradecimentos.tex classificacao.tex conclusao.tex dedicatoria.tex introducao.tex ortogonalidade.tex resultados.tex resumo.tex ${THESIS_TABLES} ${THESIS_EPS_GRAPHS}
pdf: graphs_all img_all thesis.pdf
ps: thesis.ps
#dvi: thesis.dvi

%.pdf: %.tex %.bbl
	latex $<
	pdflatex $<

%.ps: %.tex %.bbl
	latex $<
	latex $<
	dvips $*.dvi

#%.ps: %.dvi
#	dvips $<

#%.dvi: %.tex %.bbl
#	latex $<
#	latex $<

%.bbl: %.bib %.aux
	bibtex $*

%.aux: %.tex
	latex $<

clean: graphs_clean img_clean
	rm -rf *.dvi *.blg *.bbl *.toc *.pdf *.ps *.out *.lot *.log *.lof *.aux *~
