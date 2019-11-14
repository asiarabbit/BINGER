#!/bin/bash
#make pre
#mv pre pre
#make ass
#make sho

# 5mmC; B: 0.9535

# 26w
./pre 20180713_0000.dat 20180712.008 -d3 -u0
./sho 20180713_0000.dat_0.root

# 43w
./pre 20180713_0209.dat 20180713.001 -d3 -u0
./sho 20180713_0209.dat_0.root

# 68w
./pre 20180713_0508.dat 20180713.002 -d3 -u0
./sho 20180713_0508.dat_0.root

# helium gas chamber plugged into the mag cavity, withTa, 29w
./pre 20180713_1336.dat 20180713.005 -d3 -u0
./sho 20180713_1336.dat_0.root

hadd -f C12TA.root 20180713_0000.dat_0.root 20180713_0209.dat_0.root 20180713_0508.dat_0.root 20180713_1336.dat_0.root

# emtpy TA; 67w
./pre 20180713_0920.dat 20180713.003 -d3 -u0
./sho 20180713_0920.dat_0.root

mv 20180713_0920.dat_0.root C12NOTA.root


