#!/bin/bash
vmerootfile=20180606.069_0.root
file=$vmerootfile

pre(){
echo "To analyze data"
./pre -d5 $file
}


############################################################
############# BEGIN BEGIN BEGIN BEGIN ######################

make
pre
./ass $file 1 0

############# CALIBRATION #############################
for i in $(seq 1 2); do
	echo "_____ STRCor loop $i, DCArr Option: $isDCArrD _______"
    ./str_pdc $file 0 $i
    ./str_pdc $file 1 $i
    pre
    ./ass $file 0 $i
    ./ass $file 1 $i
done
echo "Accomplished"
############################################################################
############################################################################
############################################################################






