DOCUMENT=thesis
DOCUMENT_TEX=${DOCUMENT}.tex

all: graphs_all img_all thesis

graphs_all:
	make -C graphs/ all

graphs_clean:
	make -C graphs/ clean

img_all:
	make -C img/ all

img_clean:
	make -C img/ clean

thesis: thesis.pdf

%.pdf: %.tex %.bbl
	latex $<
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
clean: graphs_clean img_clean
	rm -rf *.dvi *.blg *.bbl *.toc *.pdf *.out *.lot *.log *.lof *.aux *~
