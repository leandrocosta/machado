DOCUMENT=thesis
DOCUMENT_TEX=${DOCUMENT}.tex

#all: thesis

graphs_all:
	make -C graphs/ all
graphs_clean:
	make -C graphs/ clean

img_all:
	make -C img/ all
img_clean:
	make -C img/ clean

#thesis: pdf ps
pdf: graphs_all img_all thesis.pdf
#ps: thesis.ps
#dvi: thesis.dvi

%.pdf: %.tex %.bbl
	latex $<
	pdflatex $<

#%.ps: %.tex %.bbl
#	latex $<
#	latex $<
#	dvips $*.dvi

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
