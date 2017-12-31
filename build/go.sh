#!/bin/bash

make

#./sim0.sh;./sim1.sh;./sim2.sh;./sim3.sh
#./sim4.sh;./sim5.sh;./sim6.sh;./sim7.sh
hadd gSIM.root SIM*.root
./pre -d5 gSIM.root

