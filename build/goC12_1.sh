#!/bin/bash
#make pre
#mv pre preC12
#make ass
#make sho

# 5mmC; B: 0.9535

make pre; make sho;

# 26w
#cp 20180713_0000.dat_0.root 20180713_0000.dat_1.root
./pre 20180713_0000.dat_1.root -d3
./sho 20180713_0000.dat_1.root

# 43w
#cp 20180713_0209.dat_0.root 20180713_0209.dat_1.root
./pre 20180713_0209.dat_1.root -d3
./sho 20180713_0209.dat_1.root

# 68w
#cp 20180713_0508.dat_0.root 20180713_0508.dat_1.root
./pre 20180713_0508.dat_1.root -d3
./sho 20180713_0508.dat_1.root

hadd C12TA1.root 20180713_0000.dat_1.root 20180713_0209.dat_1.root 20180713_0508.dat_1.root

/home/asia/pionExp2017/build/PID/C12/16O/WithTA_1/go.sh

# emtpy TA; 67w
#./preC12 20180713_0920.dat 20180713.003 -d6
#./sho 20180713_0920.dat_0.root

# helium gas chamber plugged into the mag cavity, withTa, 29w
#./preC12 20180713_1336.dat 20180713.005 -d6
#./sho 20180713_1336.dat_0.root


