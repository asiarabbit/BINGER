#!/bin/bash
make sim && ./sim 2 97
make pre && ./pre -r SIM.root -d5
make eval && ./eval SIM.root
