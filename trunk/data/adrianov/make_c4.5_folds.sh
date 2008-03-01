#!/bin/bash

NUM_FOLDS=10;
FOLD=fold.c4.5

mkdir -p c4.5.input
mkdir -p c4.5.output
mkdir -p rand
mkdir -p class
mkdir -p $FOLD

for file in `ls -1 orig/ | grep ac`;
do
	# changing files to c4.5 input
	echo "changing $file to c4.5 input...";
	cat orig/$file | perl -e "while (<STDIN>){chomp;@_=split(' ',$_);shift (@_);push (@_,shift (@_));print(join(' ',@_));print \"\n\";}" >c4.5.input/$file;

	# running c4.5 to files
	echo "running c4.5 in $file...";
	cat c4.5.input/$file | ./xval-prep $NUM_FOLDS 1>c4.5.output/$file 2>c4.5.output/$file.log

	# changing c4.5 output files to my input
	echo "changing c4.5 output to my input...";
	cat c4.5.output/$file | perl -e "\$i=1;while(<STDIN>){chomp;chop;@_=split(' ',$_);unshift(@_,pop(@_));unshift(@_,\$i++);print(join(' ',@_));print(\"\n\");}" >rand/$file;

	# change classes from SPAM= to CLASS=
	echo "fixing classes for $file ..."
	cat rand/$file | sed -e s/SPAM/CLASS/g >class/$file

	size=`wc -l class/$file | awk '{ print $1 }'`;
	testing=$(($size/$NUM_FOLDS));
	onemore=$(($size%$NUM_FOLDS));

	echo "size: $size, testing: $testing, onemore: $onemore";

	mkdir -p $FOLD/$file

	echo "making $file.0.training and $file.0.testing with $testing elements";

	# split data in training and testing
	cat class/$file | split -`expr $size - $testing`;

	mv xaa $FOLD/$file/$file.0.training;
	mv xab $FOLD/$file/$file.0.testing;

	for ((i=1; $i<10; i=$i+1))
	do
		if (($NUM_FOLDS - $i == $onemore))
		then
			testing=$(($testing+1));
		fi;

		echo "making $file.$i.training and $file.$i.testing with $testing elements";

		# split data in training and testing
		cat $FOLD/$file/$file.$(($i-1)).testing $FOLD/$file/$file.$(($i-1)).training | split -`expr $size - $testing`;

		mv xaa $FOLD/$file/$file.$i.training
		mv xab $FOLD/$file/$file.$i.testing
	done;
done;
