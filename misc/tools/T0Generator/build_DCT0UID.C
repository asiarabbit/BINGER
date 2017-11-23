#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>
#include <cctype>
#include <ctime>

using namespace std;
typedef int tChId[3][3][6];

#include "chId0.C" // initialization of chIdL and chIdR

const char *time0(){
	time_t tt = ::time(NULL);
	tm *t = localtime(&tt);
	static char ttt[128];
	sprintf(ttt, "%d/%02d/%02d %02d:%02d", t->tm_year + 1900,
	 t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min);
	return ttt;
}

void cable(const char *path, int *type, int chId, int cableId, char xuv, int dcId, char isL){
	char fname[2][64];
	ofstream *fout[2];
	sprintf(fname[0], "%s/sfe0.000", path);
	sprintf(fname[1], "%s/sfe1.000", path);
//	cout << "fname[0]: " << fname[0] << endl; getchar(); // DEBUG
//	cout << "fname[0]: " << fname[1] << endl; getchar(); // DEBUG
	fout[0] = new ofstream(fname[0], ios::app);
	fout[1] = new ofstream(fname[1], ios::app);
	string s0;
s0+="##################################################################################################\n";
s0+="# Data Analysis Code Project for the External Target Facility, CSR-HIRFL, @IMP\n";
s0+="#\n";
s0+="# Channel Id file for 16 channels of an SFE16 chip used for Ampli-Shape-Discri in MWDC FEE.\n";

	string s2;	
s2+="# channelId map rule: PXI channel id 1 -> 16 <=> SFE16 channel No. 16 -> 1\n";
s2+="# SFE16 channel No. 1 -> 16: from left to right (facing beam direction, x incrementing).\n";
s2+="# SFE0 cooresponds to X(U, V)1 with smaller z than SFE1(X(U, V)2),\n";
s2+="# and is 4-mm x(u, v)-wise translated than X(U, V)2.\n";
s2+="#\n";
s2+="# File format is as follows:\n";
s2+="# channelId	uid\n";
s2+="#\n";
char ts0[128]; sprintf(ts0, "# Created Time: %s\n", time0());
s2+=ts0;
s2+="# Author: SUN Yazhou, asia.rabbit@163.com\n";
sprintf(ts0, "# Last Modified: %s, SUN Yazhou\n", time0());
s2+=ts0;
s2+="##################################################################################################\n";
//	cout << "time0: " << time0() << endl; getchar(); // DEBUG

	for(int I = 2; I--;){
		ofstream &fr = *fout[I];
		// add file header
		char s1[128];
		sprintf(s1, "# Identity: __________ ETF -> MWDCArray%c -> MWDC%d -> %c -> Cable%d -> SFE%d(%c%d) __________\n", isL, dcId, xuv, cableId, I, xuv, I+1);
		fr << s0 << s1 << s2 << endl;

		for(int i = 16; i--;){
		    if(type[5] >= 16 || type[4] >= 2 || type[3] >= 6 ||
		     type[2] >= 3 || type[1] >= 3 ||
		     (type[0] != 3 && type[0] != 4)){
		        cout << "invalid type value.\n"; getchar();
		        exit(EXIT_FAILURE);
		    }
		    for(int j = 6; j--;) if(type[j] < 0){
		        cout << "invalid type value.\n"; getchar();
		        exit(EXIT_FAILURE);
		    }

			int uid = type[0] + (type[1]<<6) + (type[2]<<9)
				+ (type[3]<<11) + (type[4]<<14) + (type[5]<<15);
		    fr << chId << "\t" << uid << endl;
		    chId++; type[5]--;
		} // end for over i
		type[5] = 15; type[4] = 0;
		fr.close();
	} // end for over I
}


// main function to generete the chid config-dir tree
int main(){
	char isL[3] = "LR", xuv[4] = "XUV";
	int len[2][3] = {{5, 5, 6}, {5, 5, 5}};
	tChId *chId[2] = {&chIdL, &chIdR};
	string mkdir = "mkdir "; char path0[128] = "channelId";
	system((mkdir+path0).c_str());
	int type[6];
	for(int i = 0; i < 2; i++){
		type[0] = i + 3;
		char path1[128]; sprintf(path1, "/mwdcArr%c", isL[i]);
		system((mkdir+path0+path1).c_str());
		for(int j = 0; j < 3; j++){
			type[1] = j;
			char path2[128]; sprintf(path2, "/mwdc%d", j);
			system((mkdir+path0+path1+path2).c_str());
			for(int k = 0; k < 3; k++){
				type[2] = k;
				char path3[128]; sprintf(path3, "/%c", tolower(xuv[k]));
				system((mkdir+path0+path1+path2+path3).c_str());
				for(int l = 0; l < len[i][j]; l++){
					type[3] = l;  type[4] = 1; type[5] = 15;
					char path4[128]; sprintf(path4, "/cable%d", l);
					char path[128];
					sprintf(path, "%s%s%s%s%s", path0, path1, path2, path3, path4);
					system((mkdir+path).c_str());
//					cout << "chId: " << (*chId[i])[j][k][l] << endl; // DEBUG
	//				cout << "path: " << path << endl; getchar(); getchar(); // DEBUG
					cable(path, type, (*chId[i])[j][k][l], l, xuv[k], j, isL[i]);
				}
			} // end for over X-U-V
		} // end for over dc
	} // end for over L - R
}




