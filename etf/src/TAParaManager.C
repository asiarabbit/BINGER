///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAParaManager.C													 //
//   TAParaManager.C -- source file for class TAParaManager							 //
//   Introduction: Global parameter manager, a container class. All the parameter	 //
// used in the code would be registered in this class for assignment and retrieval.  //
// This class is supposed to be a singleton, reading parameters from config files.	 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/9/26.															     //
// Last modified: 2017/11/18, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <unistd.h>

#include "TAParaManager.h"
#include "TAMath.h"
#include "TAPopMsg.h"
#include "TACtrlPara.h"
#include "TADetUnion.h"
#include "TAParameter.h"
#include "TAChannel.h"
#include "TAChPara.h"
#include "TAAnode.h"
#include "TAAnodePara.h"
#include "TAMWDC.h"
#include "TAMWDCArray.h"
#include "TATOFWall.h"

//#define VERBOSE // show TAPopMsg::Info() information

using std::string;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;

TAParaManager *TAParaManager::kInstance = nullptr;

// not to be called from outside the class
TAParaManager::TAParaManager() : fDetList{0}{
	fPhysConst.reserve(10);
	for(auto &x : fUIDList) x = UID_DUMMY;

	// add physical constants
	AddPhysConst("e0", 1.60217733E-19); // elementary charge, unit: C
	AddPhysConst("u0", 1.66053886E-27); // atomic mass unit, unit: kg
	AddPhysConst("c0", 299.792458); //light speed in vacuum, unit: mm/ns
	AddPhysConst("mAr", 39.9525); // mass of naked argon, unit: u
}
TAParaManager::~TAParaManager(){
	for(TAParameter *&pa : fPhysConst){
		delete pa; pa = nullptr;
	}
	for(TADetUnion *&det : fDetList) if(det){
		delete det; det = nullptr;
	}
}
TAParaManager *TAParaManager::Instance(){
	if(!kInstance) kInstance = new TAParaManager();
	return kInstance;
}
double TAParaManager::GetPhysConst(const char *name) const{
	for(const TAParameter *pa : fPhysConst) if(!strcmp(pa->GetName().c_str(), name))
		return pa->GetValue();
	TAPopMsg::Error("TAParaManager", "GetPhysConst: %s not found.", name);
	return -9999.;
}
// add physical constants to fPhysConst list.
void TAParaManager::AddPhysConst(const char *name, double value){
	TAParameter *physConst = new TAParameter(name, name);
	physConst->SetValue(value);
	fPhysConst.push_back(physConst);
}
unsigned TAParaManager::GetUID(int chId) const{
	if(chId >= MAX_CH_NUM){
		TAPopMsg::Warn("TAParaManager", "GetUID: Input channel Id too large: %d", chId);
		exit(1);
	}
	unsigned uid = fUIDList[chId];
	if(UID_DUMMY == uid){ // dummy channel
//		TAPopMsg::Wran("TAParaManager", "GetUID: homeless channel: %d", chId);
	}
	return uid;
}
void TAParaManager::AddChPara(TAChPara *chpar){
	fChParaList.push_back(chpar);
}

// Every value whatsoever has a UID. This is the utmost principle of para management.
void TAParaManager::ReadParameters(){
	int cntNull = 0;
	for(auto p : fDetList) if(!p) cntNull++;
	if(fDetList.size() == cntNull){
		TAPopMsg::Warn("TAParaManager", "ReadParameters: fDetList is empty");
		return;
	}
	// read the cofig files and register them in config/content (a text file)
	string configPath = TACtrlPara::Instance()->ConfigExpDir();
	string content = configPath+"/content"; // file containing the file list
	RegisterConfigFiles(configPath.c_str());

	ifstream ff(content.c_str());
	if(!ff.is_open()){
		TAPopMsg::Error("TAParaManager", "ReadParaManager: read %s error", content.c_str());
	}
	char fname[512];
	while(ff.getline(fname, sizeof(fname))){
		if('#' == fname[0]) continue; // commentary line
		if(0 == strlen(fname)) continue; // blank line

		int type = FileType(fname);
		switch(type){
			case 0: // channel id
				AssignChId(fname); break;
			case 1: // detector position
				AssignDetPos(fname); break;
			case 2: // T0
				AssignT0(fname); break;
			case 3: // STR Correction
				AssignSTRCor(fname); break;
			default: break;
		} // end switch
	} // end external while

	// extract TF1 objects from rootfiles as STRs
	AssignSTR();

# ifdef VERBOSE
	TAPopMsg::Info("TAParaManager", "ReadParameters: Parameters has been read and assigned~ \033[33;1m:)\033[0m");
#endif
	// destruct those detectors that are not commissioned in the current experiment
	Clean(); // by telling the status of channel id of the detector units.
}







