set terminal png size 1000,600
set output 'acc_graph_21.png'
set title 'Accuracy Progress (Session 1 to 11)'
set xlabel 'Session Number'
set ylabel 'Accuracy (%)'
set yrange [0:100]
set grid
plot 'acc_data.txt' using 1:2 with linespoints title 'Accuracy'
