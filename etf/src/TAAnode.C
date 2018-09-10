///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAAnode.C														     //
//   TAAnode.C -- source file for class TAAnode									     //
//   Introduction: Drift chamber anode class, representative of the physical	     //
// entity. This is a container class, providing the attributes and data analysis     //
// function of the anode.														     //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/9/24.															     //
// Last modified: 2018/9/8, SUN Yazhou.											     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include "TAAnodePara.h"
#include "TAAnodeData.h"
#include "TAAnode.h"
#include "TAPopMsg.h"
#include "TAUIDParser.h"
#include "TACtrlPara.h"
#include "TAVisual.h"

TAAnode::TAAnode(const string &name, const string &title, unsigned uid)
		: TAStuff(name, title, uid), fPara(nullptr), fData(nullptr){
	fPara = new TAAnodePara(name+"->Para", title+"->Para", uid);
	fData = new TAAnodeData(name+"->Data", title+"->Data", uid);

	fAnodeId = -1;
	
	int type[6]{}; TAUIDParser::DNS(type, GetUID());
	if(0 == type[2]) TAVisual::Instance()->AddAnode(this); // only X anodes would be visualized
}

TAAnode::~TAAnode(){
	if(!fPara){
		delete fPara;
		fPara = nullptr;
	}
	if(!fData){
		delete fData;
		fData = nullptr;
	}
}

// return type is TACh*, which could be cast into TAAnode* if necessary
TAChPara *TAAnode::GetPara() const{
	if(!fPara) TAPopMsg::Error(GetName().c_str(), "GetPara: Para not assgined.");
	return fPara;
}
TAChData *TAAnode::GetData() const{
	if(!fData) TAPopMsg::Error(GetName().c_str(), "GetData: Data not assgined.");
	return fData;
}
short TAAnode::GetAnodeId() const{
	if(-1 == fAnodeId) TAPopMsg::Error(GetName().c_str(), "GetAnodeId: Not assgined.");
	return fAnodeId;
}

void TAAnode::SetAnodeId(int id){
	if(id < 0) TAPopMsg::Error(GetName().c_str(), "SetAnodeId: Input id is minus: %d", id);
	fAnodeId = id;
}
bool TAAnode::GetFiredStatus() const{
	return GetData()->GetFiredStatus();
}
double TAAnode::GetTOT(int n) const{
	const double tl = GetData()->GetLeadingTime(n);
	const double tt = GetData()->GetTrailingTime(n);
	if(-9999. == tt || -9999. == tl) return -9999.; // one of them is not assigned
	return tt - tl;
}
double TAAnode::GetTime(int i){
	double t = GetData()->GetLeadingTime(i);
	if(-9999 == t) return -9999.;
	return t - GetPara()->GetDelay();
}

double TAAnode::GetDriftTime() const{
	double weight;
	return GetDriftTime(weight);
}
double TAAnode::GetDriftTime(double &weight) const{
	if(!GetData()->GetFiredStatus()) TAPopMsg::Error(GetName().c_str(), "GetDriftTime: Not fired");
	const double tof = ((TAAnodeData*)GetData())->GetTOF();
	if(-9999. == tof){ // usually this is for U or V anodes
		return -9999.;
	} // end if
	double driftTime = GetData()->GetLeadingTime() - GetPara()->GetDelay() - tof;
//	cout << "GetData()->GetLeadingTime(): " << GetData()->GetLeadingTime() << endl; // DEBUG
//	cout << "GetPara()->GetDelay(): " << GetPara()->GetDelay() << endl; // DEBUG
//	cout << "tof: " << tof << endl; // DEBUG
//	getchar(); // DEBUG
	// weight: for weighted addition of chi to chi2
	if(TACtrlPara::IsDriftTimeQtCorrection())
		GetAnodePara()->DriftTimeQtCorrection(driftTime, GetTOT(), weight);
	else weight = 1.; // the default weight value
	return driftTime;
} // end of function GetDriftTime()
// for generate simulation data //
double TAAnode::GetDriftTime(double rr, double k){ // k is the track slope
//	rr = 5.2273836; // DEBUG
	double r = rr; // for overflow treatment (drift cell radius = 5 mm)
	if(rr >= 4.999){ r = 4.999; }
	const int n = 100;
	double span = 300.; // search scope, unit: ns
	double t, tc = 60., tm = 0.; // ns
	double d, dmin = 1E200;
	for(int l = 0; l < 3; l++){
//		cout << "___________l>>>>>>: " << l << endl; // DEBUG
		for(int i = 0; i <= n; i++){
			t = tc+(2.*i/n-1.)*span;
			if(t < 0.) continue;
			d = fabs(GetDriftDistance(t, k) - r);
//			if(d - dmin < 1.E-1){ // DEBUG
//				cout.setf(std::ios::fixed); // DEBUG
//				cout << "dmin: " << dmin << "\td: " << d << endl; // DEBUG
//				cout << "d - dmin: " << d - dmin << endl;
//				bool is = bool(d - dmin < -1.E-3);
//				cout <<  "is: " << is << endl; getchar(); // DEBUG
//			} // DEBUG
			if(d - dmin < -1.1E-5){
				dmin = d; tm = t;
//				cout << "\033[36;1mdmin: " << dmin << "\ttm: " << tm << "\n\033[0m"; getchar(); // DEBUG
			} // end if
		} // end for over i
		span *= 2.2/n;
		tc = tm;
	} // end for over l
//	cout << "tm: " << tm << endl; // DEBUG
//	cout << "(298. - tm)*((r-4.999)/2.072): " << (298. - tm)*((rr-4.999)/2.072) << endl; // DEBUG
	if(rr >= 4.999) tm += (298. - tm) * ((rr - 4.999) / 2.072); // 2.072 = 5*sqrt(2)-4.999
//	cout << "tm: " << tm << endl; getchar(); // DEBUG
	return tm;
} // end function GetDriftTime


