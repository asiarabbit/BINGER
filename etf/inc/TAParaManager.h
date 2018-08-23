///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAParaManager.h													 //
//   TAParaManager.h -- header file for class TAParaManager							 //
//   Introduction: Global parameter manager, a container class. All the parameter	 //
// used in the code would be registered in this class for assignment and retrieval.  //
// This class is supposed to be a singleton, reading parameters from config files.	 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/9/26.															     //
// Last modified: 2017/10/16, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAPARAMANAGER_H_
#define _TAPARAMANAGER_H_

#include <vector>
#include <array>
#include <fstream>

using std::vector;
using std::array;

class TAParameter;
class TAChPara;
class TADetUnion;
class TACtrlPara;

class TAParaManager{
public:
	typedef array<TADetUnion *, 64> ArrDet_t; // 64: 0x3F. This value should NOT be changed

	static TAParaManager *Instance();
	virtual ~TAParaManager();
	// read all the parameters required
	// types in typeIgnore[NIgnore] would be ignored
	void ReadParameters(const short NIgnore = -1, const short *typeIgnore = nullptr);
	void UpdateSTRCorrection(); // using the current STR-correction table, for STRCali
	double GetPhysConst(const char *name) const;
	ArrDet_t &GetDetList(){ return fDetList; }
	vector<TAChPara *> &GetChParaList(){ return fChParaList; }
	// add physical constants to fPhysConst list
	void AddPhysConst(const char *name, double value);
	void AddChPara(TAChPara *chpar);
	const unsigned *UIDList() const{ return fUIDList; }
	unsigned GetUID(int chId) const;
	// to tell if calibration files of a speficied type exist
	bool Exist(const short type) const; // used in aterwards calibraion procedures
	static const int MAX_CH_NUM = 20000; // maximum number of channels
	// UINT_MAX: 4,294,967,295   999,999,999
	static const int UID_DUMMY = 999999999; // dummy uid for channels with unknown channel ids
protected:
	// read the cofig files and register them in a text file
	void RegisterConfigFiles(const char *basePath) const;
	// destruct those detectors that are not commissioned in the current experiment
	void Clean(); // by telling the status of channel id of the detector' units.
	static int FileType(const char *fname); // tell file type by suffix
	// read all the files under a designated file
	// isAddHeader: ReadFileList may be called several times from without, then isAddHeader should be
	// activated to distinguish itself from calling within
	static int ReadFileList(const char *basePath, std::ofstream &configFileList, bool isAddHeader = false);
	TAParaManager(); // not to be called from outside the class

	// assignment functions: read parameters from given file
	void AssignChId(const char *fname); // channel id
	void AssignDetPos(const char *fname) const; // Detector Position set
	void AssignT0(const char *fname) const; // T0, including DC anode T0 and plastic scintillator T0
	void AssignSTRCor(const char *fname) const; // spatial time relation correction table
	void AssignSTR() const; // spatial time relation extract from rootfile
	void AssignGPar(const char *fname) const; // user input control parameters

	static TAParaManager *kInstance;
	vector<TAParameter *> fPhysConst; // physics constants
	ArrDet_t fDetList; // detectors created
	vector<TAChPara *> fChParaList; // register all the TAChPara objects in this vector
	unsigned fUIDList[MAX_CH_NUM]; // UID-ChId map, with the subscrip being channel id
};

#endif
