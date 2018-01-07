#!/bin/bash
#generate simulation data
isDCArrR=1
file=merge.root
rawfile0=20161125_2031.dat
rawfile1=20161125_2348.dat
rawfile2=20161126_0030.dat
rawfile3=20161126_0252.dat
rawfile4=20161126_0522.dat
rawfile5=20161125_2046.dat

# particle tracking
pre(){
echo "To analyze data"
./pre -d5 $rawfile0 &
sleep 3; ./pre -d5 $rawfile1 &
sleep 6; ./pre -d5 $rawfile2 &
wait
./pre -d5 $rawfile3 &
sleep 3; ./pre -d5 $rawfile4 &
sleep 6; ./pre -d5 $rawfile5 &
wait
if [ -f $file ]; then
    rm $file
fi
hadd $file 2016112*.root
}

# main action begins here
make -j8

############# TRACKING ################################
pre
mv $file mergeNoT0.root
./t0 mergeNoT0.root 1
./ass mergeNoT0.root $isDCArrR -1

pre
cp $file mergeWithT0.root
./t0 mergeWithT0.root 1
./ass $file $isDCArrR 0
#######################################################


############# CALIBRATION #############################
#./t0 $file $isDCArrR
#pre
./ass $file $isDCArrR 0
for i in $(seq 1 4); do
	echo "__________________________STRCor loop $i, DCArr Option: $isDCArrR_______________________________"
    ./str $file $isDCArrR $i
    pre
    ./ass $file $isDCArrR $i
done
echo "Accomplished"
