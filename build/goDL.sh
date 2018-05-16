#!/bin/bash

./sim 1 100 SIMDLNF10w0.root 0.00 ; time ./pre SIMDLNF10w0.root -d
./sim 1 100 SIMDLNF10w1.root 0.05 ; time ./pre SIMDLNF10w1.root -d
./sim 1 100 SIMDLNF10w2.root 0.10 ; time ./pre SIMDLNF10w2.root -d
./sim 1 100 SIMDLNF10w3.root 0.15 ; time ./pre SIMDLNF10w3.root -d
./sim 1 100 SIMDLNF10w4.root 0.20 ; time ./pre SIMDLNF10w4.root -d
./sim 1 100 SIMDLNF10w5.root 0.25 ; time ./pre SIMDLNF10w5.root -d
./sim 1 100 SIMDLNF10w6.root 0.30 ; time ./pre SIMDLNF10w6.root -d

