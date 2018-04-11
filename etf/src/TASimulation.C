///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TASimulation.C													 //
//   TASimulation.C -- source file for class TASimulation							 //
//   Introduction: Simulation data generator, following the format of struct tEntry. //
// A toy Monta Carlo simulator. Only geometrical specifications of detectors are	 //
// considered.																		 //
// 																					 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/18.															     //
// Last modified: 2018/4/4, SUN Yazhou.											     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include <ctime>
#include <cmath>
#include <unistd.h>

// ROOT include
#include "TTree.h"
#include "TFile.h"
#include "TRandom3.h"
#include "TH1F.h"

#include "TASimulation.h"
#include "TACtrlPara.h"
#include "TAT0_1.h"
#include "TAMWDCArray.h"
#include "TAMWDC.h"
#include "TADetectorPara.h"
#include "TAAnode.h"
#include "TAAnodePara.h"
#include "TAChannel.h"
#include "TATOFWall.h"
#include "TAPlaStrip.h"
#include "TAPlaStripPara.h"
#include "TAPopMsg.h"
#include "TAMath.h"

//#define DEBUG

using std::cout;
using std::endl;
using std::flush;
using std::setw;

static const double DEGREE = TAMath::DEGREE();
static const double Pi = TAMath::Pi();
short TASimulation::kFixDCArr = -1;
bool TASimulation::kIsDebug = false;

TASimulation::TASimulation(DetArr_t *detList) : fDetList(detList), fBeta{0.5, 0.68}{
	if(!fDetList) fDetList = &TAParaManager::Instance()->GetDetList();
}
TASimulation::~TASimulation(){}

