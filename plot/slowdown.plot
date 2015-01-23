set term pos eps colo
set title 'slow down without filter'
set output 'slowdown.eps'
set xlabel 'time(seconds)'
set ylabel 'slow down'
#set yrange [0:4]
set xrange [0:10800]  
plot "file.txt" using 1:7 title "slow down", \
	[0:] y = 1 title ""