#!/bin/bash
rm ./assessdaq.052_0.root 
make ass 
./ass ./daq.052_0.root 1 1
root -l tmp1.C
