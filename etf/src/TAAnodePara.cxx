///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																																							     //
// BINGER/inc/etf/TAAnodePara.cxx																										 //
//   TAAnodePara.cxx -- source file for class TAAnodePara														 //
//   Introduction: derived from TAChPara, embodiment of DC anode parameter set.			 //
//																																							     //
// Author: SUN Yazhou, asia.rabbit@163.com.																			     //
// Created: 2017/9/24.																													     //
// Last modified: 2019/10/9, SUN Yazhou.																				     //
//																																							     //
//																																							     //
// Copyright (C) 2017-2018, SUN Yazhou.																					     //
// All rights reserved.																													     //
///////////////////////////////////////////////////////////////////////////////////////

#include "TAAnodePara.h"
#include "TAPopMsg.h"
#include "TAMath.h"
#include "TACtrlPara.h"
#include "TAMWDC.h"
#include "TADetectorPara.h"
#include "TADeployPara.h"
#include "TAUIDParser.h"

const double TAAnodePara::kSTRCorRMax = 6.; ///< unit: mm positive
const double TAAnodePara::kSTRCorAngleMax =
			10. * TAMath::DEGREE(); ///< unit: rad
const double TAAnodePara::kSTRCorRStep =
			TAAnodePara::kSTRCorRMax / TAAnodePara::kSTRCorRNBins;
const double TAAnodePara::kSTRCorAngleStep =
			TAAnodePara::kSTRCorAngleMax * 2. / (TAAnodePara::kSTRCorAngleNBins - 2);
const double TAAnodePara::kSTRCorArrDummy[TAAnodePara::kSTRCorRNBins*3]{0.};
static TACtrlPara *clp = TACtrlPara::Instance();

TAAnodePara::TAAnodePara(const string &name, const string &title, unsigned uid)
		: TAChPara(name, title, uid),
		fGlobalDirection(nullptr), fSTR{0}, fMotherDC(0), fDetId(-1), fDCId(-1){
	for(int i = 0; i < 3; i++){
		fGlobalCenter[i] = -9999.;
		fGlobalProjection[i] = -9999.;
	}
	for(const double *&p : fSTRCorArr) p = kSTRCorArrDummy; // initialized to a zero array
	fDelay = TADeployPara::Instance()->GetMWDCDelay(GetUID());
//	TAPopMsg::Debug(GetName().c_str(), "Just to pop a bubble :)");
	if(kSTRCorAngleNBins %2 != 0) TAPopMsg::Error(GetName().c_str(), "kSTRCorAngleNBins is odd...");
	if(kSTRCorAngleMax < 0) TAPopMsg::Error(GetName().c_str(), "kSTRCorAngleMax is minus...");

	// assign fDetId
	int type[6]{}; TAUIDParser::DNS(type, uid);
	fDetId = type[0];
	fDCId = type[1]; // 0-1-2-3: [DC0-1-2-TOFW, PDC0-1-x-x; DCTa0-1-x-x]
} // end of the constructor
TAAnodePara::~TAAnodePara(){
	for(const double *&p : fSTRCorArr){
		if(kSTRCorArrDummy != p){ // p has been assigned by TAParaManager
			delete p; p = nullptr;
		} // end if
	} // end for
} // end the destructor

double TAAnodePara::GetDelay() const{
	if(0. == fDelay)
		TAPopMsg::Warn(GetName().c_str(), "GetDelay(): Global Delay may have not been assigned.");
	return fDelay;
}
int TAAnodePara::GetDetId() const{
	if(-1 == fDetId) TAPopMsg::Error(GetName().c_str(), "GetDetId(): fDetId not assigned yet~");
	return fDetId;
}
int TAAnodePara::GetDCId() const{
	if(-1 == fDCId) TAPopMsg::Error(GetName().c_str(), "GetDetId(): fDCId not assigned yet~");
	return fDCId;
}
// projection point of the anode to the normal plane
double TAAnodePara::GetProjectionX() const{
	if(-9999. == fGlobalProjection[0])
		TAPopMsg::Error(GetName().c_str(), "GetProjectionX(): ProjectionX has not bee assigned.");
	return fGlobalProjection[0];
}
void TAAnodePara::GetGlobalProjection(double *Ag) const{
	if(nullptr == fGlobalProjection)
		TAPopMsg::Error(GetName().c_str(), "GetGlobalProjection(): GlobalProjection has not been assigned.");
	for(int i = 3; i--;) Ag[i] = fGlobalProjection[i];
}
double TAAnodePara::GetProjectionZ() const{
	if(-9999. == fGlobalProjection[2])
		TAPopMsg::Error(GetName().c_str(), "GetProjectionZ(): ProjectionZ has not bee assigned.");
	return fGlobalProjection[2];
}
void TAAnodePara::GetGlobalCenter(double *Ag) const{
	if(-9999. == fGlobalCenter[0])
		TAPopMsg::Error(GetName().c_str(), "GetGlobalCenter(): GlobalCenter has not bee assigned.");
	for(int i = 3; i--;) Ag[i] = fGlobalCenter[i];
}
// anode orientation vector
void TAAnodePara::GetGlobalDirection(double *ag) const{
	if(nullptr == fGlobalDirection)
		TAPopMsg::Error(GetName().c_str(), "GetGlobalDirection(): GlobalDirection has not bee assigned.");
	for(int i = 3; i--;) ag[i] = fGlobalDirection[i];
}

