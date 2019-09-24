#!/bin/bash
#make pre
#mv pre preO18
#make ass
#make sho

# 5mmC; B: 1.20

#36w; B: 1.2004 Tesla
./preO18 20180707_2052.dat 20180707.021 -d6
./sho 20180707_2052.dat_0.root
#mv 20180707_2052.dat_0.root O18TA.root

# 29w; B: 1.2336 Tesla
#./preO18NOTA 20180707_1649.dat 20180707.011 -d6
#./sho 2018707_1649.dat_0.root
#mv 2018707_1649.dat_0.root O18NOTA.root

