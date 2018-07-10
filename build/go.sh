#!/bin/bash
rootfile=$1
echo "The ROOT FILE NAME would be $rootfile"
make
./ass $rootfile 1 0 0
./ass $rootfile 0 0 1
./ass $rootfile 1 0 1
./sho $rootfile
./gui $rootfile
