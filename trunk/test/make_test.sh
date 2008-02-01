#!/bin/bash

#SUPPORT=0.1
#CONFIDENCE=0.9
#SUPPORT=0.1
#CONFIDENCE=0.1
#SUPPORT=0.9
#CONFIDENCE=0.1
SUPPORT=$1
CONFIDENCE=$1

MIN_RULE_LEN=1
MAX_RULE_LEN=2

###

TEST_FILE=test_s$SUPPORT.c$CONFIDENCE.m$MIN_RULE_LEN.a$MAX_RULE_LEN.out

CLASSIFIER=classifier
APP_CLASSIFIER=../src/$CLASSIFIER

LAZY=lazy
APP_LAZY=../../testing_10_per_cent/$LAZY

APP_COMPARE=./compare.pl

TIC_TAC_BASE=tic-tac.ac
TIC_TAC_TRAINING=../data/adrianov/$TIC_TAC_BASE.training
TIC_TAC_TESTING=../data/adrianov/$TIC_TAC_BASE.testing

TIC_TAC_BASE_CLASS=../data/adrianov/$TIC_TAC_BASE.out
TIC_TAC_LAZY_CLASS=$LAZY.$TIC_TAC_BASE.out
TIC_TAC_C_CLASS=$CLASSIFIER.c.$TIC_TAC_BASE.out
TIC_TAC_O_CLASS=$CLASSIFIER.o.$TIC_TAC_BASE.out

LYMPH_BASE=lymph.ac
LYMPH_TRAINING=../data/adrianov/$LYMPH_BASE.training
LYMPH_TESTING=../data/adrianov/$LYMPH_BASE.testing

LYMPH_BASE_CLASS=../data/adrianov/$LYMPH_BASE.out
LYMPH_LAZY_CLASS=$LAZY.$LYMPH_BASE.out
LYMPH_C_CLASS=$CLASSIFIER.c.$LYMPH_BASE.out
LYMPH_O_CLASS=$CLASSIFIER.o.$LYMPH_BASE.out

AUSTRA_BASE=austra.ac
AUSTRA_TRAINING=../data/adrianov/$AUSTRA_BASE.training
AUSTRA_TESTING=../data/adrianov/$AUSTRA_BASE.testing

AUSTRA_BASE_CLASS=../data/adrianov/$AUSTRA_BASE.out
AUSTRA_LAZY_CLASS=$LAZY.$AUSTRA_BASE.out
AUSTRA_C_CLASS=$CLASSIFIER.c.$AUSTRA_BASE.out
AUSTRA_O_CLASS=$CLASSIFIER.o.$AUSTRA_BASE.out

BREAST_BASE=breast.ac
BREAST_TRAINING=../data/adrianov/$BREAST_BASE.training
BREAST_TESTING=../data/adrianov/$BREAST_BASE.testing

BREAST_BASE_CLASS=../data/adrianov/$BREAST_BASE.out
BREAST_LAZY_CLASS=$LAZY.$BREAST_BASE.out
BREAST_C_CLASS=$CLASSIFIER.c.$BREAST_BASE.out
BREAST_O_CLASS=$CLASSIFIER.o.$BREAST_BASE.out

IRIS_BASE=iris.ac
IRIS_TRAINING=../data/adrianov/$IRIS_BASE.training
IRIS_TESTING=../data/adrianov/$IRIS_BASE.testing

IRIS_BASE_CLASS=../data/adrianov/$IRIS_BASE.out
IRIS_LAZY_CLASS=$LAZY.$IRIS_BASE.out
IRIS_C_CLASS=$CLASSIFIER.c.$IRIS_BASE.out
IRIS_O_CLASS=$CLASSIFIER.o.$IRIS_BASE.out

LABOR_BASE=labor.ac
LABOR_TRAINING=../data/adrianov/$LABOR_BASE.training
LABOR_TESTING=../data/adrianov/$LABOR_BASE.testing

LABOR_BASE_CLASS=../data/adrianov/$LABOR_BASE.out
LABOR_LAZY_CLASS=$LAZY.$LABOR_BASE.out
LABOR_C_CLASS=$CLASSIFIER.c.$LABOR_BASE.out
LABOR_O_CLASS=$CLASSIFIER.o.$LABOR_BASE.out

WINE_BASE=wine.ac
WINE_TRAINING=../data/adrianov/$WINE_BASE.training
WINE_TESTING=../data/adrianov/$WINE_BASE.testing

WINE_BASE_CLASS=../data/adrianov/$WINE_BASE.out
WINE_LAZY_CLASS=$LAZY.$WINE_BASE.out
WINE_C_CLASS=$CLASSIFIER.c.$WINE_BASE.out
WINE_O_CLASS=$CLASSIFIER.o.$WINE_BASE.out

rm -rf $TEST_FILE

$APP_LAZY -i $TIC_TAC_TRAINING -t $TIC_TAC_TESTING -s 1 -c $CONFIDENCE -m 10 >$TIC_TAC_LAZY_CLASS
$APP_CLASSIFIER -i $TIC_TAC_TRAINING -t $TIC_TAC_TESTING -s $SUPPORT -c $CONFIDENCE -m $MIN_RULE_LEN -a $MAX_RULE_LEN -r c -d -1 >$TIC_TAC_C_CLASS
$APP_CLASSIFIER -i $TIC_TAC_TRAINING -t $TIC_TAC_TESTING -s $SUPPORT -c $CONFIDENCE -m $MIN_RULE_LEN -a $MAX_RULE_LEN -r o -d -1 >$TIC_TAC_O_CLASS
$APP_COMPARE $TIC_TAC_BASE $TIC_TAC_BASE_CLASS $TIC_TAC_LAZY_CLASS $TIC_TAC_C_CLASS $TIC_TAC_O_CLASS >>$TEST_FILE

