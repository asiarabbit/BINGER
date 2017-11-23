#!/bin/bash
myfile="go"
if [ -a "$myfile" ]; then
    make clean
fi
make
./go
