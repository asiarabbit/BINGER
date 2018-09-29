#!/bin/bash
make pre1
make ass
make sho
#./pre1 20180713_0209.dat 20180713.001 -d6
#for i in {1..3}; do ./ass 20180713_0209.dat_0.root $i; done
#./sho 20180713_0209.dat_0.root

#./pre1 20180713_0508.dat 20180713.002 -d6
#for i in {1..3}; do ./ass 20180713_0508.dat_0.root $i; done
#./sho 20180713_0508.dat_0.root

./pre1 20180713_0920.dat 20180713.003 -d6
for i in {1..3}; do ./ass 20180713_0920.dat_0.root $i; done
./sho 20180713_0920.dat_0.root

# helium gas chamber plugged into the mag cavity
./pre1 20180713_1336.dat 20180713.005 -d6
for i in {1..3}; do ./ass 20180713_1336.dat_0.root $i; done
./sho 20180713_1336.dat_0.root

./pre1 20180713_0000.dat 20180712.008 -d6
for i in {1..3}; do ./ass 20180713_0000.dat_0.root $i; done
./sho 20180713_0000.dat_0.root

