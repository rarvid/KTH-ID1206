# Gnuplot script for plotting data in all policy files combined

set terminal png
set output "merge.png"

set title "Effect of sort the freelist when merging the free blocks"

set key right center

set xlabel "Number of memory requests"
set ylabel "Number of blocks"

set xrange [0:100]
set yrange [0:35]

plot "merge_only.txt" u 1 w linespoints title "freelist unsorted", \
     "merge_only.txt" u 2 w linespoints title "arena unsorted", \
     "merge_and_sort.txt" u 1 w linespoints title "freelist sorted", \
     "merge_and_sort.txt" u 2 w linespoints title "arena sorted", \