$APP_LAZY -i $LYMPH_TRAINING -t $LYMPH_TESTING -s 1 -c $CONFIDENCE -m 10 >$LYMPH_LAZY_CLASS
$APP_CLASSIFIER -i $LYMPH_TRAINING -t $LYMPH_TESTING -s $SUPPORT -c $CONFIDENCE -m $MIN_RULE_LEN -a $MAX_RULE_LEN -r c -d -1 >$LYMPH_C_CLASS
$APP_CLASSIFIER -i $LYMPH_TRAINING -t $LYMPH_TESTING -s $SUPPORT -c $CONFIDENCE -m $MIN_RULE_LEN -a $MAX_RULE_LEN -r o -d -1 >$LYMPH_O_CLASS
$APP_COMPARE $LYMPH_BASE $LYMPH_BASE_CLASS $LYMPH_LAZY_CLASS $LYMPH_C_CLASS $LYMPH_O_CLASS >>$TEST_FILE

# (BETTER - SUPPORT 0.1, CONFIDENCE 0.1)
$APP_LAZY -i $AUSTRA_TRAINING -t $AUSTRA_TESTING -s 1 -c $CONFIDENCE -m 10 >$AUSTRA_LAZY_CLASS
$APP_CLASSIFIER -i $AUSTRA_TRAINING -t $AUSTRA_TESTING -s $SUPPORT -c $CONFIDENCE -m $MIN_RULE_LEN -a $MAX_RULE_LEN -r c -d -1 >$AUSTRA_C_CLASS
$APP_CLASSIFIER -i $AUSTRA_TRAINING -t $AUSTRA_TESTING -s $SUPPORT -c $CONFIDENCE -m $MIN_RULE_LEN -a $MAX_RULE_LEN -r o -d -1 >$AUSTRA_O_CLASS
$APP_COMPARE $AUSTRA_BASE $AUSTRA_BASE_CLASS $AUSTRA_LAZY_CLASS $AUSTRA_C_CLASS $AUSTRA_O_CLASS >>$TEST_FILE

$APP_LAZY -i $BREAST_TRAINING -t $BREAST_TESTING -s 1 -c $CONFIDENCE -m 10 >$BREAST_LAZY_CLASS
$APP_CLASSIFIER -i $BREAST_TRAINING -t $BREAST_TESTING -s $SUPPORT -c $CONFIDENCE -m $MIN_RULE_LEN -a $MAX_RULE_LEN -r c -d -1 >$BREAST_C_CLASS
$APP_CLASSIFIER -i $BREAST_TRAINING -t $BREAST_TESTING -s $SUPPORT -c $CONFIDENCE -m $MIN_RULE_LEN -a $MAX_RULE_LEN -r o -d -1 >$BREAST_O_CLASS
$APP_COMPARE $BREAST_BASE $BREAST_BASE_CLASS $BREAST_LAZY_CLASS $BREAST_C_CLASS $BREAST_O_CLASS >>$TEST_FILE

$APP_LAZY -i $IRIS_TRAINING -t $IRIS_TESTING -s 1 -c $CONFIDENCE -m 10 >$IRIS_LAZY_CLASS
$APP_CLASSIFIER -i $IRIS_TRAINING -t $IRIS_TESTING -s $SUPPORT -c $CONFIDENCE -m $MIN_RULE_LEN -a $MAX_RULE_LEN -r c -d -1 >$IRIS_C_CLASS
$APP_CLASSIFIER -i $IRIS_TRAINING -t $IRIS_TESTING -s $SUPPORT -c $CONFIDENCE -m $MIN_RULE_LEN -a $MAX_RULE_LEN -r o -d -1 >$IRIS_O_CLASS
$APP_COMPARE $IRIS_BASE $IRIS_BASE_CLASS $IRIS_LAZY_CLASS $IRIS_C_CLASS $IRIS_O_CLASS >>$TEST_FILE

$APP_LAZY -i $LABOR_TRAINING -t $LABOR_TESTING -s 1 -c $CONFIDENCE -m 10 >$LABOR_LAZY_CLASS
$APP_CLASSIFIER -i $LABOR_TRAINING -t $LABOR_TESTING -s $SUPPORT -c $CONFIDENCE -m $MIN_RULE_LEN -a $MAX_RULE_LEN -r c -d -1 >$LABOR_C_CLASS
$APP_CLASSIFIER -i $LABOR_TRAINING -t $LABOR_TESTING -s $SUPPORT -c $CONFIDENCE -m $MIN_RULE_LEN -a $MAX_RULE_LEN -r o -d -1 >$LABOR_O_CLASS
$APP_COMPARE $LABOR_BASE $LABOR_BASE_CLASS $LABOR_LAZY_CLASS $LABOR_C_CLASS $LABOR_O_CLASS >>$TEST_FILE

$APP_LAZY -i $WINE_TRAINING -t $WINE_TESTING -s 1 -c $CONFIDENCE -m 10 >$WINE_LAZY_CLASS
$APP_CLASSIFIER -i $WINE_TRAINING -t $WINE_TESTING -s $SUPPORT -c $CONFIDENCE -m $MIN_RULE_LEN -a $MAX_RULE_LEN -r c -d -1 >$WINE_C_CLASS
$APP_CLASSIFIER -i $WINE_TRAINING -t $WINE_TESTING -s $SUPPORT -c $CONFIDENCE -m $MIN_RULE_LEN -a $MAX_RULE_LEN -r o -d -1 >$WINE_O_CLASS
$APP_COMPARE $WINE_BASE $WINE_BASE_CLASS $WINE_LAZY_CLASS $WINE_C_CLASS $WINE_O_CLASS >>$TEST_FILE
