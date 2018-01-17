#!/bin/bash
rawfile=20171030_1608.dat
file=${rawfile}".root"
#hadd $file ${rawfile}"_[0-5].root"
go(){
rootfile=${rawfile}"_[0-5].root"
#ls $rootfile
rootfile=${rawfile}"_[0-5].root"
#hadd $file $rootfile
#if [ -f ${rawfile}"_0.root" ]; then
#    rm $rootfile
#t
}
go
