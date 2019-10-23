#!/bin/bash
#make pre
#mv pre preC14
#make ass
#make sho

# B: 1.2006, 5mmC

# 37w
./preC14 20180710_0714.dat 20180710.004 -d3
./sho 20180710_0714.dat_0.root

# 63w
./preC14 20180710_1014.dat 20180710.005 -d3
./sho 20180710_1014.dat_0.root

# 17w
./preC14 20180711_0404.dat 20180711.002 -d3
./sho 20180711_0404.dat_0.root

#18w
./preC14 20180711_0707.dat 20180711.003 -d3
./sho 20180711_0707.dat_0.root

# empty TA 50w
./preC14 20180710_2157.dat 20180710.010 -d3
./sho 20180710_2157.dat_0.root

# empty TA 55w
./preC14 20180711_0039.dat 20180711.001 -d3
./sho 20180711_0039.dat_0.root


