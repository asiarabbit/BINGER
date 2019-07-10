/**
	\file TASTRCalibDCArr.h
	\class TASTRCalibDCArr
	\brief A tool class to calibrate DC STRs using auto-calibration. An abstract base class.
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2017/10/18 Last revised: 2018/6/8, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/

#ifndef _TASTRCALIBDCARR_H_
#define _TASTRCALIBDCARR_H_

#include <string>

class TAMWDCArray;

using std::string;

class TASTRCalibDCArr{
public:
	TASTRCalibDCArr(const string &rootfile = "", TAMWDCArray *dcArr = nullptr);
	virtual ~TASTRCalibDCArr() = 0;

	void SetROOTFile(const string &rootfile){ fROOTFile = rootfile; }
	void SetMWDCArray(TAMWDCArray *dcArr){ fDCArr = dcArr; }
	void SetIsBigStatistics(bool opt){ fIsBigStatistics = opt; }
	bool IsBigStatistics() const{ return fIsBigStatistics; }
	virtual void ChiHistogramming(bool is3D); ///< is3D: whether to use 3D chi to fill the histograms
	virtual void GenerateSTRCorFile(int round = 0);
	/// \param isBigData: whether or not the data statistics is enough
	static void ChiHistogramming(const string &rootfile, TAMWDCArray *dcArr, bool is3D = false, bool isBigSta = true);
	/// \param round: STR auto-calibration round
	static void GenerateCalibFile(const string &rootfile, TAMWDCArray *dcArr, int round = 0);
protected:
	/// parameters for STR correction binning, from class TAAnodePara
	static const int nr; ///< number of drift distance bins
	static const double rmx; ///< maximum drift distance
	static const int nAng; ///< number of bins for track-drift_cell angle
	static const double rstep; ///< bin width of drfit distance

	TAMWDCArray *fDCArr;
	string fROOTFile;
	/// Mark if the statistics is enough.\n
	/// If not enough, then a whole anode layer would be used as the data
	/// for any of the sense wire in the layer
	bool fIsBigStatistics;
};

#endif
