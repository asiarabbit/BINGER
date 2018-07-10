#!/bin/bash
file=$1
if [ $# != 1 ]; then
    echo "USAGE: $0 FILENAME"
    exit 1
fi
if [ ! -f ../data/$file ]; then
    echo $file not found in ../data
    echo downloading $file from 192.168.0.100...
    ../data/vme.sh $file
fi
rootfile=$file"_1.root"
echo "The ROOT FILE NAME would be $rootfile"
make
./pre $file -u1 -d -f5000
./ass $rootfile 1 0 0
./ass $rootfile 0 0 1
./ass $rootfile 1 0 1
./sho $rootfile
./gui $rootfile
