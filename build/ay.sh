#!/bin/bash
file=20180708_0153.dat_2.root
index0=$1
index1=$(($index0+1))
if [ -f $file ]; then
    rm $file
fi
make pre -j8 && ./pre 20180708_0153.dat 20180708.003 -d3 -u2 -i$index0 -f$index1
root -l see.C
