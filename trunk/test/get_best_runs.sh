rm best_runs_lac.txt
rm best_runs_olac.txt
rm best_runs_origami.txt

for file in `find output/classifier_c/ -name best.out`; do more $file >>best_runs_lac.tmp; done;
sed -e "s/, avg_time.*//g" best_runs_lac.tmp | sort | uniq >best_runs_lac.txt
rm best_runs_lac.tmp

for file in `find output/classifier_o/ -name best.out`; do more $file >>best_runs_olac.tmp; done;
sed -e "s/, avg_time.*//g" best_runs_olac.tmp | sort | uniq >best_runs_olac.txt
rm best_runs_olac.tmp

for file in `find output/classifier_or/ -name best.out`; do more $file >>best_runs_origami.tmp; done;
sed -e "s/, avg_time.*//g" best_runs_origami.tmp | sort | uniq >best_runs_origami.txt
rm best_runs_origami.tmp
