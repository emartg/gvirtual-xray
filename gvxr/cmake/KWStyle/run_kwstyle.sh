#!/bin/bash

rm -f filelist.txt

FILES=`ls ../../include/gVirtualXRay/*.h \
          ../../include/gVirtualXRay/*.inl \
          ../../src/*/*.cxx \
          #../../examples/*/*.h \
          #../../examples/*/*.inl \
          #../../examples/*/*.cxx \
          #../../examples/*/*.vert \
          #../../examples/*/*.frag \
          ../../resources/*.vert \
          ../../resources/*.frag \
          ../../tests/*.cxx \
          #../../tutorials/*/*.h \
          #../../tutorials/*/*.cxx \
          #../../validation/src/*.cxx \
          #../../validation/shaders/*.vert \
          #../../validation/shaders/*.frag \
          `

#FILES=`ls ../../examples/*/*.h ../../examples/*/*.inl ../../examples/*/*.cxx ../../examples/*/*.vert ../../examples/*/*.frag`

#FILES=`ls ../../include/gVirtualXRay/*.h`


for i in $FILES
do
    echo $i >>filelist.txt
done

KWStyle -xml gVirtualXRay.kws.xml -v -D filelist.txt -o overwrite.txt  -html results