// generate simualtion data based on the current experiment setup, including
// spacial layout, detector efficiency, and detector structure.
void TASimulation::GenerateSim(int run, int nTrkPerEvEx, double effEx, const string &simFile){
	char rootfile[64];
	GenerateSim(run, nTrkPerEvEx, effEx, rootfile, fDetList, fBeta, simFile);
	fROOTFile = rootfile;
}
// simfile: name of the rootfile containing the simulation data
void TASimulation::GenerateSim(int run, int nTrkPerEvEx, double effEx, char *simFile, DetArr_t *detList, const double *betaEx, const string &simrootfilename){
	const int maxNTrack = nTrkPerEvEx; // number of tracks per event
	const double eff = effEx; // tracking efficiency of a certain anode plane

	string sys_time = TAPopMsg::time0(true); // used in rootfile name
	char rootfile[64]; strcpy(rootfile, "SIM.root"); // (sys_time+"_SIM.root").c_str()
	// use extra sim-rootfile name
	if(strcmp(simrootfilename.c_str(), "")) strcpy(rootfile, simrootfilename.c_str());
	strncpy(simFile, rootfile, sizeof(rootfile));
	TFile *f = new TFile(rootfile, "RECREATE");
	// treeData: raw data; each entry is a data channel
	TTree *treeData = new TTree("treeData", "RAW SIMULATION DATA");
	// treeTrackSim: simulation data, including all the results to be reconstructed
	TTree *treeTrackSim = new TTree("treeTrackSim", "SIMULATION INPUT");
	// data section index, channel id, number of leading or trailing edges
	int index = 0, nTrack, chId, nl = 1, nt = 1;
	double leadingTime[10], trailingTime[10];
	bool is_V; // whether of Very High Resolution mode of HPTDC or not
	bool isDCArrR; // wheter of MWDC array R
	int nu[3][6]; // fired anode serial id X-U-V DC0X1-DC0X2-DC1X1-DC1X2-DC2X1-DC2X2
	// minimal distance between a track and a sense wire
	// d: geometrical; rt: smeared. tt: true drift time
	double d[3][6], rt[3][6], tt[3][6]; // [X-U-V] [DC0X1-DC0X2-DC1X1-DC1X2-DC2X1-DC2X2]
	// b: 3-D track direction vector: (bx, by, bz); B: a point in the track
	// note that B and b are both in global reference
	double B[3]{}, b[3]{}; // bx, by and Bx, By are to be assigned with random nums
	double k1, k2, b1, b2; // x=k1z+b1; y=k2z+b2;
	double beta[2] = {betaEx[0], betaEx[1]}, beta_t; // beta of incident particle [DCArrL-R]
	// -16*25 = -400., so that timeToTrig would within the right range
	int bunchId = -kT0_1TimeToTrigNCycle; // T0_1 starts from 0.
	treeData->Branch("index", &index, "index/I"); // run id
	treeData->Branch("channelId", &chId, "channelId/I");
	treeData->Branch("nl", &nl, "nl/I");
	treeData->Branch("nt", &nt, "nt/I");
	treeData->Branch("leadingTime", leadingTime, "leadingTime[nl]/D");
	treeData->Branch("trailingTime", trailingTime, "trailingTime[nt]/D");
	treeData->Branch("is_V", &is_V, "is_V/O");
	treeData->Branch("bunchId", &bunchId, "bunchId/I");
	treeTrackSim->Branch("index", &index, "index/I"); // run id, a constant within a run
	treeTrackSim->Branch("nTrack", &nTrack, "nTrack/I"); // number of tracks per event
	treeTrackSim->Branch("isDCArrR", &isDCArrR, "isDCArrR/O"); // number of tracks per event
	treeTrackSim->Branch("beta", &beta_t, "beta/D"); // particle speed
	treeTrackSim->Branch("B", B, "B[3]/D");
	treeTrackSim->Branch("b", b, "b[3]/D");
	treeTrackSim->Branch("k1", &k1, "k1/D");
	treeTrackSim->Branch("k2", &k2, "k2/D");
	treeTrackSim->Branch("b1", &b1, "b1/D");
	treeTrackSim->Branch("b2", &b2, "b2/D");
	treeTrackSim->Branch("nu", nu, "nu[3][6]/I");
	treeTrackSim->Branch("d", d, "d[3][6]/D");
	treeTrackSim->Branch("rt", rt, "rt[3][6]/D");
	treeTrackSim->Branch("tt", tt, "tt[3][6]/D");

	// ooooOOOOoooo ooooOOOOoooo    GENERATE SIMULATION DATA    ooooOOOOoooo ooooOOOOoooo //
	TAMWDCArray *dcArrL = (TAMWDCArray*)(*detList)[3];
	TAMWDCArray *dcArrR = (TAMWDCArray*)(*detList)[4];
	TAT0_1 *T0_1 = (TAT0_1*)(*detList)[1]; // to provide time reference
	if(!dcArrL && !dcArrR)
		TAPopMsg::Error("TASimulation", "GenerateSim: Both MWDC arrays are null. TAEventProcessor::Configure() not run yet?");
	// number of tracks in an MWDC array and linear parameters
	// for each track are all randomly generated.
	TRandom3 rdm; rdm.SetSeed(::time(0)); // so that the random series would be different at each run
//	rdm.SetSeed(0);//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
	// DC efficiencies, each value is for a sense wire layer
	const double efficiencyT[2][3][6] = { // [L-R], [X-U-V], [DC0X1-DC0X2-DC1X1-DC1X2-DC2X1-DC2X2]
		{ {eff, eff, eff, eff, eff, eff}, // X
		  {eff, eff, eff, eff, eff, eff}, // U
		  {eff, eff, eff, eff, eff, eff} }, // V

		{ {eff, eff, eff, eff, eff, eff}, // X
		  {eff, eff, eff, eff, eff, eff}, // U
		  {eff, eff, eff, eff, eff, eff} } // V
	};
	int totalTrackCnt = 0, failCnt = 0;
	bool isValid = false; // isValid: if the track pass through all the active area.
	TAPopMsg::Info("TASimulation", "GenerateSim: %d events would be simulated. The simulation data would be stored in created rootfile %s", run, rootfile);
	int ntrkTL = 0, ntrkTR = 0;
	while(index < run){
//		nTrack = int(rdm.Uniform(maxNTrack) + 1.); // 1,2,...,maxNTrack uniform distribution
		nTrack = maxNTrack;
		for(int i = 0; i < nTrack; i++){ // loop over tracks
			// select MWDC array; the initial settings are for MWDC array R
			TAMWDCArray *dcArr = dcArrR; isDCArrR = true;
			const double (*efficiency)[6] = efficiencyT[1];
			short dcArrOpt = short(rdm.Uniform(1.)+0.5); // fifty-fifty
			if(0 == kFixDCArr) dcArrOpt = 0; // MWDC array L
			if(1 == kFixDCArr) dcArrOpt = 1; // MWDC array R
			if(0 == dcArrOpt){ // MWDC array L
				dcArr = dcArrL; isDCArrR = false;
				efficiency = efficiencyT[0];
			}
			beta_t = beta[dcArrOpt];
			if(!dcArr) continue;
			// average phi of the three MWDCs for an MWDC array
			const double phiAvrg = dcArr->GetPhiAvrg();
			TATOFWall *tofw = dcArr->GetTOFWall();

			isValid = true; totalTrackCnt++;
			for(int i = 0; i < 3; i++) for(int j = 0; j < 6; j++){
				d[i][j] = 1E200; nu[i][j] = -1;
			}
			// +-10 + 90 degrees, track v.s. x-axis and y-axis
			double thetaX = Pi / 2. - (-1. + rdm.Uniform(2.)) * 15. * DEGREE - phiAvrg;
			double thetaY = Pi / 2. - (-1. + rdm.Uniform(2.)) * 15. * DEGREE;
			double cosX = cos(thetaX), cosY = cos(thetaY);
			double cosZ = sqrt(1.-cosX*cosX-cosY*cosY);
			b[0] = cosX; b[1] = cosY; b[2] = cosZ;
			double B_local[3]; // local coordinates of a point in DC1-X1
			B_local[0] = (-400. + 2. * rdm.Uniform(400.)) * 0.7; // local X
			B_local[1] = (-300. + 2. * rdm.Uniform(300.)) * 0.7; // local Y
			B_local[2] = 0.; // local Z - X1 sense wire plane
			// local coordinates -> global coordinates
			dcArr->GetMWDC(1)->GetDetPara()->GetGlobalPosition(B_local, B);
			k1 = b[0]/b[2]; b1 = B[0] - k1*B[2]; k2 = b[1]/b[2]; b2 = B[1] - k2*B[2];
			// So, the 3-D track has been randomly sampled.
			// ---------------------------------------------------------------------- //
			// only a little math would have to be done to generate the simulation data
			double a_tmp[3][6][3], A_tmp[3][6][3]; // DEBUG
			for(int j = 0; j < 3; j++){ // loop over MWDCs
				TAMWDC *dc = dcArr->GetMWDC(j);
				for(int k = 0; k < 6; k++){ // loop over X1-X2-U1-U2-V1-V2
					// type: X-U-V layer: DC0X1-DC0X2-DC1X1-DC1X2-DC2X1-DC2X2
					int type = k/2, layer = 2*j+k%2;
					double ag[3]; dc->GetAnodeGlobalDirection(type, ag);
					const int nAnodePerLayer = dc->GetNAnodePerLayer();
					for(int l = 0; l < nAnodePerLayer; l++){ // loop over anodes per layer
						double Ag[3];
						dc->GetAnode(type, k%2+1, l)->GetAnodePara()->GetGlobalCenter(Ag);
						// (B->Ag).(ag×b)/|ag×b|, d of two skew 3-D lines
						double d_t = TAMath::dSkew(Ag, ag, b, B);
						if(d_t < d[type][layer]){
							d[type][layer] = d_t; nu[type][layer] = l;
							if(kIsDebug){ // DEBUG
								memcpy(a_tmp[type][layer], ag, sizeof(ag)); // DEBUG
								memcpy(A_tmp[type][layer], Ag, sizeof(Ag)); // DEBUG
							} // end if // DEBUG
						}
					} // end for over l
					if(rdm.Uniform() > efficiency[type][layer]){ // hit missed due to efficiency
						d[type][layer] = -9999.; nu[type][layer] = -1;
					}
				} // end for over k
			} // end for over j
			// check track validity
			for(int j = 0; j < 3; j++) // loop over X-U-V
			for(int k = 0; k < 6; k++) // loop over DC0X1-DC0X2-DC1X1-DC1X2-DC2X1-DC2X2
				if(d[j][k] > 5.*sqrt(2.)) isValid = false;
			if(!isValid){ i--; failCnt++; continue; } // choose random track again

			// fill tree treeSim //
			// determine fired TOF wall strip, x = kz+b
			double p[3], dmin = 1E200;
			int firedStripId = -1;
			for(int j = 0; j < 30; j++){
				tofw->GetStripProjection(j, p);
				double d_t = fabs(k1*p[2]-p[0]+b1)/sqrt(1.+k1*k1);
				if(d_t < dmin){
					dmin = d_t; firedStripId = j;
				}
			} // end for over strip Id
			static double stripWidth = tofw->GetStrip(0)->GetStripPara()->GetWidth();
			if(dmin > 0.5*stripWidth) isValid = false; // missed the TOF wall
			if(!isValid){ i--; failCnt++; continue; } // choose random track again
			if(0 == dcArrOpt) ntrkTL++;
			if(1 == dcArrOpt) ntrkTR++;
			// firedStripId/8: PXI module #; j%8: strip # in certain module
			// one strip occupies 4 channels. 0-1-2-3: UV-UH-DV-DH
			// fill the fired TOFWall strip, a series of temporary variables are defined
			TAPlaStrip *str = tofw->GetStrip(firedStripId);
			// 3.8961039 = 1200/(2.*veff) - scintillation transmission time veff: 1200./7.8
			const double delay = tofw->GetDelay(firedStripId) + 3.9;
			const double TOT_t[4] = {500., 2000., 500., 2000.}, preT_t[4] = {0., 100., 0., 100.};
			const bool isV_t[4] = {true, false, true, false};
			TAChannel *ch_t[4] = {str->GetUV(), str->GetUH(), str->GetDV(), str->GetDH()};
			for(int l = 0; l < 4; l++){ // loop over UV-UH-DV-DH
				leadingTime[0] = preT_t[l] + delay + ch_t[l]->GetPara()->GetDelay(); is_V = isV_t[l];
				trailingTime[0] = leadingTime[0] + TOT_t[l]; // 500: TOT
				chId = ch_t[l]->GetPara()->GetChannelId(); treeData->Fill();
			}

			// to prepare STRid for the drift time extraction
			double kl[3]; // slope of track projections [012]: [XUV]
			kl[0] = k1; // X
			kl[1] = TAMath::kXY_U(phiAvrg, k1, k2); // U
			kl[2] = TAMath::kXY_V(phiAvrg, k1, k2); // V
			// get drift time //
			for(int j = 0; j < 3; j++){ // loop over X-U-V
			for(int k = 0; k < 6; k++){ // loop over DC0X1-DC0X2-DC1X1-DC1X2-DC2X1-DC2X2
			if(nu[j][k] >= 0){ // nu[][] < 0 is only due to efficiency
				TAAnode *ano = dcArr->GetMWDC(k/2)->GetAnode(j, k%2+1, nu[j][k]);
				TAAnodePara *anoPar = (TAAnodePara*)ano->GetPara();
				chId = anoPar->GetChannelId();
				double s_t = anoPar->GetSpatialResolution(d[j][k], kl[j]);
				double r_t = rdm.Gaus(d[j][k], s_t);
				double T_drift = ano->GetDriftTime(r_t, kl[j]); // b[0]/b[2]: track projection slope
				rt[j][k] = r_t; tt[j][k] = T_drift;
				double T_wire = dcArr->GetWirePropagationTime(b, B, nu[j][k], j, k);
				// from the anode to TOF Wall
				double T_tof = dcArr->GetTimeOfFlight(b, B, nu[j][k], j, k, firedStripId, beta_t);
				double T0 = anoPar->GetDelay();
//				cout << "anoPar->GetDelay(): " << anoPar->GetDelay(); getchar(); // DEBUG
				leadingTime[0] = T_drift + T_wire - T_tof + T0;
				if(kIsDebug){ // DEBUG
					cout << "\033[33;1m" << ano->GetName() << "\033[0m\n"; // DEBUG
					cout << "beta_t: " << beta_t << endl; // DEBUG
					cout << "nu: " << nu[j][k] << "\tchId: " << chId << endl; // DEBUG
					cout << "b[0]: " << b[0] << "\tb[1]: " << b[1] << "\tb[2]: " << b[2] << endl; // DEBUG
					cout << "B[0]: " << B[0] << "\tB[1]: " << B[1] << "\tB[2]: " << B[2] << endl; // DEBUG
					double *aa = a_tmp[j][k], *AA = A_tmp[j][k]; // DEBUG
					cout << "a[0]: " << aa[0] << "\ta[1]: " << aa[1] << "\ta[2]: " << aa[2] << endl; // DEBUG
					cout << "A[0]: " << AA[0] << "\tA[1]: " << AA[1] << "\tA[2]: " << AA[2] << endl; // DEBUG
					double wire_length = dcArr->GetWirePropagationLength(b, B, nu[j][k], j, k); // DEBUG
					double flight_length = dcArr->GetDistanceOfFlight(b, B, nu[j][k], j, k, firedStripId); // DEBUG
					cout << "r_t: " << r_t << "\td[j][k]: " << d[j][k] << endl; // DEBUG
					cout << "wire_length of propagation: " << wire_length << endl; // DEBUG
					cout << "flight_length: " << flight_length << endl; // DEBUG
					cout << "T0: " << T0 << "\tleadingTime[0]: " << leadingTime[0] << endl; // DEBUG
					cout << "T_drift: " << T_drift << "\tT_wire: " << T_wire << "\tT_tof: " << T_tof << endl; // DEBUG
					cout << "T_drift + T_wire - T_tof: " << T_drift + T_wire - T_tof << endl; // DEBUG
					getchar(); // DEBUG
				} // DEBUG
				trailingTime[0] = leadingTime[0] + 300.;
				is_V = false;
				treeData->Fill();
			} // end fill loop
			} // end for over k
			} // end for over j
			// one particle trajectory has been tracked down
			treeTrackSim->Fill(); // store the input track information
		} // end for over i (loop over tracks)
		// * prepare T0_1 channel to provide global time reference * //
		const double delay = T0_1->GetDelay();
		double TOT_t[4] = {500., 2000., 500., 2000.}, preT_t[4] = {0., 100., 0., 100.};
		bool isV_t[4] = {true, false, true, false};
		TAChannel *ch_t[4] = {T0_1->GetUV(), T0_1->GetUH(), T0_1->GetDV(), T0_1->GetDH()};
		for(int l = 0; l < 4; l++){ // loop over UV-UH-DV-DH
			leadingTime[0] = preT_t[l] + delay + ch_t[l]->GetPara()->GetDelay(); is_V = isV_t[l];
			trailingTime[0] = leadingTime[0] + TOT_t[l]; // 500: TOT
			chId = ch_t[l]->GetPara()->GetChannelId(); treeData->Fill();
		}

		// complete the data section by filling the ending entry
		int temp = index; nl = 0; nt = 0;
		index = -2; chId = nTrack * 40; // section length, word(32bit)
		treeData->Fill();
		index = temp; nl = 1; nt = 1;
		index++; // one run is completed.
		cout << "run " << index << " completed.\r" << flush;
	} // end while
	cout << "\n\n";
	cout << "\033[1m" << totalTrackCnt << "\033[0m tracks have been sampled, \033[1m" << failCnt << "\033[0m attempts failed (out of detecting area).\n";
	cout << "\033[33;1mntrkTL: " << ntrkTL << "\tntrkTR: " << ntrkTR << "\033[0m\n";
	cout << "\033[1m" << totalTrackCnt - failCnt << "\033[0m tracks are saved in treeData.\n";
	treeData->Write("", TObject::kOverwrite);
	treeTrackSim->Write("", TObject::kOverwrite);
	delete treeTrackSim; delete treeData;

	f->Close(); delete f;
	cout << "\033[32;1mSIMULATION COMPELETE. Data has been stored in " << rootfile << "\033[0m\n\n";
} // end of member function GenerateSim



