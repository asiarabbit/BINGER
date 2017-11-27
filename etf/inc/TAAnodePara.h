///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAAnodePara.h														 //
//   TAAnodePara.h -- header file for class TAAnodePara								 //
//   Introduction: derived from TAChPara, embodiment of DC anode parameter set.		 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/9/24.															     //
// Last modified: 2017/10/15, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAANODEPARA_H_
#define _TAANODEPARA_H_

#include "TAChPara.h"
// ROOT include
#include "TF1.h"

class TAMWDC;

class TAAnodePara : public TAChPara{
public:
	TAAnodePara(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAAnodePara();
	virtual double GetDelay() const;
	// projection point of the anode to the normal plane
	double GetProjectionX() const;
	double GetProjectionZ() const;
	void GetGlobalCenter(double *Ag) const;
	void GetGlobalDirection(double *ag) const; // anode orientation vector
	void GetGlobalProjection(double *Ag) const; // anode projection
	const double *GetSTRCorrection(int angle_no) const;
	// specialised for STR calibration
	static int GetDriftDistanceBinNumber(double driftDistance);
	TF1 *GetSTR(int STR_id) const;
	TF1 *GetSTR(double k, int dcType) const;
	TAMWDC *GetMotherDC() const;
	virtual int GetSFE16Id() const;
	virtual int GetCableId() const;
	int GetSTRid(double k, int dcType) const;
	void SetGlobalCenter(const double *Ag);
	void SetGlobalDirection(const double *ag){ fGlobalDirection = ag; }
	void SetGlobalProjection(const double *proj);
	void SetSTRCorArr(const int *vaBinNumArr, const double *strCorArr, int angle_no, int va_bin_cnt);
	void SetSTR(TF1 *f, int id);
	void SetMotherDC(TAMWDC *dc){ fMotherDC = dc; }
	double GetSpatialResolution(double r, double k) const; // for generate simulation of data
	static void DriftTimeQtCorrection(double &driftTime, double TOT, double &weight);

	// static variables for STR correction table definition
	static const int kSTRCorRNBins = 60; // number of drift distance bins to implement STR correction
	static const int kSTRCorAngleNBins = 6; // number of bins of ..., has to be even, greater than 2
	static const double kSTRCorRMax; // maximum dirft distance
	static const double kSTRCorRStep; // bin width
	static const double kSTRCorAngleMax; // maximum track and drift cell angle
	static const double kSTRCorAngleStep; // bin width of ...
protected:
	// for storing STR correction table
	double fGlobalCenter[3]; // global coordinate of the center of the anode
	double fGlobalProjection[3]; // global coordinate the center projection on the andoe normal plane
	const double *fGlobalDirection; // global orientation vector, pointing to the Mother MWDC direction array
	// Space Time Relation (STR) correction table.
	const double *fSTRCorArr[kSTRCorAngleNBins];
	static const double kSTRCorArrDummy[kSTRCorRNBins];
	TF1 *fSTR[kSTRCorAngleNBins];
	TAMWDC *fMotherDC; // DC the anode belongs to
};

#endif