double TAAnode::GetDriftDistance(double dt, double k){
	int type[6]{}; TAUIDParser::DNS(type, GetUID());
	int id = GetAnodePara()->GetSTRid(k, type[2]);
	return GetDriftDistance(dt, id);
} // end of function GetDriftDistance()
double TAAnode::GetDriftDistance(double dt, int STR_id){ // k is the track slope
	double r_base = GetAnodePara()->GetSTR(STR_id)->Eval(dt); // the base drift distance
	const short detId = GetAnodePara()->GetDetId();
	if(8 == detId || 9 == detId){ // PDC array, large drift cells - 10mm max drift distance
		static const double p[8] = {0.197687, 0.0210968, 0.00048929, -4.14001e-06, 1.47029e-08, -2.68446e-11, 2.44614e-14, -8.70991e-18};
		r_base = p[7]*pow(dt,7)+p[6]*pow(dt,6)+p[5]*pow(dt,5)+p[4]*pow(dt,4)+p[3]*dt*dt*dt+p[2]*dt*dt+p[1]*dt+p[0]; // DEBUG
		if(r_base < 0. || dt < -10.) r_base = 0.; // DEBUG
		if(r_base > 10. || dt > 550.) r_base = 10.; // DEBUG
//		cout << "dt: " << dt << "\tr_base: " << r_base << endl; // DEBUG
//		getchar(); // DEBUG
//		return r_base;
	}
	double r_correct = GetDriftDistanceCorrection(r_base, STR_id); // the anode-specific drift distance corection, determined by STR autocalibration
//	r_correct = GetDriftDistanceCorrection(r_base+r_correct, STR_id); // so the drift distance bin would be more accurate
	double r = r_base + r_correct;
//	cout << "dt: " << dt << "\tr_base: " << r_base << endl; getchar(); // DEBUG
	return r > 0. ? r : 0.;
} // end of function GetDriftDistance
double TAAnode::GetDriftDistanceCorrection(double r, int STR_id) const{
	// range validity check
	// special treatment for PDCs //
	double STRCorRMax = TAAnodePara::kSTRCorRMax;
	const short detId = GetAnodePara()->GetDetId();
	if(8 == detId || 9 == detId){ // PDC array, large drift cells - 10mm max drift distance
		STRCorRMax *= 2.;
	}
	if(r < 0. || r >= STRCorRMax) return 0.;


	const double *pcor = GetAnodePara()->GetSTRCorrection(STR_id);
	int DT = GetAnodePara()->GetDriftDistanceBinNumber(r);
//	if(0)
	if(0 == DT){ // the zero bin is void to avoid bias, so the STR correction value of this bin has to be extrapolated.
		return pcor[1];
	}
	double k = (r - (DT + 0.5) * TAAnodePara::kSTRCorRStep) / TAAnodePara::kSTRCorRStep;
	if(k < 0.) k = 0.;
	// TODO: to be completed with more advanced interpolation method
	// such as (Newton interpolation)
	return pcor[DT] * (1. - k) + pcor[DT + 1] * k;
} // end of function GetDriftDistanceCorrection

void TAAnode::SetChId(int id){
	GetPara()->SetChannelId(id);
}

void TAAnode::Initialize(){
	GetAnodeData()->Initialize();
}


