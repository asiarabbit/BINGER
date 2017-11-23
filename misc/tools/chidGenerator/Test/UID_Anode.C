#include <iostream>
#include <cstdlib>

using namespace std;


int chId = 5301;
//				DCArr[L-R]	DC[0-1-2]	[X-U-V]		[Cable#]	[SFE16: 0-1@@@]	[anode: 0-15@@@]
int type[6] = {      3,         0,         2,           0,            1,              15};
unsigned uid = 0;

int main(){
	for(int I = 2; I--;){
		for(int i = 16; i--;){
		    if(type[5] >= 16 || type[4] >= 2 || type[3] >= 6 ||
		     type[2] >= 3 || type[1] >= 3 ||
		     (type[0] != 3 && type[0] != 4)){
		        cout << "invalid type value.\n"; getchar();
		        exit(EXIT_FAILURE);
		    }
		    for(int &x : type) if(i < 0){
		        cout << "invalid type value.\n"; getchar();
		        exit(EXIT_FAILURE);
		    }


	        uid = type[0] + (type[1]<<6) + (type[2]<<9)
	    			+ (type[3]<<11) + (type[4]<<14) + (type[5]<<15);
		    cout << chId << "\t" << uid << endl;
		    chId++; type[5]--;
		} // end for over i
		cout << "\n\n\n";
		type[5] = 15; type[4] = 0;
	}
    return 0;
}
