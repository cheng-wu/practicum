set term eps
set title 'CDF of slowdowns for Write Throughput'
set output 'eps/WT.eps'
set xlabel 'Slowdowns'
set ylabel 'CDF'
set yrange [0.95:1]
set logscale x 2
set key bot right
plot 'raw/SSD/WT.dat' using 1:2 title 'SSDs' with lines lt 1, \
'raw/HD/WT.dat' using 1:2 title 'HDs' with lines lt 2, \
'raw/VR/WT.dat' using 1:2 title 'Virtual Disks' with lines lt 3
