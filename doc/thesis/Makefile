DOCUMENT=thesis
DOCUMENT_TEX=${DOCUMENT}.tex

thesis: thesis.pdf

%.pdf: %.tex %.bbl
	latex $<

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
clean:
	rm -rf *.dvi *.blg *.bbl *.toc *.pdf *.out *.lot *.log *.lof *.aux *~
