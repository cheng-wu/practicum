set term pos eps colo
set title 'latency for each device without filter'
set output 'latency.eps'
set xlabel 'time(seconds)'
set ylabel 'latency time(ms)'
set xrange [0:10800] 
#set yrange [400:5000] 
plot "file.txt" using 1:2 title "device 1", \
	'' using 1:3 title "device 2", \
	'' using 1:4 title "device 3"