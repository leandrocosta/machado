#!/bin/bash

#SUPPORT=0.1
#CONFIDENCE=0.9
SUPPORT=0.1
CONFIDENCE=0.1
#SUPPORT=0.9
#CONFIDENCE=0.1

#./lazy -i audiology.training -t audiology.testing >lazy.audiology.out
#./lazy -i car.training -t car.testing >lazy.car.out
#./lazy -i balance-scale.training -t balance-scale.testing >lazy.balance-scale.out
#./lazy -i nursery.training -t nursery.testing >lazy.nursery.out

#./classifier -i audiology.training -t audiology.testing 0.1 0.1 c 0 >classifier.c.audiology.out
#./classifier -i car.training -t car.testing 0.1 0.1 c 0 >classifier.c.car.out
#./classifier -i balance-scale.training -t balance-scale.testing 0.1 0.1 c 0 >classifier.c.balance-scale.out
#./classifier -i nursery.training -t nursery.testing 0.1 0.1 c 0 >classifier.c.nursery.out

#./classifier -i audiology.training -t audiology.testing 0.1 0.1 o 0 >classifier.o.audiology.out
#./classifier -i car.training -t car.testing 0.1 0.1 o 0 >classifier.o.car.out
#./classifier -i balance-scale.training -t balance-scale.testing 0.1 0.1 o 0 >classifier.o.balance-scale.out
#./classifier -i nursery.training -t nursery.testing 0.1 0.1 o 0 >classifier.o.nursery.out

###

rm Print

# NO
#./lazy -i auto.ac.training -t auto.ac.testing -s 1 -c $CONFIDENCE -m 10 >lazy.auto.ac.out
#./classifier -i auto.ac.training -t auto.ac.testing -s $SUPPORT -c $CONFIDENCE -m c -a 1 -d 0 >classifier.c.auto.ac.out
#./classifier -i auto.ac.training -t auto.ac.testing -s $SUPPORT -c $CONFIDENCE -m o -a 1 -d 0 >classifier.o.auto.ac.out
#./run.pl auto.ac

# YES
./lazy -i tic-tac.ac.training -t tic-tac.ac.testing -s 1 -c $CONFIDENCE -m 10 >lazy.tic-tac.ac.out
./classifier -i tic-tac.ac.training -t tic-tac.ac.testing -s $SUPPORT -c $CONFIDENCE -m c -a 1 -d 0 >classifier.c.tic-tac.ac.out
./classifier -i tic-tac.ac.training -t tic-tac.ac.testing -s $SUPPORT -c $CONFIDENCE -m o -a 1 -d 0 >classifier.o.tic-tac.ac.out
./run.pl tic-tac.ac >>Print

# NO
#./lazy -i anneal.ac.training -t anneal.ac.testing -s 1 -c $CONFIDENCE -m 10 >lazy.anneal.ac.out
#./classifier -i anneal.ac.training -t anneal.ac.testing -s $SUPPORT -c $CONFIDENCE -m c -a 1 -d 0 >classifier.c.anneal.ac.out
#./classifier -i anneal.ac.training anneal.ac.testing -s $SUPPORT -c $CONFIDENCE -m o -a 1 -d 0 >classifier.o.anneal.ac.out
#./run.pl anneal.ac >>Print

# YES
./lazy -i lymph.ac.training -t lymph.ac.testing -s 1 -c $CONFIDENCE -m 10 >lazy.lymph.ac.out
./classifier -i lymph.ac.training -t lymph.ac.testing -s $SUPPORT -c $CONFIDENCE -m c -a 1 -d 0 >classifier.c.lymph.ac.out
./classifier -i lymph.ac.training -t lymph.ac.testing -s $SUPPORT -c $CONFIDENCE -m o -a 1 -d 0 >classifier.o.lymph.ac.out
./run.pl lymph.ac >>Print

# YES (BETTER - SUPPORT 0.1, CONFIDENCE 0.1)
./lazy -i austra.ac.training -t austra.ac.testing -s 1 -c $CONFIDENCE -m 10 >lazy.austra.ac.out
./classifier -i austra.ac.training -t austra.ac.testing -s $SUPPORT -c $CONFIDENCE -m c -a 1 -d 0 >classifier.c.austra.ac.out
./classifier -i austra.ac.training -t austra.ac.testing -s $SUPPORT -c $CONFIDENCE -m o -a 1 -d 0 >classifier.o.austra.ac.out
./run.pl austra.ac >>Print

# YES
./lazy -i breast.ac.training -t breast.ac.testing -s 1 -c $CONFIDENCE -m 10 >lazy.breast.ac.out
./classifier -i breast.ac.training -t breast.ac.testing -s $SUPPORT -c $CONFIDENCE -m c -a 1 -d 0 >classifier.c.breast.ac.out
./classifier -i breast.ac.training -t breast.ac.testing -s $SUPPORT -c $CONFIDENCE -m o -a 1 -d 0 >classifier.o.breast.ac.out
./run.pl breast.ac >>Print

./lazy -i iris.ac.training -t iris.ac.testing -s 1 -c $CONFIDENCE -m 10 >lazy.iris.ac.out
./classifier -i iris.ac.training -t iris.ac.testing -s $SUPPORT -c $CONFIDENCE -m c -a 1 -d 0 >classifier.c.iris.ac.out
./classifier -i iris.ac.training -t iris.ac.testing -s $SUPPORT -c $CONFIDENCE -m o -a 1 -d 0 >classifier.o.iris.ac.out
./run.pl iris.ac >>Print

./lazy -i labor.ac.training -t labor.ac.testing -s 1 -c $CONFIDENCE -m 10 >lazy.labor.ac.out
./classifier -i labor.ac.training -t labor.ac.testing -s $SUPPORT -c $CONFIDENCE -m c -a 1 -d 0 >classifier.c.labor.ac.out
./classifier -i labor.ac.training -t labor.ac.testing -s $SUPPORT -c $CONFIDENCE -m o -a 1 -d 0 >classifier.o.labor.ac.out
./run.pl labor.ac >>Print

./lazy -i wine.ac.training -t wine.ac.testing -s 1 -c $CONFIDENCE -m 10 >lazy.wine.ac.out
./classifier -i wine.ac.training -t wine.ac.testing -s $SUPPORT -c $CONFIDENCE -m c -a 1 -d 0 >classifier.c.wine.ac.out
./classifier -i wine.ac.training -t wine.ac.testing -s $SUPPORT -c $CONFIDENCE -m o -a 1 -d 0 >classifier.o.wine.ac.out
./run.pl wine.ac >>Print
