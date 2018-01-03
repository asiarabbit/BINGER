#!/bin/bash
#generate simulation data
file=gSIM.root
isDCArrR=1
sim(){
echo "To genearte simualtion data"
sleep 3
./sim 1 100 SIM0.root &
sleep 2; ./sim 1 100 SIM0.root &
sleep 4; ./sim 1 100 SIM2.root &
sleep 6; ./sim 1 100 SIM3.root &
wait
./sim 1 100 SIM4.root &
sleep 2; ./sim 1 100 SIM5.root &
sleep 4; ./sim 1 100 SIM6.root &
sleep 6; ./sim 1 100 SIM7.root &
wait
}
# particle tracking
pre(){
echo "To analyze data"
sleep 3
./pre -d5 SIM0.root &
sleep 5; ./pre -d5 SIM1.root &
sleep 4; ./pre -d5 SIM2.root &
sleep 6; ./pre -d5 SIM3.root &
wait
./pre -d5 SIM4.root &
sleep 2; ./pre -d5 SIM5.root &
sleep 4; ./pre -d5 SIM6.root &
sleep 6; ./pre -d5 SIM7.root &
wait
if [ -f $file ]; then
    rm $file
fi
hadd $file SIM[0-5].root
}

# main action begins here
make

if [ -f 'SIM0.root' ]; then
	rm SIM[0-7].root
fi

sim
wait;
echo "sim data have been generated"
sleep 3
pre
./t0 $file $isDCArrR
#for i in $(seq 0 3); do
#    ./ass $file $isDCArrR $i
#    ./str $file $isDCArrR $i
#    pre
#done
#echo "Accomplished"
