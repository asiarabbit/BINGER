#!/bin/bash
make -j8 pre
rm 20180708_0153.dat_1.root
./pre 20180708_0153.dat 20180708.003 -d3 -u1 -f200
root -l 20180708_0153.dat_1.root
