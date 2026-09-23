#!/usr/bin/sh

if [ -d ../dist_doc1 ];
then
    rm -rf ../doc/dist_doc1
fi

if [ -d ../dist_doc2 ];
then
    rm -rf ../doc/dist_doc2
fi

mkdir ../doc/dist_doc1
mkdir ../doc/dist_doc2

cd ../doc/latex;     make&&cp refman.pdf  ../dist_doc1/gVirtualXRay-programmer-guide.pdf;
cd ../INSTALL;   ./run.sh&&cp install.pdf ../dist_doc1/gVirtualXRay-install.pdf;
cd ..;
                           cp ../LICENSE.txt dist_doc1/;
                           cp Doxyfile       dist_doc2/;
                           cp logo.ico       dist_doc2/;
                           cp logo.png       dist_doc2/;

