#!/bin/bash
../data/pxi.sh $1
../data/vme.sh $2
./pre $1 $2 -d -f50000
