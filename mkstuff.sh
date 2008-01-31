#!/bin/bash

echo autoheader
autoheader

echo aclocal
aclocal

echo libtoolize --force
libtoolize --force

echo automake --add-missing
automake --add-missing

echo automake
automake

echo autoconf
autoconf

### configure @ ATI (hydrus) ###
ROOT=/data/users/leandro.costa
echo ./configure --prefix=$ROOT/local/machado

#echo make clean
#make clean

#echo make
#make

#echo make install
#make install
