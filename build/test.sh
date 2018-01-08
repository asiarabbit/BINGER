#!/bin/bash
make ass
./ass mergeWithT0.root 1 0
root -l test.C
