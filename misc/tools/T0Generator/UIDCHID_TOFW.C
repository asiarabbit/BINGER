#include <iostream>
#include <cstdlib>
#include <fstream>

using namespace std;

typedef int tChId[4];

int chIdRFrag[4] = {   1,  101,  201,  301}; // TOF strip 0-7, 8-15, 16-23, 24-29 dcArrR
int chIdLFrag[4] = {3301, 3401, 3501, 3601}; // TOF strip 1-8, 9-16, 17-24, 25-30 dcArrL

//				DCArr[L-R]	TOFWall@@@	[strip#]	[Channel#]
int type[4] = {      3,         3,         0,           0};
unsigned uid = 0;

void Fragment(int chId0, int *type, int nStrip, ofstream *pfout){
	ofstream &fr = *pfout;
	for(int i = 0; i < nStrip; i++){
	    if(type[2] >= 30 || type[1] >= 4 || (type[0] != 3 && type[0] != 4)){
	        cout << "invalid type value.\n"; getchar();
	        exit(EXIT_FAILURE);
	    }
	    for(int j = 0; j < 4; j++) if(type[j] < 0){
	        cout << "invalid type value.\n"; getchar();
	        exit(EXIT_FAILURE);
	    }

		for(int j = 0; j < 4; j++){
			if(type[3] >= 4){
			    cout << "invalid type value.\n"; getchar();
			    exit(EXIT_FAILURE);
			}
			uid = type[0] + (type[1]<<6) + (type[2]<<9) + (type[3]<<15);
			fr << chId0 << "\t" << uid << endl;
			chId0++; type[3]++;
		}
		type[2]++; type[3] = 0;
		fr << endl;
	} // end for over i
	fr << endl << endl;
}

int main(){
	ofstream fout("tofwCHID.txt");
	int nStrip[4] = {8, 8, 8, 6};
	tChId *chId[2] = {&chIdRFrag, &chIdLFrag};
	for(int I = 0; I < 2; I++){
		type[0] = I + 3; // DC Array
		for(int i = 0; i < 4; i++){
			type[2] = i * 8; // strip0
			type[3] = 0;
			Fragment((*chId[I])[i], type, nStrip[i], &fout);
		}
		fout << "#############################################################\n\n\n";
	}
	fout.close();
}
