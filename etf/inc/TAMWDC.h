///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAMWDC.h															 //
//   TAMWDC.h -- header file for class TAMWDC										 //
//   Introduction: MWDC class, providing position calculation service for wires in	 //
// the MWDC. A container class, storing TADCSuperLayer objects.						 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/3.															     //
// Last modified: 2018/4/4, SUN Yazhou.											     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#ifndef _TAMWDC_H_
#define _TAMWDC_H_

#include <vector>
#include "TADetector.h"

using std::vector;

class TF1; // ROOT class
class TADCSuperLayer;
class TAMWDCArray;
class TAAnode;

class TAMWDC : public TADetector{
public:
	TAMWDC(const string &name = "", const string &title = "", unsigned uid = 999999999);
	virtual ~TAMWDC();
	short GetMWDCId() const;
	short GetNAnodePerLayer() const;
	TADCSuperLayer *GetSLayer(int id) const;
	TAMWDCArray *GetMotherDCArr() const;
	void SetSLayer(int n, TADCSuperLayer *sl);
	void SetMWDCId(int id);
	void SetMotherDCArr(TAMWDCArray *mother){ fMotherDCArr = mother; }
	double Acceptance() const; // detector acceptance
	int GetNFiredAnodePerLayer(int dcType, int layerOption) const;
	int GetSTRid(double k, int dcType) const;

	// coordinate of the anode center of the sense wire. serialId: 0-159
	void AssignAnodePosition();
	virtual void GetAnodeCenterPosition(int dcType, int layerOption, int anodeId, double *Ag) const;
	virtual void GetAnodeGlobalDirection(int dcType, double *ag) const;
	virtual void GetAnodeGlobalProjection(int dcType, const double *globalCenter, double *globalProjection) const;
	TAAnode *GetAnodeL1(int dcType, int anodeId) const;
	TAAnode *GetAnodeL2(int dcType, int anodeId) const;
	TAAnode *GetAnode(int dcType, int anodeSerialId) const;
	TAAnode *GetAnode(int dcType, int layerOption, int anodeId) const;

	virtual void Configure();
	virtual void Initialize() override;
	virtual void Info() const;
	
	static const int kX = 0, kU = 1, kV = 2, kY = 3;
protected:
	vector<TADCSuperLayer *> fSLayerArr; // [0-1-2] -> [X,U,V] or [0-1] -> [X-Y]
	short fMWDCId;
	short fNAnodePerLayer;
	TAMWDCArray *fMotherDCArr;
};

#endif
