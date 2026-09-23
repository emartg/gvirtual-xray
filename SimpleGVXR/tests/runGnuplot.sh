FILES=`ls *.plt`

for f in $FILES;
do
    echo `basename $f .plt`
    gnuplot < $f
    gv `basename $f .plt`.eps
done

