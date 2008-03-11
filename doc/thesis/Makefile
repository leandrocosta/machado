DOCUMENT=thesis
DOCUMENT_TEX=${DOCUMENT}.tex

all: graphs_all thesis

graphs_all:
	make -C graphs/ all

graphs_clean:
	make -C graphs/ clean

thesis: thesis.pdf

%.pdf: %.tex %.bbl
	pdflatex $<

%.bbl: %.bib %.aux
	bibtex $*

%.aux: %.tex
	latex $<

#latex:
#	latex ${DOCUMENT_TEX}
#
#bibtex:
#	bibtex ${DOCUMENT}
#
clean: graphs_clean
	rm -rf *.dvi *.blg *.bbl *.toc *.pdf *.out *.lot *.log *.lof *.aux *~
