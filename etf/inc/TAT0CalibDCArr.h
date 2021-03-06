/**
	\file TAT0CalibDCArr.h
	\class TAT0CalibDCArr
	\brief A tool class to calibrate DCT0(delay)s anode by anode by fitting
	drift time distribution. An abstract base class.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2017/10/18 Last revised: 2018/4/3, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/


#ifndef _TAT0CALIBDCARR_H_
#define _TAT0CALIBDCARR_H_

#include <string>

class TAMWDCArray;

using std::string;

class TAT0CalibDCArr{
public:
	TAT0CalibDCArr(const string &rootfile = "", TAMWDCArray *dcArr = nullptr);
	virtual ~TAT0CalibDCArr() = 0;
	
	void SetROOTFile(const string &rootfile){ fROOTFile = rootfile; }
	void SetMWDCArray(TAMWDCArray *dcArr){ fDCArr = dcArr; }
	/// if T_tof and T_wire has been corrected for in pattern recognition stage
	void SetHasCorrected(bool opt = true){ fHasCorrected = opt; }
	bool HasCorrected() const{ return fHasCorrected; }
	/// generate T0 calibration text files
	virtual void Refine_DTHisto(bool isCalib = true);
	virtual void GenerateCalibFile(bool isShowFit = false);
	/// isCalib: whether to store hdt histos and generate calibration file
	static void Refine_DTHisto(const string &rootfile, TAMWDCArray *dcArr, bool hasCorrected = true, bool isCalib = true);
	/// isShowFit: whether to show the fitting graphs
	static void GenerateCalibFile(const string &rootfile, TAMWDCArray *dcArr, bool isShowFit = false);
protected:
	static double Fermi_Dirac_Function(double *x, double *p); ///< used to fit the drfit time spectrum.
	string fROOTFile;
	TAMWDCArray *fDCArr;
	bool fHasCorrected; ///< whether T_tof and T_wire have been corrected for from drift time in PatReg
};

#endif
