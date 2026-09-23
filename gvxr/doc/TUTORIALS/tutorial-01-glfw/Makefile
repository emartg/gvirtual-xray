DOC=tutorial-01-glfw

all:$(DOC).pdf $(DOC).php

pass1 : $(DOC).html
	@head -n 5 $< > $@
	@echo "    <meta name="description" content="gVirtualXRay: Virtual X-Ray Imaging Library on GPU" />" >> $@
	@echo "    <meta name="robots" Ccontent="index, follow" />" >> $@
	@echo "    <meta name="revisit-after" content="30 days" />" >> $@
	@echo "    <meta name="keywords" content="X-ray simulation,GPU programing,OpenGL,GLSL,medical imaging,simulation" />" >> $@
	@echo "    <link rel="stylesheet" href="../../../css/main.css" type="text/css" media="screen, projection" />" >> $@
	@echo "    <link rel="shortcut icon" href="img/logo.ico" type="image/x-icon">" >> $@
	@echo "        <?php include_once(\"../../../analyticstracking.php\") ?>" >> $@
	NB_LINES=$(shell echo `awk 'END{print NR}' $<`) && \
	OFFSET=5 && \
	DIFF_SUB=$$(($$NB_LINES - $$OFFSET)) && \
	tail -n $$((DIFF_SUB)) $< >> $@

pass2 : pass1
	sed 's/\<body \>*/\<body\>\<div id="header"\>\<?php include("\.\.\/\.\.\/menu.php"); ?\>\<\/div\>/' $< > $@
	$(RM) $<
	#\<body\>\\n
	#\\n
	#\\n
	#\<!-- begin header which contains our heading and navigation --\>\\n
	#
	#\<!-- end header --\>


$(DOC).php : pass2
	NB_LINES=$(shell echo `awk 'END{print NR}' $<`) && \
	OFFSET=2 && \
	DIFF_SUB=$$(($$NB_LINES - $$OFFSET)) && \
	head -n $$((DIFF_SUB)) $< > $@
	@echo "    </em><a href="http://hevea.inria.fr/index.html"><em>H</em><em><span style="font-size:small"><sup>E</sup></span></em><em>V</em><em><span style="font-size:small"><sup>E</sup></span></em><em>A</em></a><em>.</em></blockquote>" >> $@
	@echo "    </div></div><!-- end main column -->" >> $@
	@echo "    <!-- begin footer -->" >> $@
	@echo "    <?php include('../../../footer.php'); ?>" >> $@
	@echo "    <!-- end footer -->" >> $@
	@echo "    </body>" >> $@
	@echo "    </html>" >> $@
	@$(RM) $<

%.pdf : %.tex cube_01.pdf cube_02.pdf
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
	$(RM) cube_01.pdf cube_02.pdf

%.html : %.tex cube_01.eps cube_02.eps screenshot.eps
	hevea -fix $<
	$(RM) $(DOC).haux
	$(RM) $(DOC).htoc
	$(RM) $(DOC).image.out
	$(RM) $(DOC).image.tex
	#$(RM) cube_01.eps cube_02.eps screenshot.eps

%.eps : %.png
	convert $< $@

%.eps : %.fig
	fig2eps --nogv $<

%.pdf : %.fig
	fig2pdf --nogv $<

clean:
	$(RM) $(DOC).pdf
	$(RM) $(DOC).php
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
