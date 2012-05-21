name = report
source = $(name).tex

all: $(name).pdf

$(name).pdf: $(source)
	xelatex $(source)

ref:
	xelatex $(source)

full:
	xelatex $(source)
	xelatex $(source)
	bibtex $(name)
	xelatex $(source)
	xelatex $(source)

clean :
	-rm *.aux *.toc *.log *.out *.thm *.snm *.nav

distclean :
	-rm report.dvi report.pdf

