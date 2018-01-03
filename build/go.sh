#!/bin/bash
#generate simulation data
file=gSIM.root
isDCArrR=1
sim(){
./sim 1 100 SIM0.root
./sim 1 100 SIM1.root
./sim 1 100 SIM2.root
./sim 1 100 SIM3.root
./sim 1 100 SIM4.root
./sim 1 100 SIM5.root
./sim 1 100 SIM6.root
./sim 1 100 SIM7.root
}
# particle tracking
pre(){
./pre -d5 SIM0.root
./pre -d5 SIM1.root
./pre -d5 SIM2.root
./pre -d5 SIM3.root
./pre -d5 SIM4.root
./pre -d5 SIM5.root
./pre -d5 SIM6.root
./pre -d5 SIM7.root
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
pre
./t0 $file $isDCArrR
for i in $(seq 0 3); do
    ./ass $file $isDCArrR $i
    ./str $file $isDCArrR $i
    pre
done
echo "Accomplished"
