#!/bin/bash
vmerootfile=20180606.069_0.root
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
#pre
./ass $file 0 0
./ass $file 1 0

############# CALIBRATION #############################
for i in $(seq 1 2); do
	echo "_____ STRCor loop $i, DCArr Option: $isDCArrD _______"
    ./strpdc $file 0 $i
    ./strpdc $file 1 $i
    pre
    ./ass $file 0 $i
    ./ass $file 1 $i
done
echo "Accomplished"
############################################################################
############################################################################
############################################################################






