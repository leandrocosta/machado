all: brasao.pdf coverage.pdf

%.pdf: %.eps
	epstopdf $<

clean:
	rm -rf *.pdf *~
