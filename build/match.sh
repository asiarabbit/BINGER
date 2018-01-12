#!/bin/bash
make vme
option=1
if [ $# -ge 1 ]; then
    option=$1
fi
case $option in
    0) ./vme 11232237.001 1 20161123_2247.dat_0.root ;;
    1) ./vme 20161125.001 1 20161125_2031.dat_0.root ;;
    2) ./vme 20161125.002 1 20161125_2046.dat_0.root ;;
    3) ./vme 20161125.004 1 20161125_2348.dat_0.root ;;
    4) ./vme 20161125.006 1 20161126_0030.dat_0.root ;;
    5) ./vme 20161125.008 1 20161126_0252.dat_0.root ;;
    6) ./vme 20161125.009 1 20161126_0522.dat_0.root ;;
esac
root -l test1.C
