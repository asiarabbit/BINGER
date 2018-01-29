#!/bin/bash
#generate simulation data
rawfile=20171030_1608.dat
file=${rawfile}".root"
nev=967854
dn=`expr $nev / 6`
# particle tracking
pre(){
echo "To analyze data"
sleep 0; ./pre -d5 $rawfile -i`expr $dn \* 0` -f`expr $dn \* 1` -u0 &
sleep 3; ./pre -d5 $rawfile -i`expr $dn \* 1` -f`expr $dn \* 2` -u1 &
sleep 6; ./pre -d5 $rawfile -i`expr $dn \* 2` -f`expr $dn \* 3` -u2 &
wait
sleep 3; ./pre -d5 $rawfile -i`expr $dn \* 3` -f`expr $dn \* 4` -u3 &
sleep 6; ./pre -d5 $rawfile -i`expr $dn \* 4` -f`expr $dn \* 5` -u4 &
sleep 9; ./pre -d5 $rawfile -i`expr $dn \* 5` -f`expr $dn \* 7` -u5 &
wait
if [ -f $file ]; then
    rm $file
fi
hadd $file ${rawfile}"_[0-5].root"
#if [ -f $rawfile"_[0-5].root" ]; then
#    rm $rawfile"_[0-5].root"
#fi
}


############################################################################
############################################################################
############################################################################
# main action begins here
make -j8
############# TRACKING ################################
pre
#exit
############# T0 Calibration ##########################
./t0 $file 0
pre
./ass $file 0 0
./ass $file 1 0
#######################################################


############# CALIBRATION #############################
for i in $(seq 1 4); do
	echo "_____ STRCor loop $i, DCArr Option: $isDCArrR _______"
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






