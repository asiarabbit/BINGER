/**
	\file TACtrlPara.h
	\class TACtrlPara
	\brief mainly for holding global parameters used for program control in
	tracking precision level and tracking procedures.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2017/10/7 Last revised: 2018/6/7, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TACTRLPARA_H_
#define _TACTRLPARA_H_

#include <string>
using std::string;

class TAAnodePara;

class TACtrlPara{
public:
	static TACtrlPara *Instance();
	virtual ~TACtrlPara();
	//-------------- read offline data --------------//
	bool IsCheckBunchIdAlignment() const;
	bool IsDriftTimeQtCorrection() const;
	bool IsCoarseFit() const;
	bool Is3DTracking() const;
	/// tolerance window for 3D coincidence test of X U and V track projections
	/// 5: half a DC cell, given all kinds of errors
	double Get3DCoincideWindow() const;
	double D2Thre(unsigned uid = 999999999) const; ///< for eliminating falsely fired andoes. unit: mm^2
	double DsquareThresholdPerDot(unsigned uid) const;
	bool TimeThre(double t, unsigned uid = 999999999) const; ///< if time is within set range
	double ChiThre(unsigned uid = 999999999) const; ///< threshold for chi average
	/// threshold for chi per dot, to eliminate false combinations. 4.0
	double ChiThrePD(unsigned uid = 999999999) const;
	int Vicinity() const; ///< used in discerning multiple tracks, unit: cell
	int StripTolerance() const; ///< used in discerning multiple tracks, unit: strip
	/// TATrack::kBFGSFit; // kNormalFit: 0; kBFGSFit: 1 kIterFit: 2
	int FitMethod() const;
	/// only effective if input fit method is kNormalFit
	/// allowed value: -2, -1, 0, 1, 2, 3, with calculation depth increasing. unit: mm^2
	int Precision() const;
	/// MWDCArrayR_DC1_U: installation error
	double DCArrR_DC1UHorizontalDeviation() const;
	/// as its name indicates, used in TAAnode::GetDriftTime
	double DriftTimeQtCorrectionWeight() const{ return kDriftTimeQtCorrectionWeight; }
	const char *DataFileName() const;
	const char *RootFileName() const;
	const char *ConfigExpDir() const;
	const char *STRROOTFile() const;
	/// calculate the minmum deviation of a track off the fired strips in a TOF wall
	void GetNStripStrayRangeR(double &minR, double &maxR) const;
	void GetNStripStrayRangeL(double &minL, double &maxL) const;
	bool TOFWallStripStrayTest(double strayMin, unsigned uid) const;
	void AssignSTR(TAAnodePara *para) const;

	/// spatial resolution used to smear drift distance during the generation of simulation data
	/// This method is used in TAAnodePara::GetSpatialResolution()
	double GetSimSpatialResolution() const;
	void SetSimSpatialResolution(double sigmar);

	/// rough time of flight from DC to TOF wall unit: ns
	/// for pattern recognition purposes only
	/// PION: {12.5, 7.5, 2.5}; BEAM TEST: {29.06, 20.34, 18.23} - 17.73 - 1.5
	double T_tofDCtoTOFW(unsigned uid) const;
	double T_wireMean(unsigned uid) const;

	void SetConfigExpDir(const string &dir);
	void SetSTRROOTFile(const string &file);
	void CoarseFit(bool opt = true){ kIsCoarseFit = opt; }
	void SetIs3DTracking(bool opt = true){ kIs3DTracking = opt; }
private:
	TACtrlPara();
	static TACtrlPara *kInstance;

	bool kIsCoarseFit; ///< true then only iterative fit would be used
	bool kIs3DTracking; ///< whether or not to implement 3D tracking

	//---------  for pattern recognition -------------------//
	/// for map function
	double kDriftTimeQtCorrectionWeight; ///< as its name indicates, used in TAAnode.C
	double fSigmaR; ///< spatial resolution used to smear drift distance during the generation of simulation data

	string kDataFile; ///< for extra use
	/// the directory name in config directory holding the selected experiment config files
	string kConfigExpDir;
	/// STRs for MWDC calculated from simulation
	string kSTRROOTFile; ///< root file containing TF1 objects for MWDCs: aaa%d.root 1500 1000 900 [V]
	string kRootFile; ///< used in refined fit and calibration phase //  20161126_0522.dat.root
};

#endif
