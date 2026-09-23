#!/usr/bin/sh

MD_FILES=`ls *.md`
for file in $MD_FILES
do
    fbname=$(basename "$file" .md);
    discount-markdown $file > $fbname.html
done
