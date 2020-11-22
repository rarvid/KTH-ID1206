# Gnuplot script for plotting data in file "random.dat"

set terminal png
set output "random.png"

set title "Page replacment using random policy"

set key right center

set xlabel "frames in memory"
set ylabel "hit ratio"

set xrange [0:100]
set yrange [0:1]

plot "random.dat" u 1:2 w linespoints title "random"