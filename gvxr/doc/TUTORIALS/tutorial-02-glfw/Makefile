DOC=tutorial-02-glfw

all:$(DOC).pdf $(DOC).html

%.pdf : %.tex
	pdflatex $<
	#bibtex $(<:%.tex=%)
	#makeglossaries $(<:%.tex=%)
	pdflatex $<
	pdflatex $<
	$(RM) $(DOC).aux
	$(RM) $(DOC).blg
	$(RM) $(DOC).log
	$(RM) $(DOC).out
	$(RM) $(DOC).bbl
	$(RM) $(DOC).lof
	$(RM) $(DOC).lol
	$(RM) $(DOC).toc

%.html : %.tex screenshot.eps
	hevea -fix $<
	$(RM) $(DOC).haux
	$(RM) $(DOC).htoc
	$(RM) $(DOC).image.out
	$(RM) $(DOC).image.tex
	$(RM) screenshot.eps

%.eps : %.png
	convert $< $@

%.eps : %.fig
	fig2eps --nogv $<

%.pdf : %.fig
	fig2pdf --nogv $<

clean:
	$(RM) $(DOC).pdf
	$(RM) $(DOC).html
	$(RM) $(DOC).aux
	$(RM) $(DOC).blg
	$(RM) $(DOC).log
	$(RM) $(DOC).out
	$(RM) $(DOC).bbl
	$(RM) $(DOC).lof
	$(RM) $(DOC).lol
	$(RM) $(DOC).toc
	$(RM) $(DOC).haux
	$(RM) $(DOC).htoc
	$(RM) $(DOC).image.out
	$(RM) $(DOC).image.tex
