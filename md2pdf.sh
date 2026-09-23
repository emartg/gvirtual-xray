#!/bin/bash

FILES=$(ls ./*.md)

PANDOC='/usr/bin/env pandoc'

#PANDOC_PDF_OPTIONS='-fmarkdown-implicit_figures --pdf-engine=pdflatex -V geometry:margin=1in'
PANDOC_PDF_OPTIONS='-fmarkdown-implicit_figures -t context'



for file in $FILES
do
    echo $file
    OUTPUT="$(basename $file .md).pdf"
    $PANDOC -s $file $PANDOC_PDF_OPTIONS -o $OUTPUT

    OUTPUT="$(basename $file .md).html"
    $PANDOC -s $file -o $OUTPUT
done
