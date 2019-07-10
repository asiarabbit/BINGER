#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iomanip>

#include <TApplication.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TGraph.h>
#include <TFile.h>
#include <TTree.h>
//#include <TRandom3.h>

#include "ComponentConstant.hh"
#include "MediumMagboltz.hh"
#include "Random.hh"
#include "ViewField.hh"
#include "ViewCell.hh"
#include "ViewDrift.hh"
#include "Sensor.hh"
#include "Plotting.hh"
#include "SolidBox.hh"
#include "AvalancheMicroscopic.hh"
#include "ComponentAnalyticField.hh"
#include "GeometrySimple.hh"
#include "ViewGeometry.hh"
#include "TrackHeed.hh"
#include "DriftLineRKF.hh"
#include "ViewSignal.hh"

//#define SIGNAL // calculate induced signal
//#define GRAPH // show and store drift time of certain anodes for certain track
#define VERBOSE // verbose mode
#define SHOW_DRIFT_LINE

using namespace Garfield;
using std::cout;
using std::endl;
using std::string;
using std::cin;
using std::flush;

void sort(const int size, double *x, double *t, int &max_t);

// DC HV/V
const double vSenseWire = 900.;
const int nEvents = 1;
int main(int argc, char *argv[]){
	TApplication app("app", &argc, argv);
	char filename[128]; sprintf(filename, "garfield%d.root", int(vSenseWire));
	TFile *file = new TFile(filename, "recreate");
//	plottingEngine.SetDefaultStyle();

	// define the medium
	MediumMagboltz *gas = new MediumMagboltz();
	gas->SetComposition("ar", 80., "co2", 20.);
	gas->LoadGasFile("ar_80_co2_20.gas");
	string garpath = std::getenv("GARFIELD_HOME");
	gas->LoadIonMobility(garpath + "/Data/IonMobility_Ar+_Ar.txt");
	gas->LoadIonMobility(garpath + "/Data/IonMobility_CO2+_CO2.txt");
	// set temperature [K] and pressure [Torr]
	gas->SetPressure(0.841 * 760.);
	gas->SetTemperature(293.15);

	// wire radius [cm]
	const double rSenseWire = 10.e-4; // 10 um gold-plated tungsten
	const double rFieldWire = 50.e-4; // 50 um beryllium-plated copper
	const double mdd = Sqrt2*5.; // maximum drift distance within a drift cell
	// make a tube (centered at the origin, inner radius: rWire, out radius: rTube)
	SolidBox *box = new SolidBox(0., 0., 0., 40., 2.0, 60.);
	GeometrySimple *geo = new GeometrySimple();
	// add the solid to the geometry, together with the medium inside
	geo->AddSolid(box, gas);
	// create the component to calculate the electric field
	ComponentAnalyticField *cmp = new ComponentAnalyticField();
	cmp->SetGeometry(geo);
	// voltages; unit: V
	const double vFieldWire = 0.;
	const int nCells = 8; // have to be even
	const int hnCells = nCells/2;
	// add wires
	double x = 0., y = -0.5; // coordinates of the X1 central sense wire
	double sx[nCells]; // abscissas of sense wires
	sx[0] = 0.; sx[nCells-1] = hnCells;
	for(int i = 1; i < hnCells; i++){
		sx[2*i-1] = i*1.;
		sx[2*i] = -i*1.;
	}
#ifdef SIGNAL
	string readout_label = "s";
#else
	string readout_label = "s";
#endif
	for(int i = 0; i < nCells; i++){ // add one drift cell for each loop
		x = sx[i];
		// add sense wire
		string s = readout_label; if(i != 0) s = "s";
		cmp->AddWire(x + 0.4, y + 1., 2. * rSenseWire, vSenseWire, "s", 30.); // X1
		cmp->AddWire(x, y, 2. * rSenseWire, vSenseWire, s, 30.); // X2
		// add field wires
		// X1 the field wires in the central up and down
		cmp->AddWire(x, y + 1. + .5, 2. * rFieldWire, vFieldWire, "f", 30.);
		// X2 the field wires in the central up and down
		cmp->AddWire(x, y + .5, 2. * rFieldWire, vFieldWire, "f", 30.);
		cmp->AddWire(x, y - .5, 2. * rFieldWire, vFieldWire, "f", 30.);
		for(int j = 1; j < 3; j++){
			cmp->AddWire(x - j*.2, y + 1.5, 2. * rFieldWire, vFieldWire, "f", 30.);
			cmp->AddWire(x + j*.2, y + 1.5, 2. * rFieldWire, vFieldWire, "f", 30.);

			cmp->AddWire(x - j*.2, y + .5, 2. * rFieldWire, vFieldWire, "f", 30.);
			cmp->AddWire(x - j*.2, y - .5, 2. * rFieldWire, vFieldWire, "f", 30.);
			cmp->AddWire(x + j*.2, y + .5, 2. * rFieldWire, vFieldWire, "f", 30.);
			cmp->AddWire(x + j*.2, y - .5, 2. * rFieldWire, vFieldWire, "f", 30.);
		} // end for over j
		cmp->AddWire(x + 0.4 - .5, y + 1., 2. * rFieldWire, vFieldWire, "f", 30.);
		cmp->AddWire(x - .5, y, 2. * rFieldWire, vFieldWire, "f", 30.);
	} // end for over i
	// X1 central right of the last anode
	cmp->AddWire(sx[nCells-1] + 0.4 + .5, y + 1., 2. * rFieldWire, vFieldWire, "f", 30.);
	// X2 central right of the last anode
	cmp->AddWire(sx[nCells-1] + .5, y, 2. * rFieldWire, vFieldWire, "f", 30.);
	cmp->AddPlaneY(-1.5, 0., "Aluminum");
	cmp->AddPlaneY(1.5, 0., "Aluminum");
#ifdef SIGNAL
	// requires to calculate the weighting field
	cmp->AddReadout(readout_label);
#endif

	// view the drift cell
	ViewCell *cellView = new ViewCell();
	TCanvas *myCanvas = new TCanvas("myCanvas", "MWDC Drift Cell Visualization");
	cellView->SetCanvas(myCanvas);
	cellView->SetComponent(cmp);
	cellView->SetArea(-2.5, -2., -5., 2.5, 2., 5.);
	cellView->Plot2d();
	myCanvas->Update();

	// create the sensor
	Sensor *sensor = new Sensor();
	sensor->AddComponent(cmp);
#ifdef SIGNAL
	sensor->AddElectrode(cmp, readout_label);
	sensor->SetTimeWindow(0., 0.01, 50000);
#endif

	ViewField *fieldView = new ViewField();
	TCanvas *cf = new TCanvas("cf", "Field View");
	fieldView->SetCanvas(cf);
	fieldView->SetSensor(sensor);
	fieldView->SetArea(-2.5, -2., 2.5, 2.);
	fieldView->PlotContour("e");

	ViewDrift *driftView = new ViewDrift();
	driftView->SetCanvas(myCanvas);
	driftView->SetArea(-2.5, -2., -5., 2.5, 2., 5.);
	
	DriftLineRKF *driftLine = new DriftLineRKF();
	driftLine->SetSensor(sensor);
#ifdef SHOW_DRIFT_LINE
	driftLine->EnablePlotting(driftView);
#endif

	// track generator for primary ionized electrons
	TrackHeed *track = new TrackHeed();
	track->SetSensor(sensor);
	track->SetParticle("deuteron");
	track->SetBeta(0.9);
#ifdef SHOW_DRIFT_LINE
	track->EnablePlotting(driftView);
#endif
#ifdef VERBOSE
	track->EnableDebugging();
#endif

	// define a tree to hold the data
	int layerId_tree, anodeId_tree, driftLineCnt_tree; // to specify the hit anode
	double dca_tree, driftTime_tree, x_ionization_tree, kl_tree, bl_tree;
	TTree *tree = new TTree("tree", "Drift Time Research Using Garfield");
	tree->Branch("layerId", &layerId_tree, "layerId/I");
	tree->Branch("anodeId", &anodeId_tree, "anodeId/I");
	tree->Branch("dca", &dca_tree, "dca/D");
	tree->Branch("t", &driftTime_tree, "t/D");
	tree->Branch("x", &x_ionization_tree, "x/D");
	tree->Branch("kl", &kl_tree, "kl/D");
	tree->Branch("bl", &bl_tree, "bl/D");
	tree->Branch("DLcnt", &driftLineCnt_tree, "DLcnt/I");

	double x0, y0, z0, t0, dx0, dy0, dz0, x1, y1;
	TRandom3 rdm; rdm.SetSeed(time(0)); // seed would be different for each run
	int loop = 0;
	cout << nEvents << " events would be simulated.\n";
	while(loop++ < nEvents){
	// prepare a new track
	x0 = rdm.Uniform(-2.3, 2.3); y0 = -1.5; x1 = rdm.Uniform(-2.3, 2.3); y1 = 1.5;
	dx0 = x1-x0; dy0 = y1-y0; dz0 = 0; z0 = 0.; t0 = 0.;
	double kl = dy0/dx0, bl = y0 - kl*x0;
	cout << "\033[32;1m----- LOOP " << loop << "\tNew Track Generated.";
	cout << "\tkl: " << kl << "\tbl: " << bl << " -----\033[0m\r" << flush;
	track->NewTrack(x0, y0, z0, t0, dx0, dy0, dz0);
	// position, time and energy of the ionizing collision
	double xc, yc, zc, tc, ec, extra;
	int nc; // number of electrons produced in the collision
	int ncsum = 0; double ecsum = 0.; int counter = 0;
	// for filling x_en-t graph gxt_x: initial abscissa of ionized electrons
	const int ncmax = 10; // maximum number of electrons in a cluster
	double gxt_x[2][nCells][10000], gxt_t_en[2][nCells][10000];
	int gxt_cnt[2][nCells]{}; // [layer][anode][v]
	double dca[2][nCells]; // distance of the closest approach
	while(track->GetCluster(xc, yc, zc, tc, nc, ec, extra)){
		ncsum += nc; ecsum += ec;
#ifdef VERBOSE
		cout << "xc: " << xc << "\tyc: " << yc << "\tzc: " << zc << "\ttc: " << tc;
		cout << "\tnc : " << nc << "\tec: " << ec << "\tec/nc: " << ec / nc;
		cout << "\t" << counter << endl;
//		cout << endl; getchar();
#endif
		int nc_ = nc < ncmax ? nc : ncmax;
//		cout << "nc_: " << nc_ << endl; getchar();
		for(int i = 0; i < nc_; i++){
			double x, y, z, t, e, dx, dy, dz;
			track->GetElectron(i, x, y, z, t, e, dx, dy, dz);
//			cout << "i: " << i << " x: " << x << " y: " << y << " z: " << z; // DEBUG
//			cout << " t: " << t << " e: " << e << " dx: " << dx << " dy: " << dy; // DEBUG
//			cout << " dz: " << dz << endl; // DEBUG
			int status;
			double ex, ey, ez, v; Medium *m;
			cmp->ElectricField(x, y, z, ex, ey, ez, v, m, status);
			if(status) continue; // no valid field at initial point
			driftLine->DriftElectron(x, y, z, 0.);
//			continue;
			double x_en, y_en, z_en, t_en;
			driftLine->GetEndPoint(x_en, y_en, z_en, t_en, status);
			// projected drift distance
//			double distance = sqrt(pow(x_en-x, 2)+pow(y_en-y, 2)+pow(z_en-z, 2)); // DEBUG
			cmp->ElectricField(x_en, y_en, z_en, ex, ey, ez, v, m, status);
			double eNorm = sqrt(ex*ex+ey*ey+ez*ez);
			if(v > vSenseWire * 0.7 && eNorm > 8000.){ // electron drifted to a sense wire
				// reckon the hit sense wire
				int mm = -1, jj; double sxHit, syHit; // of the hit sense wire
				for(int m = 0; m < 2; m++) for(int j = 0; j < nCells; j++){
					// the j-th X[m] sense wire
					double sx_t = sx[j], sy_t = -0.5;
					if(m == 0){ // X1
						sx_t += 0.4; sy_t += 1.;
					}
					if(fabs(x_en-sx_t)<0.25 && fabs(y_en-sy_t)<0.25){
						mm = m; jj = j; sxHit = sx_t; syHit = sy_t;
						break;
					} // end if
				} // end for over m and j

				if(mm == -1) continue;
//				cout << "\n\033[31;1m______BANG_______\033[0m\n"; // DEBUG
//				cout << "\033[33;1mWire at layer X" << mm+1; // DEBUG
//				cout << " id_" << jj << " Hit\033[0m\n"; // DEBUG
				// distance of closest approach
				dca[mm][jj] = fabs(kl*sxHit-syHit+bl)/sqrt(1.+kl*kl);
				gxt_x[mm][jj][gxt_cnt[mm][jj]] = x;
				gxt_t_en[mm][jj][gxt_cnt[mm][jj]] = t_en;
				gxt_cnt[mm][jj]++;
//				cout << "V: " << v << "\tE: " << eNorm; // DEBUG
//				cout << "\tdca: " << dca[mm][jj]*10. << "\tdistance: " << distance*10. << endl; // DEBUG
//				cout << "x_en: " << x_en << "\ty_en: " << y_en; // DEBUG
//				cout << "\tz_en: " << z_en << "\tt_en: " << t_en << endl; // DEBUG

				//$$$$$$$$$$$$$$$$$$$ THE DESIRED OUTPUT $$$$$$$$$$$$$$$$$$$//
//				cout << "\033[32;1mkl: " << kl; // DEBUG
//				cout << "\tDCA: " << dca[mm][jj]*10. << "\tt_en: " << t_en; // DEBUG
//				cout << "\tvr: " << distance*1.e4/t_en; // DEBUG
//				cout << "\tv_: " << dca[mm][jj]*1.e4/t_en << " [um/ns]\n\033[0m"; // DEBUG
				//$$$$$$$$$$$$$$$$$$$ THE DESIRED OUTPUT $$$$$$$$$$$$$$$$$$$//
//				getchar();

			} // end if
		} // end for over i
		counter++;
	} // end while

#ifdef GRAPH
	TGraph *gxt[2][5]; char name[8], title[64];
#endif
	for(int i = 0; i < 2; i++) for(int j = 0; j < 5; j++) if(gxt_cnt[i][j] > 0){
		int mi; // index of the minimum of the drift time for certain anode.
		sort(gxt_cnt[i][j], gxt_x[i][j], gxt_t_en[i][j], mi); // x ascending order
		if(dca[i][j] < mdd && dca[i][j] > 1e-3){
			layerId_tree = i; anodeId_tree = j; driftLineCnt_tree = gxt_cnt[i][j];
			dca_tree = dca[i][j]; driftTime_tree = gxt_t_en[i][j][mi];
			x_ionization_tree = gxt_x[i][j][mi]; kl_tree = kl; bl_tree = bl;
			tree->Fill();
		}
#ifdef VERBOSE
		cout << "n: " << gxt_cnt[i][j] << "\tmi: " << mi << endl; // DEBUG
		cout << "x[mi]: " << gxt_x[i][j][mi] << "\tt[mi]: " << gxt_t_en[i][j][mi] << "\tdca: " << dca[i][j]*10 << " [mm]"; // DEBUG
		cout << "\tv[mi]: " << dca[i][j]*1.e4/gxt_t_en[i][j][mi] << "[um/ns]\n"; // DEBUG
//		getchar(); // DEBUG
#endif
#ifdef GRAPH
		gxt[i][j] = new TGraph(gxt_cnt[i][j], gxt_x[i][j], gxt_t_en[i][j]);
		sprintf(name, "gxt%d%d", i, j);
		sprintf(title, "x_initial - drift Time of X%d sense_wire %d", i+1, j);
		gxt[i][j]->SetNameTitle(name, title); gxt[i][j]->SetMarkerStyle(23);
		gxt[i][j]->SetMarkerColor(2); gxt[i][j]->SetMarkerSize(2);
		gxt[i][j]->Write();
#endif
	} // end for over i and j
	} // end while over events


#ifdef SHOW_DRIFT_LINE
	driftView->Plot(true, false);
#endif
#ifdef SIGNAL
	// plot the induced current.
	ViewSignal *vs = new ViewSignal();
	TCanvas *cvs = new TCanvas("cvs", "Signal");
	vs->SetCanvas(cvs);
	vs->SetSensor(sensor);
	vs->PlotSignal(readout_label);
#endif
	// write the ROOT objects into a rootfile
	myCanvas->Write(); cf->Write(); tree->Write();
#ifdef SIGNAL
	cvs->Write();
#endif
	file->Close();

	cout << "\nDONE\n";
	app.Run(true);
}


void sort(const int n, double *x, double *t, int &min_i){
	double xt,tt; double min_t = t[0]; min_i = 0;
	for(int i = 1; i < n; i++){
		for(int j = n - 1; j >= i; j--){
			if(x[j] < x[j-1]){
				xt = x[j-1]; tt = t[j-1];
				x[j-1] = x[j]; t[j-1] = t[j];
				x[j] = xt; t[j] = tt;
			} // end if
		} // end for over j
	} // end for over i
//	for(int i = 0; i < n; i++) cout << "x[i]: " << x[i] << "\tt[i]: " << t[i] << endl; // DEBUG
	for(int i = 0; i < n; i++) if(t[i] < min_t) { min_t = t[i]; min_i = i; }
} // end function sort






