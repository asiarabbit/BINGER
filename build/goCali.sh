#!/bin/bash
vmerootfile=20180606_0826.dat_0.root
file=$vmerootfile
assfile="assess"${file}

pre(){
echo "To analyze data"
./pre -d $file
}


############################################################
############# BEGIN BEGIN BEGIN BEGIN ######################

make
if [ -f $assfile ]; then
    rm $assfile
fi
pre
./ass $file 0 0
./ass $file 1 0

#./t0 $file 0
./t0 $file 1
############# CALIBRATION #############################
for i in $(seq 1 4); do
	echo "_____ STRCor loop $i, DCArr Option: $isDCArrD _______"
    ./str $file 0 $i
    ./str $file 1 $i
    pre
    ./ass $file 0 $i
    ./ass $file 1 $i
done
echo "Accomplished"
############################################################################
############################################################################
############################################################################






