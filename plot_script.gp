set terminal png size 1000,600
set output 'wpm_graph_22.png'
set title 'WPM Progress (Session 1 to 11)'
set xlabel 'Session Number'
set ylabel 'WPM'
set grid
plot 'wpm_data.txt' using 1:2 with linespoints title 'WPM'
