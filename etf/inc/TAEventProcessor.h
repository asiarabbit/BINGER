///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAEventProcessor.h												 //
//   TAEventProcessor.h -- header file for class TAEventProcessor					 //
//   Introduction: The global event processor class, a singleton class, responsible  //
// for distribution of channel data to the detector hierarchy and the right channel	 //
// objects. After the event is fully assigned, event assembly routine would be		 //
// initiated. It is also this class' duty to provide analysis result output			 //
// interface.																		 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/13.															     //
// Last modified: 2018/8/16, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAEVENTPROCESSOR_H_
#define _TAEVENTPROCESSOR_H_

#include <string>
#include <vector>
#include <climits>
#include "TAPopMsg.h"
#include "TACtrlPara.h"

using std::string;
using std::vector;

// ROOT class
class TGraph;
class TTree;

class TARawDataProcessor;
class TAParaManager;
class TAVisual;
class TAPID;
class TAGPar;

struct tEntry; // store raw data of one channel of readout electronics
struct tTrack; // store 2-D projection of straight particle track information
struct t3DTrkInfo; // store 3D track information, used to facilitate parameter passing
struct t3DPIDInfo; // store PID result using 3D tracks, used to facilitate parameter passing

class TAEventProcessor{
public:
	static TAEventProcessor* Instance(const string &datafile = "", int runId = 0);
	virtual ~TAEventProcessor();

	vector<tEntry *> &GetEntryList() { return fEntryList; }
	vector<tTrack *> &GetTrackList() { return fTrackList; }
	TARawDataProcessor *GetRawDataProcessor() const;
	TAParaManager *GetParaManager() const;
	TACtrlPara *GetCtrlPara() const;
	TAVisual *GetVisual() const;
	TAPID *GetPID() const;
	TAGPar *GetGPar() const;
	void SetConfigExpDir(const string &dir);
	void SetSTRROOTFile(const string &file);
	void SetDataFile(const string &datafile, int runId, bool isPXI = true);
	void SetMagneticIntensity(double B);
	// whether or not to implement the particle tracking or rigidity analysis
	void SetIsPID(bool opt = true){ fIsPID = opt; }
	void SetIsTracking(bool opt = true){ fIsTracking = opt; }
	void SetIs3DTracking(bool opt = true);
	bool Is3DTracking() const;
	bool IsPID() const{ return fIsPID; }
	bool IsTracking() const{ return fIsTracking; }
	void Verbose(bool opt = true){ TAPopMsg::Verbose(opt); } // true or null: verbose; false: succinct
	void Silent(bool opt = true){ TAPopMsg::Silent(opt); } // true: silent TAPopMsg::Info: false: not
	void CoarseFit(bool opt = true){ GetCtrlPara()->CoarseFit(opt); } // rough ye fast fit
	void SetPeriod(int index0, int index1); // analyze index0 to index1
	virtual void Configure(); // create detectors
	void CheckChannelId() const; // check the the channel with channelId being chId
	void CheckChannelId(int chId){ fChkChId = chId; } // check the the channel with channelId being chId
	// assign an event to the detectors by distributing channel data to the matching channel objects
	virtual void Assign();
	virtual void Assign(tEntry *entry);
	// analyze one event after its channels data are assigned to detectors
	virtual void Analyze();
	// correct drift time and refit with the update
	// tof2: time of flight from target to TOFWall; taHitX: hit pos x in TA
	virtual void RefineTracks(int &n3Dtr, t3DTrkInfo *trk3DIf, const double *tof2, const double *taHitX);
	virtual void RefinePID(const int n3Dtr, const t3DTrkInfo *trk3DIf, t3DPIDInfo *pid3DIf);
	// a method dedicated for TAVisual::Fill()
	void FillTrack(TGraph *gTrack, TGraph *gTrack_R) const;
	// the overall data analysis routine
	// (id0, id1): index range for analysis; secLenLim: event length limit; rawrtfile: raw rootfile
	virtual void Run(int id0 = 0, int id1 = INT_MAX, int secLenLim = INT_MAX, const string &rawrtfile = "");
	virtual void Initialize();
protected:
	TAEventProcessor(const string &datafile = "", int runId = 0);

	static TAEventProcessor* fInstance;
	bool fIsPID; // whether or not to implement the rigidity analysis
	bool fIsTracking; // whether or not to implement particle tracking
	int fChkChId; // channel with chid input by user for checking and showing
	vector<tEntry *> fEntryList; // store channel data entry of one data section
	vector<tTrack *> fTrackList; // store particle track of one data section
	TARawDataProcessor *fRawDataProcessor; // to read raw binary data file into a ROOT tree
	TAParaManager *fParaManager; // holding channel ids, channel delays, detector layouts, etc.
	TACtrlPara *fCtrlPara; // control parameters to regulate program running
	TAVisual *fVisual; // for visualize the data analysis result
	TAPID *fPID; // particle identification method object
	TAGPar *fGPar; // a container storing global parameters
};

#endif

