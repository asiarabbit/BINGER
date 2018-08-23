// vme.C -- read data collected by VME electronics and link them with PXI data event by event
// Created: 2018/1/10, Lasted modified: 2018/3/3, Author: SUN Yazhou
#include <iostream>
#include <cstdlib>
#include "TAVMEReader.h"

using std::cout;
using std::endl;

int main(int argc, char *argv[]){
	if(argc < 3){
		cout << "./vme <vmedatafile> <ifMatch>.\nvmedatafile name and matchOption have to be provided\n";
		exit(1);
	}
	bool isMatch = bool(atoi(argv[2]));
	if(isMatch && argc < 4){
		cout << "./vme <datafile> <ifMatch> <PXIROOTFile>.\nvmedatafile, matchOption and PXIROOTFile have to be given\n";
		exit(1);
	}

	TAVMEReader *vme = new TAVMEReader(argv[1]);
	vme->ReadVME();
	if(isMatch) vme->Match(argv[3]);
}
