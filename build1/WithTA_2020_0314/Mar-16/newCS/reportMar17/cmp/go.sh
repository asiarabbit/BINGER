#!/bin/bash

if [ -e sigmaIC.root ]; then
    rm sigmaIC.root
fi

root -l -q report1111/sigmaIC.C
root -l -q Mar17_2020/sigmaIC.C
hadd sigmaIC.root sigmaIC_0.root sigmaIC_1.root

root -l openSigmaIC.C

