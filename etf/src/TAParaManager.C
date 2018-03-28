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
// Last modified: 2018/1/27, SUN Yazhou.										     //
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
#include "TAGPar.h"
#include "TAUIDParser.h"

//#define VERBOSE // show TAPopMsg::Info() information

using std::string;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;

TAParaManager *TAParaManager::kInstance = nullptr;
static TAGPar *gp = TAGPar::Instance();

// not to be called from outside the class
TAParaManager::TAParaManager() : fDetList{0}{
	fPhysConst.reserve(10);
	for(auto &x : fUIDList) x = UID_DUMMY;

	// add physical constants
	AddPhysConst("e0", 1.60217733E-19); // elementary charge, unit: C
	AddPhysConst("u0", 1.66053886E-27); // atomic mass unit, unit: kg
	AddPhysConst("u0MeV", 931.493582); // atomic mass unit, unit: MeV
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
	if(-2 == chId){ // default chid -- not assigned
//		TAPopMsg::Wran("TAParaManager", "GetUID: default channel Id: channel Id not assigned %d", chId);
		return UID_DUMMY * 2; // default value: -> default chId: ch not used in expriment.
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

bool TAParaManager::Exist(const short type_) const{
	// read the cofig files and register them in config/content (a text file)
	string configPath = TACtrlPara::Instance()->ConfigExpDir();
	string content = configPath+"/content"; // file containing the file list
	RegisterConfigFiles(configPath.c_str());

	ifstream ff(content.c_str());
	if(!ff.is_open()){
		TAPopMsg::Error("TAParaManager", "Exist: read %s error", content.c_str());
	}
	char fname[512];
	while(ff.getline(fname, sizeof(fname))){
		if('#' == fname[0]) continue; // commentary line
		if(0 == strlen(fname)) continue; // blank line

		int type = FileType(fname);
		if(type_ == type) return true;
	} // end external while
	return false;
}
// skip spaces and tabs, return subscript of the valid char
inline int skipCrap(const char *s){
	int tmp = 0;
	while(1){
		char c = s[tmp++];
		if(' ' != c && '\t' != c) break;
	}
	return tmp - 1;
}
// Every value whatsoever has a UID. This is the utmost principle of para management
// types in typeIgnore[NIgnore] would be ignored
void TAParaManager::ReadParameters(const short NIgnore, const short *typeIgnore){
	unsigned cntNull = 0;
	for(auto p : fDetList) if(!p) cntNull++;
	if(fDetList.size() == cntNull && NIgnore <= 0){ // NIgnore > 0: after TAEvPro::Config.
		TAPopMsg::Warn("TAParaManager", "ReadParameters: fDetList is empty");
		return;
	}
	// read the cofig files and register them in config/content (a text file)
	string configPath = TACtrlPara::Instance()->ConfigExpDir();
	if(0 != access(configPath.c_str(), F_OK))
		TAPopMsg::Error("TAParaManager", "ReadParameters: Folder %s doesn't exist", configPath.c_str());
	string content = configPath+"/content"; // file containing the file list
	RegisterConfigFiles(configPath.c_str());

	ifstream ff(content.c_str());
	if(!ff.is_open()){
		TAPopMsg::Error("TAParaManager", "ReadParaManager: read %s error", content.c_str());
	}
	char fname[512];
	while(ff.getline(fname, sizeof(fname))){
		int tmp = skipCrap(fname);
		if('#' == fname[tmp] || '\0' == fname[tmp]) continue; // commentary line
		if(0 == strlen(fname)) continue; // blank line

		int type = FileType(fname);
		bool isIgnore = false;
		for(int i = 0; i < NIgnore; i++)
			if(typeIgnore[i] == type) isIgnore = true;
		if(isIgnore) continue;
		switch(type){
			case 0: // channel id
				AssignChId(fname); break;
			case 1: // detector position
				AssignDetPos(fname); break;
			case 2: // T0
				AssignT0(fname); break;
			case 3: // STR Correction
				AssignSTRCor(fname); break;
			case 4: // Global parameters
				AssignGPar(fname); break;
			default: break;
		} // end switch
	} // end external while
//	cout << "Precision: " << gp->Val(48) << endl; getchar(); // DEBUG

	// extract TF1 objects from rootfiles as STRs
	AssignSTR(); // XXX: should be implemented after TAGPar

# ifdef VERBOSE
	TAPopMsg::Info("TAParaManager", "ReadParameters: Parameters has been read and assigned~ \033[33;1m:)\033[0m");
#endif
	// destruct those detectors that are not commissioned in the current experiment
	// FIXME: this function is a flawed function, may not be used.
//	Clean(); // by telling the status of channel id of the detector units.
}
void TAParaManager::UpdateSTRCorrection(){
	// read T0 and STR correction calibration constants
	const short nignore = 3, typeignore[nignore] = {0, 1, 4};
	ReadParameters(nignore, typeignore);
}




// read the cofig files and register them in a text file
void TAParaManager::RegisterConfigFiles(const char *basePath) const{
# ifdef VERBOSE
	TAPopMsg::Info("TAParaManager", "RegisterConfigFiles: The config file dir is %s", basePath);
#endif
	char fname[512]; // file name
	sprintf(fname, "%s/content", basePath);
	ofstream configFileList(fname); // output the file list to file
	ReadFileList(basePath, configFileList, true);
	configFileList.close();
# ifdef VERBOSE
	TAPopMsg::Info("TAParaManager", "RegisterConfigFiles: %s/content has been updated.", basePath);
#endif
}
int TAParaManager::ReadFileList(const char *basePath, ofstream &configFileList, bool isAddHeader){
	DIR *dir;
	dirent *ptr;
	char base[1024], fname[1024];
	static int cnt = 0;
	if(isAddHeader) cnt = 0;
	if(0 == cnt){ // so that only one such remark would be printed into file
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
	static const char *type[] = {".000", ".001", ".002", ".003", ".004"};
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
		int tmp = skipCrap(line);
		if('#' == line[tmp] || '\0' == line[tmp]) continue; // commentary line
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
void TAParaManager::AssignDetPos(const char *fname) const{
	ifstream cf(fname);
	if(!cf.is_open()) TAPopMsg::Error("TAParaManager", "AssignDetPos: read %s error", fname);
	char line[512];
	int linecnt = 0; // the current line number
	while(cf.getline(line, sizeof(line))){
		linecnt++;
		int tmp = skipCrap(line);
		if('#' == line[tmp] || '\0' == line[tmp]) continue; // commentary line
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
			TAPopMsg::Error("TAParaManager", "AssignDetPos: null detector pointer");
			continue;
		}

		// MWDCArrayL-R //
		if(3 == detId || 4 == detId){ // valid TAMWDCArray pointer
			TAMWDCArray* dcArr = (TAMWDCArray*)fDetList[detId];
			if(subDetId < 3){ // MWDC
				TAMWDC *dc = dcArr->GetMWDC(subDetId);
				dc->GetDetPara()->SetPosition(value);
				isAssigned = true;
			}
			else if(3 == subDetId){ // TOF Wall
				TATOFWall *tofw = dcArr->GetTOFWall();
				tofw->GetDetPara()->SetPosition(value);
				tofw->AssignStripPosition();
				isAssigned = true;
			}
		} // end if(3 == detId || 4 == detId)
		// MWDCArrayU-D //
		if(6 == detId || 7 == detId){ // valid TAMWDCArray pointer
			TAMWDCArray2* dcArr = (TAMWDCArray2*)fDetList[detId];
			if(subDetId < 2){ // MWDC
				TAMWDC *dc = dcArr->GetMWDC(subDetId);
				dc->GetDetPara()->SetPosition(value);
				isAssigned = true;
			}
		} // end if(6 == detId || 7 == detId)

		if(!isAssigned) TAPopMsg::Warn("TAParaManager",
			"AssignDetPos: homeless Detector Position: %s: line %d", fname, linecnt);
	} // end while

	// assign position for all anodes
	for(int i = 0; i < 2; i++){ // loop over MWDC array L-R
		TAMWDCArray* dcArr = (TAMWDCArray*)fDetList[i + 3];
		if(!dcArr) continue;
		for(int j = 0; j < 3; j++){ // loop over MWDCs
			TAMWDC *dc = dcArr->GetMWDC(j);
			dc->AssignAnodePosition();
		} // end for over j
	} // end for over i
	for(int i = 0; i < 2; i++){ // loop over MWDC array U-D
		TAMWDCArray2* dcArr = (TAMWDCArray2*)fDetList[i + 6];
		if(!dcArr) continue;
		for(int j = 0; j < 2; j++){ // loop over MWDCs
			TAMWDC *dc = dcArr->GetMWDC(j);
			dc->AssignAnodePosition();
		} // end for over j
	} // end for over i
} // end member function AssignDetPos
// STR extraction from root file
void TAParaManager::AssignSTR() const{
	if(!gp->HasRead())
		TAPopMsg::Error("TAParaManager", "AssignSTR: Global Parameters in TAGPar have not been assigned with values read from config files, where HV information is stored");
	static TACtrlPara *ctrlpara = TACtrlPara::Instance();

	TAMWDCArray *dcArr[2]; // MWDC array L and R
	dcArr[0] = (TAMWDCArray*)fDetList[3];
	dcArr[1] = (TAMWDCArray*)fDetList[4];
	for(int i = 2; i--;) if(dcArr[i]){ // loop over two DC arrays
		for(int j = 3; j--;){ // loop over DCs in an array
			TAMWDC *dc = dcArr[i]->GetMWDC(j);
			const int n = dc->GetNAnodePerLayer();
			for(int m = 0; m < 3; m++){ // X-U-V
				for(int l = 0; l < n; l++){ // loop over anodes in a DC
					ctrlpara->AssignSTR(dc->GetAnodeL1(m, l)->GetAnodePara());
					ctrlpara->AssignSTR(dc->GetAnodeL2(m, l)->GetAnodePara());
				} // end for over anodes in a DC
			} // end for over X-U-V
		} // end for over j
	} // end loop over two DC arrays

	TAMWDCArray2 *dcArr2[2]; // MWDC array U and D
	dcArr2[0] = (TAMWDCArray2*)fDetList[6];
	dcArr2[1] = (TAMWDCArray2*)fDetList[7];
	for(int i = 2; i--;) if(dcArr2[i]){ // loop over two DC arrays
		for(int j = 2; j--;){ // loop over DCs in an array
			TAMWDC *dc = dcArr[i]->GetMWDC(j);
			const int n = dc->GetNAnodePerLayer();
			for(int m = 0; m < 2; m++){ // X-Y
				for(int l = 0; l < n; l++){ // loop over anodes in a DC
					ctrlpara->AssignSTR(dc->GetAnodeL1(m, l)->GetAnodePara());
					ctrlpara->AssignSTR(dc->GetAnodeL2(m, l)->GetAnodePara());
				} // end for over anodes in a DC
			} // end for over X-U-V
		} // end for over j
	} // end loop over two DC arrays
} // end member function AssignSTR
// T0, including DC anode T0 and plastic scintillator T0
void TAParaManager::AssignT0(const char *fname) const{
	ifstream cf(fname);
	if(!cf.is_open()) TAPopMsg::Error("TAParaManager", "AssignT0: read %s error", fname);
	char line[512];
	int linecnt = 0; // the current line number
	while(cf.getline(line, sizeof(line))){
		linecnt++;
		int tmp = skipCrap(line);
		if('#' == line[tmp] || '\0' == line[tmp]) continue; // commentary line
		if(0 == strlen(line)) continue; // blank line
		double value, UID;
		sscanf(line, "%lg %lg", &value, &UID);

		unsigned uid = unsigned(UID);
		int type[6]{};
		TAUIDParser::DNS(type, UID);
		short detId = type[0]; // first section of UID, 6 bits
//		cout << "fname: " << fname << endl; getchar(); // DEBUG
//		cout << "line: " << line << endl; // DEBUG
//		cout << "\tuid: " << uid << "\tvalue: " << value << endl; // DEBUG
//		cout << "detId: " << detId << "\tsubDetId: " << subDetId << endl; // DEBUG
		TAChPara *chPara = nullptr;
		if(fDetList[detId]) chPara = fDetList[detId]->GetChannel(uid)->GetPara();
		if(chPara){
//			cout << chPara->GetName() << endl; // DEBUG
//			cout << chPara->GetDelay() << endl; // DEBUG
			chPara->AppendDelay(value);
//			cout << "value: " << value << endl; // DEBUG
//			cout << chPara->GetDelay() << endl; getchar(); // DEBUG
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
void TAParaManager::AssignSTRCor(const char *fname) const{
	ifstream cf(fname);
	if(!cf.is_open()) TAPopMsg::Error("TAParaManager", "AssignSTRCor: read %s error", fname);
	char line[2048];
	int linecnt = 0; // the current line number
	while(cf.getline(line, sizeof(line))){
		linecnt++;
		int tmp = skipCrap(line);
		if('#' == line[tmp] || '\0' == line[tmp]) continue; // commentary line
		if(0 == strlen(line)) continue; // blank line
//		cout << "line: " << line << endl; // DEBUG
		// read header of an STRCor item: anode UID, angle_#, valid_bin_cnt
		double UID, angle_No, va_bin_cnt;
		char info[6]; strncpy(info, line, 5);
		if(!strcmp(info, "Info:")){ // info line
			sscanf(line, "%*s %lg %lg %lg", &UID, &angle_No, &va_bin_cnt);
		}
//		cout << "UID: " << UID << "\tangle_No:  " << angle_No << "\tva_bin_cnt: " << va_bin_cnt << endl; getchar(); // DEBUG
		if(0. == va_bin_cnt) continue; // empty

		// get the channel matching the UID
		unsigned uid = unsigned(UID);
		TAAnode *ano = nullptr;
		short detId = uid & 0x3F; // first section of UID, 6 bits
		short subDetId = (uid>>6) & 0x7; // second section of UID for TAMWDCArray objects, 3 bits
		if( ((3 == detId || 4 == detId) && subDetId < 3) // MWDC Array L-R
		 || ((6 == detId || 7 == detId) && subDetId < 2)) // MWDC Array U-D
		{ // MWDC
			if(fDetList[detId]) ano = (TAAnode*)fDetList[detId]->GetChannel(uid);
		}
//		cout << "ano->GetName(): " << ano->GetName() << endl; getchar(); // DEBUG
		if(!ano){
			TAPopMsg::Warn("TAParaManager",
				"AssignSTRCor: homeless STR correction parameter array: %s: line %d", fname, linecnt);
			// skip the wrong lines
			cf.getline(line, sizeof(line)); linecnt++;
			cf.getline(line, sizeof(line)); linecnt++;
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
//			cout << "i: " << i << "\tvaBinNumArr[i]: " << vaBinNumArr[i]; // DEBUG
//			cout << "\tVaBinNumArr[i]: " << VaBinNumArr[i]; getchar(); // DEBUG
			i++;
		}
		while(cf.get() != '\n') continue; // skip the rest of the line
		linecnt++;
		i = 0;
		// read valid STR_Cor array
		while(i < va_bin_cnt){
			if(!(cf >> STRCorArr[i]))
				TAPopMsg::Error("TAManager", "AssignSTRCor: file format error 1: line %d", linecnt);
//			cout << "i: " << i << "\tSTRCorArr[i]: " << STRCorArr[i]; // DEBUG
//			getchar(); // DEBUG
			i++;
		}
		while(cf.get() != '\n') continue; // skip the rest of the line
		linecnt++;
		// assign the STR correction data.
		ano->GetAnodePara()->SetSTRCorArr(vaBinNumArr, STRCorArr, (int)angle_No, (int)va_bin_cnt);
		delete [] STRCorArr;
		delete [] VaBinNumArr;
		delete [] vaBinNumArr;
	} // end internal while
}
// read global parameters from text config files for user input
// the config files are suffixed with .004, and stored in config/[exp]/control/
// file format: paraId value
void TAParaManager::AssignGPar(const char *fname) const{
	ifstream cf(fname);
	if(!cf.is_open()) TAPopMsg::Error("TAParaManager", "AssignGPar: read %s error", fname);
	char line[512];
	int linecnt = 0; // the current line number
	while(cf.getline(line, sizeof(line))){
		linecnt++;
		int tmp = skipCrap(line);
		if('#' == line[tmp] || '\0' == line[tmp]) continue; // commentary line
		if(0 == strlen(line)) continue; // blank line
		double paId, value;
		sscanf(line, "%lg %lg", &paId, &value);

		unsigned id = unsigned(paId);
		gp->Parameter(id)->SetValue(value);
		gp->Parameter(id)->SetUID(UID_DUMMY+paId);
//		cout << "id: " << id << "\tvalue: " << value << endl; // DEBUG
//		gp->ShowPar(id); getchar(); // DEBUG
	} // end while
	gp->SetHasRead(true);
}



