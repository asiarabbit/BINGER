#!/bin/bash
rm 20180708_0153.dat_8.root
make -j8 pre
gdb pre -x a.b
