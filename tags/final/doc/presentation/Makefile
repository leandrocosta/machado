DOCUMENT=presentation
DOCUMENT_TEX=${DOCUMENT}.tex

all: pdf

graphs_all:
	make -C ../thesis/graphs/ all

img_all:
	make -C ../thesis/img/ all

THESIS_TABLES = ../thesis/tables/table_best_parms_for_avg_db.tex ../thesis/tables/table_comparison_lac_olac.tex ../thesis/tables/table_test_parms.tex

THESIS_EPS_GRAPHS = ../thesis/graphs/histogram_best_run_for_avg_db_acc.eps ../thesis/graphs/histogram_best_run_for_avg_db_pat.eps ../thesis/graphs/histogram_best_run_for_avg_db_rul.eps ../thesis/graphs/histogram_best_run_for_avg_db_tim.eps ../thesis/graphs/histogram_best_run_for_each_db_acc.eps ../thesis/graphs/histogram_best_run_for_each_db_pat.eps ../thesis/graphs/histogram_best_run_for_each_db_rul.eps ../thesis/graphs/histogram_best_run_for_each_db_tim.eps

THESIS_PDF_GRAPHS = ../thesis/graphs/histogram_best_run_for_avg_db_acc.pdf ../thesis/graphs/histogram_best_run_for_avg_db_pat.pdf ../thesis/graphs/histogram_best_run_for_avg_db_rul.pdf ../thesis/graphs/histogram_best_run_for_avg_db_tim.pdf ../thesis/graphs/histogram_best_run_for_each_db_acc.pdf ../thesis/graphs/histogram_best_run_for_each_db_pat.pdf ../thesis/graphs/histogram_best_run_for_each_db_rul.pdf ../thesis/graphs/histogram_best_run_for_each_db_tim.pdf

presentation.pdf: presentation.tex introducao.tex classificacao.tex ortogonalidade.tex avaliacao.tex conclusao.tex ${THESIS_TABLES} ${THESIS_PDF_GRAPHS}
presentation.ps: presentation.tex introducao.tex classificacao.tex ortogonalidade.tex avaliacao.tex conclusao.tex  ${THESIS_TABLES} ${THESIS_EPS_GRAPHS}
presentation.dvi: presentation.tex introducao.tex classificacao.tex ortogonalidade.tex avaliacao.tex conclusao.tex  ${THESIS_TABLES} ${THESIS_EPS_GRAPHS}
pdf: graphs_all img_all presentation.pdf
ps: presentation.ps
#dvi: presentation.dvi

%.pdf: %.tex
	pdflatex $<
	pdflatex $<

%.ps: %.tex
	latex $<
	latex $<
	dvips $*.dvi -o $*.ps

#%.ps: %.dvi
#	dvips $< -o $*.ps

#%.dvi: %.tex %.bbl
#	latex $<
#	latex $<

%.aux: %.tex
	latex $<

clean:
	rm -rf *.dvi *.blg *.toc *.pdf *.ps *.out *.lot *.log *.lof *.aux *~