void TAAnodePara::SetSTR(TF1 *str, int id){
	if(id >= kSTRCorAngleNBins)
		TAPopMsg::Error(GetName().c_str(), "SetSTR: str_id too large: %d", id);
	if(!str) TAPopMsg::Error(GetName().c_str(), "SetSTR: input str is null.");
	if(fSTR[id]) TAPopMsg::Warn(GetName().c_str(), "SetSTR: fSTR has been assigned before. Is this cool?");
	fSTR[id] = str;
}
TF1 *TAAnodePara::GetSTR(int id) const{
	if(id >= kSTRCorAngleNBins)
		TAPopMsg::Error(GetName().c_str(), "GetSTR: str_id too large: %d", id);
	if(!fSTR[id]) TAPopMsg::Error(GetName().c_str(), "GetSTR: fSTR is null.");
	return fSTR[id];
}
TAMWDC *TAAnodePara::GetMotherDC() const{
	if(!fMotherDC) TAPopMsg::Error(GetName().c_str(), "GetMotherDC: Null pointer");
	return fMotherDC;
}
const double *TAAnodePara::GetSTRCorrection(int STR_id) const{
	if(STR_id >= kSTRCorAngleNBins)
		TAPopMsg::Error(GetName().c_str(), "GetSTRCorrection: Input STR_id out of range");
	return fSTRCorArr[STR_id];
}
int TAAnodePara::GetSTRid(double k, int dcType) const{
	int type[6]{}; TAUIDParser::DNS(type, GetUID());

	if( ((3 == type[0] || 4 == type[0]) && (dcType < 0 || dcType > 2)) // DCArrL-R
		|| ((6 == type[0] || 7 == type[0]) && (dcType < 0 || dcType > 1)) // DCArrU-D
		|| ((8 == type[0] || 9 == type[0]) && (dcType < 0 || dcType > 1)) // PDCArrU-D
	){
		TAPopMsg::Error(GetName().c_str(), "GetSTRid: invalid dcType: %d", dcType);
	} // end the legnthy if

	const double phiC = GetMotherDC()->GetDetPara()->GetPhi();
	const double thetaC = GetMotherDC()->GetDetPara()->GetTheta();
	double theta; // the trkproj-drift_cell angle
	if(TAMWDC::kX == dcType) theta = atan(k) - phiC; // X
	else if(TAMWDC::kY == dcType) theta = atan(k) - thetaC; // Y
	else theta = atan(k); // U or V
//	cout << "name: " << GetName() << endl; // DEBUG
//	cout << "theta: " << theta / TAMath::DEGREE() << endl; getchar(); // DEBUG
	return GetSTRid(theta, GetName().c_str());
} // end of function GetSTRid
// alpha: angle between track proj and drift cell; unit: rad
// name: the caller name
int TAAnodePara::GetSTRid(double alpha, const char *name){
	double theta = alpha; // a humdrum value passage ;)
	double thetaStair = -kSTRCorAngleMax; // angle range border stairs
	if(theta < -kSTRCorAngleMax) return 0.;
	int i = 0;
	while(thetaStair <= kSTRCorAngleMax){
		if(theta < thetaStair){
			if(i < kSTRCorAngleNBins) return i;
			else TAPopMsg::Error(name, "GetSTRid: Abnormal id calculated: %d", i);
		}
		thetaStair += kSTRCorAngleStep; i++;
	} // end while
	if(i >= kSTRCorAngleNBins)
		TAPopMsg::Error(name, "GetSTRid: Abnormal id calculated: %d", i);
	return i;
}
int TAAnodePara::GetDriftDistanceBinNumber(double r){
	// special treatment for PDCs //
	double STRCorRMax = kSTRCorRMax;
	const short detId = GetDetId();
	if(8 == detId || 9 == detId){ // PDC array, large drift cells - 10mm max drift distance
		STRCorRMax = kSTRCorRMax * 2.;
	}

	if(r <= kSTRCorRStep / 2.) // no extrapolation before the first point.
		r = kSTRCorRStep / 2. + 0.01;
	if(r >= STRCorRMax - kSTRCorRStep / 2.) // no extrapolation beyond the last point
		r = STRCorRMax - kSTRCorRStep / 2. - 0.01;
	return int(r / kSTRCorRStep - 0.5);
} // end of function GetDriftDistanceBinNumber
void TAAnodePara::SetGlobalCenter(const double *Ag){
	for(int i = 3; i--;) fGlobalCenter[i] = Ag[i];
}
void TAAnodePara::SetGlobalProjection(const double *proj){
	for(int i = 3; i--;) fGlobalProjection[i] = proj[i];
}

