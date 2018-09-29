#!/bin/bash
make pre
make ass
make sho
./pre 20180708_1530.dat 20180708.005 -d6
for i in {1..3}; do ./ass 20180708_1530.dat_0.root $i; done
./sho 20180708_1530.dat_0.root

./pre 20180708_1840.dat 20180708.006 -d6
for i in {1..3}; do ./ass 20180708_1840.dat_0.root $i; done
./sho 20180708_1840.dat_0.root

./pre 20180708_2137.dat 20180708.007 -d6
for i in {1..3}; do ./ass 20180708_2137.dat_0.root $i; done
./sho 20180708_2137.dat_0.root

