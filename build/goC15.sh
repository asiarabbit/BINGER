#!/bin/bash
#make pre
#mv pre preC15
#make ass
#make sho

# 5mmC; B: 1.295;

# 50w
./preC15 20180711_1552.dat 20180711.008 -d6
./sho 20180711_1552.dat_0.root

# 50w
./preC15 20180711_2005.dat 20180711.009 -d6
./sho 20180711_2005.dat_0.root

# emptyTA; 40w
./preC15 20180711_2355.dat 20180711.011 -d6
./sho 20180711_2355.dat_0.root

# emptyTA; 55w
./preC15 20180712_0251.dat 20180711.012 -d6
./sho 20180711_0251.dat_0.root


