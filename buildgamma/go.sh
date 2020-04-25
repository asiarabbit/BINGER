#!/bin/bash

rootfile=./20200410_0310.dat_0.root;
if [ -e $rootfile ]; then
    rm $rootfile
fi

make pre
gdb pre -x a.b
