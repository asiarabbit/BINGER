// gui.C -- GUI system for BINGER Data analysis Project.
// Created: 25 Jun., 2018; Last modified: 25 Jun., 2018, Author: SUN Yazhou

#include <iostream>
#include <cstdlib>
#include <string>

#include "TApplication.h"
#include "TGClient.h"
#include "MyMainFrame.h"

using std::cout;
using std::endl;
using std::string;

int main(int argc, char **argv){
	if(argc < 2){
		cout << "usage: ./gui <rootfile> [<assrootfile>]" << endl;
		exit(1);
	}
	string rootfile = argv[1], assrootfile;
	if(argv[2]) assrootfile = argv[2];

	TApplication theApp("App", &argc, argv);
	MyMainFrame *gui = new MyMainFrame(gClient->GetRoot(), 800, 800);
	gui->Initialize(rootfile.c_str(), assrootfile.c_str()); // read the input root file

	theApp.Run();

	return 0;
}
