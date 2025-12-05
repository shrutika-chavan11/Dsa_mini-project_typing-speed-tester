set terminal png size 900,450
set output 'wpm_plot.png'
set title 'WPM - Session History'
set xlabel 'Session / Time Index'
set ylabel 'WPM'
set grid
plot 'data.txt' using 1:2 with linespoints linewidth 2 pointtype 7