// read the cofig files and register them in a text file
void TAParaManager::RegisterConfigFiles(const char *basePath){
# ifdef VERBOSE
	TAPopMsg::Info("TAParaManager", "RegisterConfigFiles: The config file dir is %s", basePath);
#endif
	char fname[512]; // file name
	sprintf(fname, "%s/content", basePath);
	ofstream configFileList(fname); // output the file list to file
	ReadFileList(basePath, configFileList);
	configFileList.close();
# ifdef VERBOSE
	TAPopMsg::Info("TAParaManager", "RegisterConfigFiles: %s/content has been updated.", basePath);
#endif
}
int TAParaManager::ReadFileList(const char *basePath, ofstream &configFileList){
	DIR *dir;
	dirent *ptr;
	char base[1024], fname[1024];
	static int cnt = 0;
	if(cnt == 0){ // so that only one such remark would be printed into file
		configFileList << "################################################################\n";
		configFileList << "# BINGER Config File List: " << basePath << endl;
		configFileList << "# Automatically generated config file. DO NOT EDIT!" << endl;
		configFileList << "# Created: " << TAPopMsg::time0() << endl;
		configFileList << "################################################################\n";
		configFileList << endl << endl;
		cnt++;
	}

	if((dir = opendir(basePath)) == nullptr){
		perror("Open dir error...");
		exit(1);
	}

	while((ptr = readdir(dir)) != nullptr){
		if(!strcmp(ptr->d_name, ".") || !strcmp(ptr->d_name, ".."))
			continue; // current dir or parent dir
		else if(ptr->d_type == 8){ // file
			sprintf(fname, "%s/%s\n", basePath, ptr->d_name);
			configFileList << fname;
		}
		else if(ptr->d_type == 10){} // link file
		else if(ptr->d_type == 4){ // dir
			memset(base, '\0', sizeof(base));
			strcpy(base, basePath);
			strcat(base, "/");
			strcat(base, ptr->d_name);
			ReadFileList(base, configFileList); // recursive calling
		}
	} // end while
	closedir(dir);
	return 1;
} // end of ReadFileList
void TAParaManager::Clean(){
	const int n = fDetList.size();
	bool isEmpty[n]{0}; // each element for a detector
	memset(isEmpty, 1, sizeof(isEmpty));
	for(TAChPara *p : fChParaList){
		if(-2 != p->GetChannelId()){
			isEmpty[p->GetUID() & 0x3F] = false;
		}
	}
	for(int i = 0; i < n; i++) if(isEmpty[i] && fDetList[i]){
		delete fDetList[i]; fDetList[i] = nullptr;
	}
}
// 0: channel id; 1: detector position; 2: T0; 3: STR cor
int TAParaManager::FileType(const char *fname){
	int size = strlen(fname);
	static const char *type[] = {".000", ".001", ".002", ".003"};
	const int n = sizeof(type) / sizeof(const char *);
	for(int i = 0; i < n; i++){
		int sufLen = strlen(type[i]);
		if(!strcmp(fname + (size - sufLen), type[i]))
			return i;
	}
	return -1;
}


