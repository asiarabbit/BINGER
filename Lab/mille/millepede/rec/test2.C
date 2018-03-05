// test.C -- test script for matrix package written for millepede project

#include "TAMatrix.h"
#include "TAMatrixSym.h"
#include "TAFun.h"
// ROOT header files
#include "TFile.h"
#include "TTree.h"

const int NPGLB = 10, NPLOC = 2; // number of global and local parameters
extern const double a[10][12]; // observation point
extern const double res[10]; // spatial resolution of the ten planar detectors
double fun(const double *a, const double *p); // objective function declaration

int main(){
	double dataT[2] = {1., 2.}; // DEBUG
	double dataSym[4] = {0., 1., 1., 0.};
	TAMatrix mR(2, 1, dataT); // DEBUG
	TAMatrix mL(1, 2, dataT); // DEBUG
	TAMatrix mSym(2, 2, dataSym);
	TAMatrixSym mM = mR*mL; // DEBUG
	mM.Show(); // DEBUG
	mM += mSym; // DEBUG
	mM.Show();
	return 0; // DEBUG

	TAFun f(fun, 10, 12);
	
	double y_r[10], y[10], p[12];
	for(double &x : p) x = 0.; // all parameters are initialized to zero.
	// A_ij: partial_f_i / partial_a_j; Ag: global coefficent matrix; Al: local coefficient matrix
	double Ag[10*10], Al[10*2], W[10*10], r[10]; // W: V^{-1}, r_i: residual_i

	TFile *file = new TFile("calibration.root");
	TTree *treeData = (TTree*)file->Get("treeData");
	treeData->SetBranchAddress("y_r", y_r);
	const int n = treeData->GetEntries();
	TAMatrix ma(10, 1); // to store the final solved global parameters


	int iter = 0;
	while(iter++ < 3){ // iteration times
		TAMatrixSym mCp(10); // the coefficient matrix of the normal equation for global parameters
		TAMatrix mbp(10, 1); // constant vector of the normal equation for global parameters

		for(int i = 0; i < 100; i++){ // loop over events
			treeData->GetEntry(i);
			// assign mAgi, mAli and r //
			int nF = 0, nu[10]{}; // number of fired planar detectors
			for(int j = 0; j < 10; j++) if(y_r[j] != -9999.){
				nu[nF] = j;
				y[nF++] = y_r[j];
			}
			for(double &x : W) x = 0.;
			for(int j = 0; j < nF; j++) W[j*nF+j] = 1./(res[nu[j]]*res[nu[j]]);
			TAMatrix mW(nF, nF, W);
			TAMatrix mAgi(nF, 10); TAMatrix mAli(nF, 2);
			TAMatrixSym mGammai(2); TAMatrixSym mGammaiInv(2);
			TAMatrix mri(nF, 1); TAMatrix malphai(2, 1); TAMatrix mbetai(2, 1);
			// implement local fit
			int iterLoc = 0;
			while(iterLoc++ < 3){
				#include "localfit.C" // fit the i-th track
			}

			// %%%%%%%%%%%%%%%%%%%% MILLEPEDE SUBROUTINE %%%%%%%%%%%%%%%%%%%%%%%%%% //
			TAMatrixSym mCi = mAgi.Transpose()*mW*mAgi;
			TAMatrix mGi = mAgi.Transpose()*mW*mAli;
			TAMatrix mbi = mAgi.Transpose()*mW*mri;

			mCp += mCi - mGi*mGammaiInv*mGi.Transpose();
			mbp += mbi - mGi*malphai;
			// %%%%%%%%%%%%%%%%%%%% END OF MILLEPEDE SUBROUTINE %%%%%%%%%%%%%%%%%%%%%% //
		} // end for over l
		// #################********* SOLVE GLOBAL PARAMETERS *********################# //
		ma = mCp.Invert()*mbp; // mCp.Invert(): potentially most time-consuming
		ma.GetData(p); // update global parameters
		// #############********* END OF SOLVING GLOBAL PARAMETERS *********############# //
	} // end of the outmost loop
	cout << "MILIPEDE IMPLEMENTATION COMPLETE!\n";
	cout << "PRESS ENTER TO SEE THE RESULT...";
	getchar();
	ma.Show(); // HAHA
	getchar();

	file->Close();
	delete file;
	return 0;
} // end of main function

#include "coefficient.C"

