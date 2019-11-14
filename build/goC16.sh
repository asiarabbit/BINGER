#!/bin/bash
#make pre
#mv pre preC16
#make ass
#make sho

# 5mmC; B: 1.45

#19w
./preC16 20180708_0153.dat 20180708.003 -d3
./sho 20180708_0153.dat_0.root

# 29w
./preC16 20180708_1530.dat 20180708.005 -d3
./sho 20180708_1530.dat_0.root

# 32w
./preC16 20180708_1840.dat 20180708.006 -d3
./sho 20180708_1840.dat_0.root

hadd -f C16TA.root 20180708_0153.dat_0.root 20180708_1530.dat_0.root 20180708_1840.dat_0.root

# empty TA; 40w
./preC16 20180708_2137.dat 20180708.007 -d3
./sho 20180708_2137.dat_0.root

mv 20180708_2137.dat_0.root C16NOTA.root