// assignment functions: read parameters from given file //
// channel id
void TAParaManager::AssignChId(const char *fname){
	ifstream cf(fname);
	if(!cf.is_open()) TAPopMsg::Error("TAParaManager", "AssignChId: read %s error", fname);
	char line[512];
	int linecnt = 0; // the current line number
	while(cf.getline(line, sizeof(line))){
		linecnt++;
		if('#' == line[0]) continue; // commentary line
		if(0 == strlen(line)) continue; // blank line

		double value, UID;
		sscanf(line, "%lg %lg", &value, &UID);

		// find the right channel and assign channel id
		if(UID < 0.) TAPopMsg::Error("TAParaManager", "AssignChId: UID is minus: %.0f", UID);
		int chId = int(value);
		unsigned uid = int(UID);
		if(chId < 0) continue; // dummy channel
		if(chId >= MAX_CH_NUM)
			TAPopMsg::Warn("TAParaManager", "AssignChId: channel Id too large: %d", chId);
		short detId = uid & 0x3F; // first section of UID, 6 bits
		TAChPara *chPara = nullptr;
		if(fDetList[detId]) chPara = fDetList[detId]->GetChannel(uid)->GetPara();
		if(chPara){
			if(fUIDList[chId] != UID_DUMMY)
				TAPopMsg::Error("TAParaManager", "AssignChId: chId %d's already been assigned with UID %d", chId, fUIDList[chId]);
			chPara->SetChannelId(chId);
			fUIDList[chId] = uid;
//			if(2 == detId){
//				TAPopMsg::Debug("TAParaManager", "AssignChId: chPara->GetChannelId(): %d", chPara->GetChannelId());
//				cout << chPara->GetName() << endl;
//				cout << "uid: " << uid << endl; getchar(); // DEBUG
//			}
		}
		else TAPopMsg::Warn("TAParaManager",
			"AssignChId: homeless Chid: %s: line %d", fname, linecnt);
	} // end internal while
} // end member function AssignChId
// Detector Position set
// file format: x	y	z	phi	theta	psi		UID
void TAParaManager::AssignDetPos(const char *fname){
	ifstream cf(fname);
	if(!cf.is_open()) TAPopMsg::Error("TAParaManager", "AssignDetPos: read %s error", fname);
	char line[512];
	int linecnt = 0; // the current line number
	while(cf.getline(line, sizeof(line))){
		linecnt++;
		if('#' == line[0]) continue; // commentary line
		if(0 == strlen(line)) continue; // blank line
		double value[6], UID; // value[6]: x, y, z, phi, theta, psi
		sscanf(line, "%lg %lg %lg %lg %lg %lg %lg", &value[0], &value[1],
			&value[2], &value[3], &value[4], &value[5], &UID);
		for(int i = 3; i--;) value[i+3] *= TAMath::DEGREE(); // degree -> rad
		
		// find the right detector and assign the posiiton set
		unsigned uid = unsigned(UID);
		bool isAssigned = false;
		short detId = uid & 0x3F; // first section of UID, 6 bits
		short subDetId = (uid>>6) & 0x7; // second section of UID for TAMWDCArray objects, 3 bits
		if(!fDetList[detId]){
			TAPopMsg::Error("TAParaManager", "AssignDetPos: null detector pointer.");
			return;
		}
		if(3 == detId || 4 == detId){ // valid TAMWDCArray pointer
			TAMWDCArray* dcArr = (TAMWDCArray*)fDetList[detId];
			if(subDetId < 3){ // MWDC
				TAMWDC *dc = (TAMWDC*)dcArr->GetMWDC(subDetId);
				dc->GetDetPara()->SetPosition(value);
				isAssigned = true;
			}
			else if(3 == subDetId){ // TOF Wall
				TATOFWall *tofw = (TATOFWall*)dcArr->GetTOFWall();
				tofw->GetDetPara()->SetPosition(value);
				tofw->AssignStripPosition();
				isAssigned = true;
			}
		} // end if(fDetList[detId])
		if(!isAssigned) TAPopMsg::Warn("TAParaManager",
			"AssignDetPos: homeless Detector Position: %s: line %d", fname, linecnt);
	} // end while

	// assign position for all anodes
	for(int i = 0; i < 2; i++){ // loop over MWDC arrays
		TAMWDCArray* dcArr = (TAMWDCArray*)fDetList[i + 3];
		for(int j = 0; j < 3; j++){ // loop over MWDCs
			TAMWDC *dc = (TAMWDC*)dcArr->GetMWDC(j);
			dc->AssignAnodePosition();
		} // end for over j
	} // end for over i
} // end member function AssignDetPos
// STR extraction from root file
void TAParaManager::AssignSTR(){
	static TACtrlPara *ctrlpara = TACtrlPara::Instance();

	TAMWDCArray	*dcArr[2]; // MWDC array L and R
	dcArr[0] = (TAMWDCArray*)fDetList[3];
	dcArr[1] = (TAMWDCArray*)fDetList[4];
	for(int i = 2; i--;) if(dcArr[i]){ // loop over two DC arrays
		for(int j = 3; j--;){ // loop over DCs in an array
			TAMWDC *dc = dcArr[i]->GetMWDC(j);
			const int n = dc->GetNAnodePerLayer();
			for(int m = 0; m < 3; m++){ // X-U-V
				for(int l = 0; l < n; l++){ // loop of anodes in a DC
					ctrlpara->AssignSTR((TAAnodePara*)dc->GetAnodeL1(m, l)->GetPara());
					ctrlpara->AssignSTR((TAAnodePara*)dc->GetAnodeL2(m, l)->GetPara());
				} // end for over anodes in a DC
			} // end for over X-U-V
		} // end for over j
	} // end loop over two DC arrays
} // end member function AssignSTR
// T0, including DC anode T0 and plastic scintillator T0
void TAParaManager::AssignT0(const char *fname){
	ifstream cf(fname);
	if(!cf.is_open()) TAPopMsg::Error("TAParaManager", "AssignT0: read %s error", fname);
	char line[512];
	int linecnt = 0; // the current line number
	while(cf.getline(line, sizeof(line))){
		linecnt++;
		if('#' == line[0]) continue; // commentary line
		if(0 == strlen(line)) continue; // blank line
		double value, UID;
		sscanf(line, "%lg %lg", &value, &UID);

		unsigned uid = unsigned(UID);
		short detId = uid & 0x3F; // first section of UID, 6 bits
		short subDetId = (uid>>6) & 0x7; // second section of UID for TAMWDCArray objects, 3 bits
		TAChPara *chPara = nullptr;
		if(fDetList[detId]) chPara = fDetList[detId]->GetChannel(uid)->GetPara();
		if(chPara){
			chPara->AppendDelay(value);
		}
		else TAPopMsg::Warn("TAParaManager",
			"AssignT0: homeless T0: %s: line %d", fname, linecnt);
	} // end while
}
// file format:
// #blah blah blah
// #Info: anode_UID angle_No valid_bin_cnt
// #STR cor_r_bin_#
// #STR cor
// Info: 123456789 0 3
// 0 3 7
// 0.23 -0.25 0.45
// spatial time relation correction table
void TAParaManager::AssignSTRCor(const char *fname){
	ifstream cf(fname);
	if(!cf.is_open()) TAPopMsg::Error("TAParaManager", "AssignSTRCor: read %s error", fname);
	char line[2048];
	int linecnt = 0; // the current line number
	while(cf.getline(line, sizeof(line))){
		linecnt++;
		if('#' == line[0]) continue; // commentary line
		if(0 == strlen(line)) continue; // blank line
		// read header of an STRCor item: anode UID, angle_#, valid_bin_cnt
		double UID, angle_No, va_bin_cnt;
		char info[6]; strncpy(info, line, 5);
		if(!strcmp(info, "Info:")){ // info line
			sscanf(info, "%*s %lg %lg %lg", &UID, &angle_No, &va_bin_cnt);
		}

		// get the channel matching the UID
		unsigned uid = unsigned(UID);
		TAAnode *ano = nullptr;
		short detId = uid & 0x3F; // first section of UID, 6 bits
		short subDetId = (uid>>6) & 0x7; // second section of UID for TAMWDCArray objects, 3 bits
		if((3 == detId || 4 == detId) && subDetId < 3){ // MWDC
			if(fDetList[detId]) ano = (TAAnode*)fDetList[detId]->GetChannel(uid);
		}
		if(!ano){
			TAPopMsg::Warn("TAParaManager",
				"AssignSTRCor: homeless STR correction parameter array: %s: line %d", fname, linecnt);
			// skip the wrong lines
			cf.getline(line, sizeof(line));
			cf.getline(line, sizeof(line));
			continue;
		}
		// read the correction parameters
		double *STRCorArr = new double[int(va_bin_cnt)];
		double *VaBinNumArr = new double[int(va_bin_cnt)];
		int *vaBinNumArr = new int[int(va_bin_cnt)]; // this is a replica of the array above
		int i = 0;
		// read valid bin id array
		while(i < va_bin_cnt){
			if(!(cf >> VaBinNumArr[i]))
				TAPopMsg::Error("TAManager", "AssignSTRCor: file format error 2: line %d", linecnt);
			vaBinNumArr[i] =  (int)VaBinNumArr[i];
			i++;
		}
		while(cf.get() != '\n') continue; // skip the rest of the line
		linecnt++;
		i = 0;
		// read valid STR_Cor array
		while(i < va_bin_cnt){
			if(!(cf >> STRCorArr[i]))
				TAPopMsg::Error("TAManager", "AssignSTRCor: file format error 1: line %d", linecnt);
			i++;
		}
		while(cf.get() != '\n') continue; // skip the rest of the line
		linecnt++;
		// assign the STR correction data.
		((TAAnodePara*)ano->GetPara())->SetSTRCorArr(vaBinNumArr, STRCorArr, (int)angle_No, (int)va_bin_cnt);
		delete [] STRCorArr;
		delete [] VaBinNumArr;
		delete [] vaBinNumArr;
	} // end internal while
}