// evalute and print the data analysis result
void TASimulation::Evaluate(){
	Evaluate(fROOTFile);
}

//#define DEBUG

void TASimulation::Evaluate(const string &rootfile){
	TAPopMsg::Info("TASimulation", "Evaluate: rootfile to be evaluted is >> %s", rootfile.c_str());
	if(0 != access(rootfile.c_str(), F_OK))
		TAPopMsg::Error("TASimulation", "Evaluate: Input rootfile %s doesn't exist", rootfile.c_str());
	TFile *f = new TFile(rootfile.c_str(), "UPDATE");
	if(!f->FindObjectAny("treeTrackSim"))
		TAPopMsg::Error("TASimulation", "Evaluate: treeTrackSim is not found in the rootfile");
	if(!f->FindObjectAny("treeTrack"))
		TAPopMsg::Error("TASimulation", "Evaluate: treeTrack is not found in the rootfile");
	TTree *treeTrackSim = (TTree*)f->Get("treeTrackSim");
	TTree *treeTrack = (TTree*)f->Get("treeTrack");

	int nTrkPerEv = 0, nTrack; // nTrk per event; number of tracks for a certain event
	int ntr = 0; // number of track projections in a data section(3*3D track)
	int index, indexSim = 0, indexSimPre = indexSim; // data section index
	const int ntrMax = 100, ntrMax3D = ntrMax / 3;

	int nuSim[3][6], nu[ntrMax][6], nuSimAr[ntrMax3D][3][6];
	bool isDCArrR, isDCArrRSimAr[ntrMax3D]{}; // wheter of MWDC array R
	treeTrackSim->SetBranchAddress("index", &indexSim); // run id, a constant within a run
	treeTrackSim->SetBranchAddress("nTrack", &nTrack); // run id, a constant within a run
	treeTrackSim->SetBranchAddress("isDCArrR", &isDCArrR); // wheter of MWDC array R
	treeTrackSim->SetBranchAddress("nu", nuSim);

	int type[ntrMax], id[ntrMax];
	int n3Dtr, n3DtrSim, n3DtrTot = 0;
	treeTrack->SetBranchAddress("index", &index);
	treeTrack->SetBranchAddress("ntr", &ntr);
	treeTrack->SetBranchAddress("nu", nu);
	treeTrack->SetBranchAddress("type", type); // track type: 1[LR][XUV]
	treeTrack->SetBranchAddress("id", id); // 3-D track id
	// five kind of tracks, explained in the book Data Analysis... p134.
	int hitTypeCntX[6]{}, hitTypeCntU[6]{}, hitTypeCntV[6]{}, hitTypeCnt3D[6]{};
	int hitType[ntrMax3D][3], hitType3D[ntrMax3D]; // X-U-V

	TH1F *h00 = new TH1F("h00", "hitTypeX (0~5)", 8, -1.5, 6.5);
	TH1F *h01 = new TH1F("h01", "hitTypeU (0~5)", 8, -1.5, 6.5);
	TH1F *h02 = new TH1F("h02", "hitTypeV (0~5)", 8, -1.5, 6.5);
	TH1F *h03 = new TH1F("h03", "hitType3D (0~5)", 8, -1.5, 6.5);
	TTree *treeEff = new TTree("treeEff", "Tracking efficiency");
	treeEff->Branch("index", &index, "index/I"); // event index, or data section index
	treeEff->Branch("n3Dtr", &n3Dtr, "n3Dtr/I"); // number of 3d tracks
	treeEff->Branch("n3DtrSim", &n3DtrSim, "n3DtrSim/I"); // number of 3d tracks perEv in simualtion
	treeEff->Branch("hitType", hitType, "hitType[n3Dtr][3]/I"); // X-U-V
	treeEff->Branch("hitType3D", hitType3D, "hitType3D[n3Dtr]/I");

	const int n = treeTrack->GetEntries(); // number of data sections
	int n3DtrXUV[3]{}, trkId[ntrMax3D][3]; // track id [3D track id][XUV];
	cout << "Totally " << n << " data sections\n";
	const int nSim = treeTrackSim->GetEntries(); // number of total 3D tracks
	int iSim = 1; int typeMinusCnt[3]{}; // count of ungrouped tracks
	int cntEXproj = 0, cntCXproj = 0; // EXProj: Excellent (Perfect), CXproj: Common
	for(int i = 0; i < n; i++){
		// Get all tracks in a data section SIM //
		n3DtrSim = 0; iSim--;
		while(iSim < nSim){
			indexSimPre = indexSim;
			if(iSim < nSim) treeTrackSim->GetEntry(iSim++);
			else indexSim = -1; // the end of the tree
			if(indexSim != indexSimPre) break;
			memcpy(nuSimAr[n3DtrSim], nuSim, sizeof(nuSim));
			isDCArrRSimAr[n3DtrSim] = isDCArrR;
			n3DtrSim++;
		}
		// Get all tracks proj in a data section PATTERN //
		treeTrack->GetEntry(i);
		if(index != indexSimPre){
			TAPopMsg::Error("TASimulation", "Evaluate: Sim disagrees with Pattern Recognition in event index: i: %d, index: %d, indexSimPre: %d", i, index, indexSimPre);
		}
		// identify 3-D tracks
		memset(n3DtrXUV, 0, sizeof(n3DtrXUV));
		memset(trkId, -1, sizeof(trkId));
		// loop over grouped track projections
		for(int j = 0; j < ntr; j++) if(-1 != id[j]){
			for(int k = 0; k < 3; k++){ // loop over X-U-V track types
				if(type[j]%10 == k){
					trkId[id[j]][k] = j;
					n3DtrXUV[k]++;
				}
			} // end loop over X-U-V track types
		} // end for over j and if
		else typeMinusCnt[type[j]%10]++;
		if(n3DtrXUV[0] != n3DtrXUV[1] || n3DtrXUV[0] != n3DtrXUV[2]){
			TAPopMsg::Error("TASimulation", "Evaluate: This is odd... track projections of X, U and V are not consistent: n3DtrX: %d, n3DtrU: %d, n3DtrV: %d", n3DtrXUV[0], n3DtrXUV[1], n3DtrXUV[2]);
		} // end if
		n3Dtr = n3DtrXUV[0]; n3DtrTot += n3Dtr;
#ifdef DEBUG
		if(12 <= index){ // DEBUG
			cout << "\nntr: " << ntr << "\tn3DtrSim: " << n3DtrSim << endl; // DEBUG
			cout << "n3Dtr: " << n3Dtr << "\tn3DtrTot: " << n3DtrTot << endl; getchar(); // DEBUG
		} // DEBUG
#endif
		// check purity of Xproj
		for(int j = 0; j< ntr; j++){
			if(0 != type[j]%10) continue;
			// the optimal set for the current 3D Trk
			int nCoinM = 0; // coin: coincide
			int scorem = -1; // optimal store to estimate track overlap
			bool isDCArrR_ = bool((type[j]/10)%10);
			for(int k = 0; k < n3DtrSim; k++){ // loop over simulated 3-D tracks
				if(isDCArrR_ != isDCArrRSimAr[k]) continue; // not in the same MWDC array
				int score = 0; // score marking track overlap
				// number of fired anode layers per track X
				int nFSim = 0, nFPat = 0, nFCoin = 0, nCoin = 0; // Simulation, patReg; Coincidence
				for(int l = 0; l < 6; l++){
					if(-1 != nuSimAr[k][0][l]) nFSim++;
					if(-1 != nu[j][l]) nFPat++;
					if(nu[j][l] == nuSimAr[k][0][l]){
						nCoin++;
						if(-1 != nu[j][l]) nFCoin++;
					}
				} // end for over DC0-X1X2-DC0-X1X2-DC1-X1X2-DC2-X1X2
				score = nFCoin;
				// select the best match in simulation for Trk_pat
				if(score > scorem){
					scorem = score;
					nCoinM = nCoin;
				} // end if(score > scorem)
			} // end for over Sim tracks
			cntCXproj++;
			if(6 == nCoinM) cntEXproj++; // -> Reconstructable
#ifdef DEBUG
			if(12 <= index){ // DEBUG
				cout << "cntCXproj: " << cntCXproj << "\tcntEXproj: " << cntEXproj << endl; // DEBUG
				cout << "scorem: " << scorem << "\tisDCArrR_: " << isDCArrR_ << endl; // DEBUG
				getchar(); // DEBUG
			} // DEBUG
#endif
		} // end for over TrkProjs
		for(int j = 0; j < n3Dtr; j++){
			// assign coin and dif
			// optimal number of fired anode layers per track; Simulation, patReg; Coincidence; [XUV]
			int nCoinM[3]{};
			int nFSimM[3]{}, nFPatM[3]{}, nFCoinM[3]{}; // Sim, patReg; Coin, FCoin
			int scorem = -1; // optimal store to estimate track overlap
			bool isDCArrR_ = bool((type[trkId[j][0]]/10)%10);
			for(int k = 0; k < n3DtrSim; k++){ // loop over simulated 3-D tracks
//				if(12 <= index){ // DEBUG
//					cout << "index: " << index << endl; // DEBUG
//					cout << "trkId[j][0]: " << trkId[j][0] << endl; // DEBUG
//					cout << "trkId[j][1]: " << trkId[j][1] << endl; // DEBUG
//					cout << "trkId[j][2]: " << trkId[j][2] << endl; // DEBUG
//					cout << "type[trkId[j][0]]: " << type[trkId[j][0]] << endl; // DEBUG
//					cout << "isDCArrRSimAr[j]: " << isDCArrRSimAr[j] << "\tisDCArrR_: " << isDCArrR_ << endl; // DEBUG
//					getchar(); // DEBUG
//				} // DEBUG
				if(isDCArrR_ != isDCArrRSimAr[k]) continue; // not in the same MWDC array
				int score = 0; // score marking track overlap
				// number of fired anode layers per track [XUV]
				int nFSim[3]{}, nFPat[3]{}, nFCoin[3]{}, nCoin[3]{}; // Sim, patReg; Coin, FCoin
				for(int m = 0; m < 3; m++){ // loop over X-U-V
					for(int l = 0; l < 6; l++){ // loop over six anode layers over three DCs
						if(-1 != nuSimAr[k][m][l]) nFSim[m]++;
#ifdef DEBUG
						if(12 <= index){ // DEBUG
							cout << "nu[trkId[j][m]][l]: " << nu[trkId[j][m]][l]; // DEBUG
							cout << "\tnuSimAr[k][m][l]: " << nuSimAr[k][m][l] << endl; // DEBUG
						} // DEBUG
#endif
						if(-1 != nu[trkId[j][m]][l]) nFPat[m]++;
						if(nu[trkId[j][m]][l] == nuSimAr[k][m][l]){
							nCoin[m]++;
							if(-1 != nu[trkId[j][m]][l]) nFCoin[m]++;
						}
					} // end loop over l
					score += nFCoin[m];
				} // end loop over m
#ifdef DEBUG
				if(12 <= index){ // DEBUG
					cout << "\033[33;1m________ index: "<< index; // DEBUG
					cout << "\tj: " << j << "\tk: " << k << "\033[0m\n"; // DEBUG
					for(int k = 0; k < 3; k++){ // DEBUG
						cout << "nFSim["<< k << "]: " << nFSim[k] << endl; // DEBUG
						cout << "nFPat["<< k << "]: " << nFPat[k] << endl; // DEBUG
						cout << "nCoin["<< k << "]: " << nCoin[k] << endl; // DEBUG
						cout << "nFCoin["<< k << "]: " << nFCoin[k] << endl; // DEBUG
					} // DEBUG
				} // DEBUG
				if(12 <= index){ // DEBUG
					cout << "score: " << score << "\tscorem: " << scorem << endl; // DEBUG
					getchar(); // DEBUG
				} // DEBUG
#endif
				// select the best match in simulation for Trk_pat
				if(score > scorem){
					scorem = score;
					memcpy(nFSimM, nFSim, sizeof(nFSimM));
					memcpy(nFPatM, nFPat, sizeof(nFPatM));
					memcpy(nCoinM, nCoin, sizeof(nCoinM));
					memcpy(nFCoinM, nFCoin, sizeof(nFCoinM));
				}
			} // end loop over simulated 3-D tracks
#ifdef DEBUG
			if(12 <= index){ // DEBUG
				for(int k = 0; k < 3; k++){ // DEBUG
					cout << "nFSimM["<< k << "]: " << nFSimM[k] << endl; // DEBUG
					cout << "nFPatM["<< k << "]: " << nFPatM[k] << endl; // DEBUG
					cout << "nCoinM["<< k << "]: " << nCoinM[k] << endl; // DEBUG
					cout << "nFCoinM["<< k << "]: " << nFCoinM[k] << endl; // DEBUG
				} // DEBUG
				getchar(); // DEBUG
			} // DEBUG
#endif
			// assign hitType
			memset(hitType[j], -1, sizeof(hitType[j]));
			for(int k = 0; k < 3; k++){ // loop over X-U-V
#ifdef DEBUG
				if(nFCoinM[k] == nFPatM[k] && nFPatM[k] == nFSimM[k] && nCoinM[k] != 6){ // DEBUG
					cout << "\n\nindex: " << index << endl; // DEBUG
					cout << "nFCoinM[k]: " << nFCoinM[k] << "\tnFPatM[k]: " << nFPatM[k] << endl; // DEBUG
					getchar(); // DEBUG
				} // DEBUG
#endif
				if(6 == nCoinM[k]) // -> Reconstructable
					hitType[j][k] = 1; // no false hit in Trk_pat, perfectly recognized
				if(nFCoinM[k] == nFPatM[k] && nFPatM[k] < nFSimM[k]) // -> Reconstructable
					hitType[j][k] = 2; // no false hit in Trk_pat, partly recognized
				if(nFCoinM[k] < nFPatM[k] && nFCoinM[k] >= 0.6*nFSimM[k]) // rough overlap
					hitType[j][k] = 3; // has false hit in Trk_pat, but still abreast
				// has same hit
				if(nFCoinM[k] < nFPatM[k] && nFCoinM[k] < 0.6*nFSimM[k] && nFCoinM[k] >=1)
					hitType[j][k] = 4; // has false hit in Trk_pat, usually not reconstructable
				if(nFCoinM[k] < nFPatM[k] && 0 == nFCoinM[k]) // totally wrong
					hitType[j][k] = 5; // irrelevant to the sim track
#ifdef DEBUG
				if(5 == hitType[j][k]){ // DEBUG
					cout << "\nindex: " << index << endl; getchar(); // DEBUG
				} // DEBUG
#endif
			} // end loop over XUV
#ifdef DEBUG
			if(12 <= index){ // DEBUG
				cout << "\x1b[36;1m__________^&*#$%^&*$%&*!#^$&*__________\x1b[0m" << endl; // DEBUG
				cout << "hitType[j][0]: " << hitType[j][0] << endl; // DEBUG
				cout << "hitType[j][1]: " << hitType[j][1] << endl; // DEBUG
				cout << "hitType[j][2]: " << hitType[j][2] << endl; getchar(); // DEBUG
			} // DEBUG
#endif
			// any type of tracks messes, the 3D track resconstruction would not succeed
			if(hitType[j][0] == -1 || hitType[j][1] == -1 || hitType[j][2] == -1)
				hitType3D[j] = -1;
			else if(hitType[j][0] == 1 && hitType[j][1] == 1 && hitType[j][2] == 1)
				hitType3D[j] = 1;
			else if(hitType[j][0] <= 2 && hitType[j][1] <= 2 && hitType[j][2] <= 2)
				hitType3D[j] = 2;
			else if(hitType[j][0] <= 3 && hitType[j][1] <= 3 && hitType[j][2] <= 3)
				hitType3D[j] = 3;
			else if(hitType[j][0] == 5 && hitType[j][1] == 5 && hitType[j][2] == 5)
				hitType3D[j] = 5;
			else hitType3D[j] = 4; // evaluation of one 3D track is complete.
			hitTypeCntX[hitType[j][0]]++;
			hitTypeCntU[hitType[j][1]]++;
			hitTypeCntV[hitType[j][2]]++;
			hitTypeCnt3D[hitType3D[j]]++;
			h00->Fill(hitType[j][0]);
			h01->Fill(hitType[j][1]);
			h02->Fill(hitType[j][2]);
			h03->Fill(hitType3D[j]);
		} // end for over j(3D tracks)
		cout << "Index " << i << " processed.\r" << flush;
		treeEff->Fill();
		nTrkPerEv += n3DtrSim;
	} // end for over i
	nTrkPerEv /= n;

	cout << "\n\n\033[31;1m________________\033[32;1m";
	cout << "PROGRAM EVALUTATION RESULT DISPLAY";
	cout << "\033[31;1m________________\033[0m\n\n";
	cout << "Track Count:\n";
	cout << "Track Type 1 - Perfectly Recognized;\n";
	cout << "Track Type 2 - Imcomplete Pattern;\n";
	cout << "Track Type 3 - Flawed Pattern;\n";
	cout << "Track Type 4 - Damaged Pattern;\n";
	cout << "Track Type 5 - All Miss.\n";
	cout << "Type   " << setw(9) << 1 << setw(9) << 2 << setw(9) << 3 << setw(9) << 4 << setw(9) << 5 << endl;
	cout << " X     \033[32;1m";
	for(int i = 1; i < 6; i++)
		cout << setw(9) << hitTypeCntX[i];
	cout << "\033[0m\n U     \033[32;1m";
	for(int i = 1; i < 6; i++)
		cout << setw(9) << hitTypeCntU[i];
	cout << "\033[0m\n V     \033[32;1m";
	for(int i = 1; i < 6; i++)
		cout << setw(9) << hitTypeCntV[i];
	cout << "\033[0m\n 3D    \033[32;1m";
	for(int i = 1; i < 6; i++)
		cout << setw(9) << hitTypeCnt3D[i];
	cout << "\033[0m\n";
	cout << "<f>: " << double(hitTypeCnt3D[1] + hitTypeCnt3D[2]) / n << endl;
	double eff = double(hitTypeCnt3D[1] + hitTypeCnt3D[2]) / n / nTrkPerEv;
	cout << "Efficiency: <f>/<n>: " << eff << endl;
	cout << "n3DtrTot: " << n3DtrTot << endl;
	cout << "Purity: " << double(hitTypeCnt3D[1]) / n3DtrTot << endl;
	cout << "PurityX: " << double(hitTypeCntX[1]) / n3DtrTot << endl;
	cout << "PurityU: " << double(hitTypeCntU[1]) / n3DtrTot << endl;
	cout << "PurityV: " << double(hitTypeCntV[1]) / n3DtrTot << endl;
	cout << "Ungrouped tracks count : " << endl;
	cout << "typeMinusCntX: " << typeMinusCnt[0];
	cout << "\ttypeMinusCntU: " << typeMinusCnt[1];
	cout << "\ttypeMinusCntV: " << typeMinusCnt[2] << endl;
	cout << "n3DtrTot: " << n3DtrTot << endl;
	cout << "cntEXproj: " << cntEXproj << "\tcntCXproj: " << cntCXproj << endl;
	cout << "\033[36;1mEfficiency: <f>/<n>: " << eff << "\033[0m\n";
	cout << "\033[36;1m3D Purity: " << double(hitTypeCnt3D[1]) / n3DtrTot << "\033[0m\n";
	cout << "\033[36;1mXproj Purity: " << double(cntEXproj) / cntCXproj << "\033[0m\n";
	cout << "\n\n\033[33;1mDONE\033[0m\n\n";

	const char dir[] = "EvalPatReg";
	if(!f->FindObjectAny(dir)) f->mkdir(dir);
	f->cd(dir);
	h00->Write("", TObject::kOverwrite);
	h01->Write("", TObject::kOverwrite);
	h02->Write("", TObject::kOverwrite);
	h03->Write("", TObject::kOverwrite);
	treeEff->Write("", TObject::kOverwrite);
	f->Close(); delete f;
}









