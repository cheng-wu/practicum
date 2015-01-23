set term pos eps colo
set title 'CDF of slowdowns'
set output 'slowdownCDF2.eps'
set xlabel 'slowdowns'
set ylabel 'CDF'
set yrange [0.95:1]

plot "cdf2.txt" using 1:2 title "slowdown" with lines