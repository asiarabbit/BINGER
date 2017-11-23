#include <iostream>
#include <cstdlib>
#include <fstream>

using namespace std;

typedef int tChId[3];

//				SiPMArray	 StripId
//int type[2] = {      3,         0};
unsigned uid = 0;

void Fragment(int chId0, ofstream *pfout){
	ofstream &fr = *pfout;
	int type[2]{};
	type[1] = 0; // strip Id

	for(int i = 0; i < 10; i++){
		type[0] = 2;
//		cout << "serialId: " << serialId << endl; getchar(); // DEBUG
		
	    if(type[1] >= 10){
	        cout << "1, invalid type value.\n"; getchar();
	        exit(EXIT_FAILURE);
	    }
	    for(int j = 0; j < 2; j++) if(type[j] < 0){
	        cout << "2, invalid type value.\n"; getchar();
	        exit(EXIT_FAILURE);
	    }

		uid = type[0] + ((type[1]*2)<<6);
		fr << chId0 << "\t" << uid << endl;
		chId0++;
		uid = type[0] + ((type[1]*2+1)<<6);
		fr << chId0 << "\t" << uid << endl;
		chId0++;
		type[1]++;
		fr << endl;
	} // end for over i
	fr << endl << endl;
}

int main(){
	ofstream fout("SiPMArray.txt");
	Fragment(2901, &fout);
	fout.close();
}











