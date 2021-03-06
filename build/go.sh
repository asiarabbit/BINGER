#!/bin/bash
file="mergeC12.root"
# particle tracking
pre(){
./pre $file -d
}


############################################################################
############################################################################
############################################################################
# main action begins here
make -j8
############# TRACKING ################################
#./pre $file -d
#./ass $file 0 -1
#./ass $file 1 -1
#exit
############# T0 Calibration ##########################
#./t0 $file 0
#./t0 $file 1
#./pre $file -d
./pre 20180713.001 20180713_0209.dat -d
./pre 20180713.002 20180713_0508.dat -d
hadd $file 20180713_0508.dat_0.root 20180713_0209.dat_0.root
#./ass $file 0 $i
./ass $file 1 0
./ass $file 2 0
./ass $file 3 0
#######################################################


############# CALIBRATION #############################
for i in $(seq 1 4); do
	echo "_____ STRCor loop $i, DCArr Option: $isDCArrR _______"
    ./strpdc $file 0 $i
    ./strpdc $file 1 $i
    ./str $file 1 $i
    pre
#    ./ass $file 0 $i
    ./ass $file 1 $i
    ./ass $file 2 $i
    ./ass $file 3 $i
done
./pre $file -d3
echo "Accomplished"
############################################################################
############################################################################
############################################################################






