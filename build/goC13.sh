#!/bin/bash
#make pre
#mv pre preC13
#make ass
#make sho

# B: 1.0750, 5mmC

# 21w powerOff in the end. Cheng's Det was in
./preC13 20180713_2001.dat 20180713.010 -d3
./sho 20180713_2001.dat_0.root

############### DC1 FEE configuration failure && MU Gain too large ##########
# 41w
./preC13 20180714_0040.dat 20180713.013 -d3
./sho 20180714_0040.dat_0.root

# 22w
./preC13 20180714_0425.dat 20180713.014 -d3
./sho 20180714_0425.dat_0.root

# empty TA 36w
./preC13 20180714_0613.dat 20180714.001 -d3
./sho 20180714_0613.dat_0.root
############### DC1 FEE configuration failure && MU Gain too large ##########


