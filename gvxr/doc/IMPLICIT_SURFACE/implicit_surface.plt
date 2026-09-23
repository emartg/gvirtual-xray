set terminal postscript enhanced color
set output "density_function.eps"

set xrange [0:4]
set yrange [0:3]

set xlabel "r"

plot "implicit_surface.dat" using 1:2 w l linecolor rgb "black" lt 1 t "1 / r^2", \
                         "" using 1:3 w l linecolor rgb "red"   lt 1 t "Blobby molecules a = 2, b = 1", \
                         "" using 1:4 w l linecolor rgb "green" lt 1 t "Meta balls a = 1, b = 3", \
                         "" using 1:5 w l linecolor rgb "blue"  lt 1 t "Soft objects a = 1, b = 3"
