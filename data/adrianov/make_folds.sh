#!/bin/bash

NUM_FOLDS=10;

mkdir -p rand
mkdir -p class
mkdir -p fold

for file in `ls -1 orig/ | grep ac`;
do
	# sort data randomly
	echo "sorting $file ...";
	sort -R orig/$file >rand/$file;

	# change classes from SPAM= to CLASS=
	echo "fixing classes for $file ..."
	cat rand/$file | sed -e s/SPAM/CLASS/g >class/$file

	size=`wc -l class/$file | awk '{ print $1 }'`;
	testing=$(($size/$NUM_FOLDS));
	onemore=$(($size%$NUM_FOLDS));

	echo "size: $size, testing: $testing, onemore: $onemore";

	mkdir -p fold/$file

	echo "making $file.0.training and $file.0.testing with $testing elements";

	# split data in training and testing
	cat class/$file | split -`expr $size - $testing`;

	mv xaa fold/$file/$file.0.training;
	mv xab fold/$file/$file.0.testing;

	for ((i=1; $i<10; i=$i+1))
	do
		if (($NUM_FOLDS - $i == $onemore))
		then
			testing=$(($testing+1));
		fi;

		echo "making $file.$i.training and $file.$i.testing with $testing elements";

		# split data in training and testing
		cat fold/$file/$file.$(($i-1)).testing fold/$file/$file.$(($i-1)).training | split -`expr $size - $testing`;

		mv xaa fold/$file/$file.$i.training
		mv xab fold/$file/$file.$i.testing
	done;
done;
