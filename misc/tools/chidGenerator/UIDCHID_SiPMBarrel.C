#include <iostream>
#include <cstdlib>
#include <fstream>

using namespace std;

typedef int tChId[3];

tChId chIdFrag = {3001, 3101, 3201}; // Barrel strip 1-8, 9-16, 17-24

//				SiPMBarrel	 plateId   [strip#] 	[Channel#]
//int type[4] = {      5,         0,         0,           0};
unsigned uid = 0;

void Fragment(int chId0, ofstream *pfout){
	static short serialId = 0;
	ofstream &fr = *pfout;
	int type[4]{};

	for(int i = 0; i < 8; i++){
		type[0] = 5;
		type[1] = serialId / 3;
		type[2] = serialId % 3;
		type[3] = 0;
//		cout << "serialId: " << serialId << endl; getchar(); // DEBUG
		
	    if(type[1] >= 8 || type[2] >= 3){
	        cout << "1, invalid type value.\n"; getchar();
	        exit(EXIT_FAILURE);
	    }
	    for(int j = 0; j < 4; j++) if(type[j] < 0){
	        cout << "2, invalid type value.\n"; getchar();
	        exit(EXIT_FAILURE);
	    }

		for(int j = 0; j < 4; j++){
			if(type[3] >= 4){
			    cout << "3, invalid type value.\n"; getchar();
			    exit(EXIT_FAILURE);
			}
			uid = type[0] + (type[1]<<6) + (type[2]<<9) + (type[3]<<15);
			fr << chId0 << "\t" << uid << endl;
		    if(chId0 > 0) chId0++;
		    else chId0 = -2;
			type[3]++;
		}

		serialId++;
		fr << endl;
	} // end for over i
	fr << endl << endl;
}

int main(){
	ofstream fout("SiPMBarrel.txt");
	for(int i = 0; i < 3; i++){
		Fragment(chIdFrag[i], &fout);
	}
	fout.close();
}











