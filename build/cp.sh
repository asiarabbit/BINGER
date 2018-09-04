#!/bin/bash
while [ $# -ne 0 ]
do
    cp $1 /run/media/asia/GSP1RMCULXF/20180830/
    shift
done
umount -v /dev/sdb4
