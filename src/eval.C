/**
	\file assess.C
	\example assess.C
	\brief assess the data analysis result
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2017/12/1 Last revised: 2018/3/3, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/

// assess.C -- assess the data analysis result
// Created: 2017/12/1, Lasted modified: 2018/3/3, Author: SUN Yazhou
#include <iostream>
#include <cstdlib>
#include "TAEventProcessor.h"
#include "TASimulation.h"

using std::cout;
using std::endl;

int main(int argc, char *argv[]){
	if(argc <= 1){
		cout << "./eval <rootfile>; rootfile name have to be given\n";
		exit(1);
	}
	TAEventProcessor *ep = TAEventProcessor::Instance();
	ep->Configure();
	TASimulation::Evaluate(argv[1]);
	return 0;
}