void TAAnodePara::SetSTRCorArr(const int *vaBinNumArr,
		const double *strCorArr, int angle_no, int va_bin_cnt){
	if(angle_no >= kSTRCorAngleNBins){
		TAPopMsg::Error(GetName().c_str(), "SetSTRCorArr: angle_no out of range. angle_no: %d",
			 angle_no);
		return;
	}


	// special treatment for PDCs //
	int STRCorRNBins = kSTRCorRNBins;
	const short detId = GetDetId();
	if(8 == detId || 9 == detId){ // PDC array, large drift cells - 10mm max drift distance
		STRCorRNBins = kSTRCorRNBins * 3;
	}
	if(va_bin_cnt > STRCorRNBins){
		TAPopMsg::Error(GetName().c_str(), "SetSTRCorArr: va_bin_cnt out of range. valid_bin_cnt: %d", va_bin_cnt);
		return;
	}
//	cout << "This is TAAnodePara. Period." << endl; getchar(); // DEBUG
//	cout << "va_bin_cnt: " << va_bin_cnt << "\tangle_no: " << angle_no << endl; getchar(); // DEBUG
	if(va_bin_cnt < 5) return; // statistics is too low

	double *p = new double[STRCorRNBins];
	memset(p, 0, STRCorRNBins * sizeof(double));
	for(int i = 0; i < va_bin_cnt; i++){
		if(vaBinNumArr[i] >= STRCorRNBins)
			TAPopMsg::Error(GetName().c_str(),
				"SetSTRCorArr: vaBinNumArr out of range. vaBinNumArr[%d]: %d", i, vaBinNumArr[i]);
		p[vaBinNumArr[i]] = strCorArr[i];
	} // the assignment is complete. :)
	fSTRCorArr[angle_no] = p;
//	for(int i = 0; i < STRCorRNBins; i++){ // DEBUG
//		cout << "fSTRCorArr[angle_no][" << i << "]: " << fSTRCorArr[angle_no][i] << endl; getchar(); // DEBUG
//	} // DEBUG
} // end of function SetSTRCorArr

int TAAnodePara::GetSFE16Id() const{
	static int type[6]{};
	if(0 == type[0]) TAUIDParser::DNS(type, fUID);
	if(3 == type[0] || 4 == type[0] || 6 == type[0] || 7 == type[0] || 8 == type[0] || 9 == type[0])
		return fUID & 0x7FFF; // the 15 LSB
	else TAPopMsg::Error(GetName().c_str(), "GetSFE16Id: this is not from a valid MWDC array");
	return -1;
} // the first 15 LSBs
int TAAnodePara::GetCableId() const{

	static int type[6]{};
	if(0 == type[0]) TAUIDParser::DNS(type, fUID);
	if(3 == type[0] || 4 == type[0] || 6 == type[0] || 7 == type[0] || 8 == type[0] || 9 == type[0])
		return fUID & 0x3FFF; // the 14 LSB
	else TAPopMsg::Error(GetName().c_str(), "GetCableId: this is not from a valid MWDC array");
	return -1;
} // the first 14 LSBs

double TAAnodePara::GetSpatialResolution(double r, double k) const{ // smeared and delayed;  unit: mm
//	cout << "sigmar: " << TACtrlPara::GetSimSpatialResolution() << endl; getchar(); // DEBUG
	if(r < 0.5) return clp->GetSimSpatialResolution(); // .3
	else return clp->GetSimSpatialResolution(); // .2
} // end of member function GetSpatialResolution(...)
void TAAnodePara::DriftTimeQtCorrection(double &driftTime, double TOT, double &weight){
//	TAPopMsg::Debug("TAAnodePara", "DriftTimeQtCorrection: is being called."); // DEBUG
	if(TOT < 250. || TOT > 1000. || driftTime > 300.){
		weight = 1.;
		return; // very probable noise or correction is of little meaning
	}
	if(TOT < 300.){ // the fomula can be invalid
		driftTime += -9.89;
		weight = clp->DriftTimeQtCorrectionWeight() * 0.8; // because the correction for low TOT is relatively not accurate
		return;
	} // end if
	double p0 = 512.125, p1 = 48.5836, p2 = 2.72877; // the fitted parabola TOT-dt relation. unit: ns
	double correction = (-p1 + sqrt(p1*p1 - 4.*p2*(p0-TOT))) / (2.*p2);
	driftTime += correction;
	weight = clp->DriftTimeQtCorrectionWeight();
	return;
} // end of member function DriftTimeQtCorrection(...)
