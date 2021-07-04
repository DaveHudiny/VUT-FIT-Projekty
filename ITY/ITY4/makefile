all:
	latex proj4.tex
	bibtex proj4.aux
	latex proj4.tex
	latex proj4.tex
	dvips -t a4 proj4.dvi
	ps2pdf -sPAPERSIZE=a4 proj4.ps
