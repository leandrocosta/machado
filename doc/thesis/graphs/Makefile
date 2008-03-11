all: bst_histogram_acc.pdf bst_histogram_pat.pdf bst_histogram_rul.pdf histogram_acc.pdf histogram_ometric_acc.pdf histogram_ometric_pat.pdf histogram_ometric_rul.pdf histogram_pat.pdf histogram_rul.pdf

%.pdf: %.eps
	epstopdf $<

clean:
	rm -rf *.pdf *~
