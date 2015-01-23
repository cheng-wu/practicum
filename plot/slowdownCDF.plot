set term pos eps colo
set title 'CDF of slowdowns'
set output 'slowdownCDF1.eps'
set xlabel 'slowdowns'
set ylabel 'CDF'

plot "cdf1.txt" using 1:2 title "slowdown" with lines