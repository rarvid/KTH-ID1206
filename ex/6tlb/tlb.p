set terminal png
set output "tlb.png"

set title "TLB benchmark, 4 KiByte pages, 10 Gi operations"

set key right center

set xlabel "number of pages"

set ylabel "time in s"

plot "tlb.dat" u 1:2 w linespoints title "page refs"