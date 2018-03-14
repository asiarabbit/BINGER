#!/bin/bash
#generate simulation data
file=gSIM.root
isDCArrR=1
sim(){
echo "To genearte simualtion data"
if [ -f 'SIM0.root' ]; then
	rm SIM[0-7].root
fi
if [ -f $file ]; then
	rm $file
fi
./sim 1 100 SIM0.root &
sleep 3; ./sim 1 100 SIM1.root &
sleep 6; ./sim 1 100 SIM2.root &
sleep 9; ./sim 1 100 SIM3.root &
wait
./sim 1 100 SIM4.root &
sleep 3; ./sim 1 100 SIM5.root &
sleep 6; ./sim 1 100 SIM6.root &
sleep 9; ./sim 1 100 SIM7.root &
wait
}
# particle tracking
pre(){
echo "To analyze data"
./pre -d5 SIM0.root &
sleep 3; ./pre -d5 SIM1.root &
sleep 6; ./pre -d5 SIM2.root &
sleep 9; ./pre -d5 SIM3.root &
wait
./pre -d5 SIM4.root &
sleep 3; ./pre -d5 SIM5.root &
sleep 6; ./pre -d5 SIM6.root &
sleep 9; ./pre -d5 SIM7.root &
wait
if [ -f $file ]; then
    rm $file
fi
hadd $file SIM[0-7].root
}

# main action begins here
make -j8

sim
#echo "sim data have been generated"
#exit
pre
./ass $file $isDCArrR 0
./str $file $isDCArrR 1
exit
#./t0 $file $isDCArrR
#######################################################
# calibration process #
for i in $(seq 1 4); do
	echo "STRCor loop $i, DCArr Option: $isDCArrR"
    ./str $file $isDCArrR $i
    pre
    ./ass $file $isDCArrR $i
done
echo "Accomplished"
