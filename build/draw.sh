#!/bin/bash
n0=$1
let n1=n0+1

if [ -e 20180708_0153.dat_7.root ]; then
    rm 20180708_0153.dat_7.root
fi

make pre
./pre 20180708_0153.dat 20180708.003 -d3 -i$n0 -f$n1 -p$n0 -u7
root -l draw.C

