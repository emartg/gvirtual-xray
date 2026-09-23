DOCS=README.html \
	README.pdf \
	\
	1-overview/README.html \
	1-overview/1-gVirtualRay-overview-IBSim-4i_2020.html \
	1-overview/1-gVirtualRay-overview-IBSim-4i_2020.pdf \
	\
	2-theory/README.html \
	2-theory/2-gVirtualRay-theory-IBSim-4i_2020.html \
	2-theory/2-gVirtualRay-theory-IBSim-4i_2020.pdf \
	\
	3-implementation/README.html \
	3-implementation/3-gVirtualRay-implementation-IBSim-4i_2020.html \
	3-implementation/3-gVirtualRay-implementation-IBSim-4i_2020.pdf \
	\
	4-validation/README.html \
	4-validation/4-gVirtualRay-validation-IBSim-4i_2020.html \
	4-validation/4-gVirtualRay-validation-IBSim-4i_2020.pdf \
	\
	5-install/README.html \
	5-install/5-gVirtualRay-install-IBSim-4i_2020.html \
	5-install/5-gVirtualRay-install-IBSim-4i_2020.pdf \
	\
	6-coding/README.html \
	6-coding/6-gVirtualRay-coding-IBSim-4i_2020.html \
	6-coding/6-gVirtualRay-coding-IBSim-4i_2020.pdf \

all: ${DOCS}

%.pptx: %.md
	pandoc -s $< -o $@ #--reference-doc theme.pptx

%.html: %.md
	pandoc -t  slidy -s $< -o $@
	# pandoc -t  slideous -s $< -o $@
	#pandoc -t  dzslides -s $< -o $@

%.pdf: %.md
	# pandoc -t  beamer -s $< -o $@
	pandoc -t context -s $< -o $@

clean:
	${RM} ${DOCS}
