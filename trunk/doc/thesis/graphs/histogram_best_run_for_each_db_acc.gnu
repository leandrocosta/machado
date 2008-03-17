set term postscript eps enhanced color
set output 'output/graphs/histogram_best_run_for_each_db_acc.eps'
set boxwidth 0.9 absolute
set style fill solid 1.00 border -1
set style histogram clustered gap 1 title offset character 0, 0, 0
set datafile missing '-'
set style data histograms
set xtics border in scale 1,0.5 nomirror rotate by -45 offset character 0, 0, 0 
set xtics ("anneal.ac" 0, "austra.ac" 1, "auto.ac" 2, "breast.ac" 3, "cleve.ac" 4, "crx.ac" 5, "diabetes.ac" 6, "german.ac" 7, "glass.ac" 8, "heart.ac" 9, "hepati.ac" 10, "horse.ac" 11, "hypo.ac" 12, "ionosphere.ac" 13, "iris.ac" 14, "labor.ac" 15, "led7.ac" 16, "lymph.ac" 17, "pima.ac" 18, "sick.ac" 19, "sonar.ac" 20, "tic-tac.ac" 21, "vehicle.ac" 22, "waveform.ac" 23, "wine.ac" 24, "zoo.ac" 25, "average" 26)
set title "Accuracy Histogram"
set xlabel "Data Sets"
set ylabel "Accuracy"
plot 'output/graphs/histogram_best_run_for_each_db_acc.dat' using 2:xtic(1) ti col, '' u 3 ti col, '' u 4 ti col, '' u 5 ti col