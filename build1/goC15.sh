#!/bin/bash
#make pre
#mv pre preC15
#make ass
#make sho

# 5mmC; B: 1.295;

# 50w
./preC15 20180711_1552.dat 20180711.008 -d3
./sho 20180711_1552.dat_0.root

# 50w
./preC15 20180711_2005.dat 20180711.009 -d3
./sho 20180711_2005.dat_0.root

if [ -f "C15TA.root" ]; then
	rm C15TA.root
fi
hadd -f C15TA.root 20180711_1552.dat_0.root 20180711_2005.dat_0.root

# emptyTA; 40w
./preC15 20180711_2355.dat 20180711.011 -d3
./sho 20180711_2355.dat_0.root

# emptyTA; 55w, events after 486800 are spoiled (PDC HV down after 06:34)
./preC15 20180712_0251.dat 20180711.012 -d3 -f486800
./sho 20180712_0251.dat_0.root

if [ -f "C15NOTA.root" ]; then
	rm C15NOTA.root
fi
hadd C15NOTA.root 20180711_2355.dat_0.root 20180712_0251.dat_0.root
