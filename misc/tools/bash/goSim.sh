#!/bin/bash
make sim && ./sim 1 100 SIM0.root
make pre && ./pre -r SIM0.root -d5
#make eval && ./eval SIM.root
#make ass && ./ass SIM.root 0
#make t0 && ./t0 SIM.root
