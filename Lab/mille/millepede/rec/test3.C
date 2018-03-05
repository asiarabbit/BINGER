// test.C -- test script for matrix package written for millepede project

#include "TAMatrix.h"
#include "TAMatrixSym.h"
#include "TAFun.h"
// ROOT header files
#include "TFile.h"
#include "TTree.h"

const int MG = 10, ML = 2, MT = MG + ML; // Maximum number of Global, Local and Total parameters
const int MO = 10; // Maximum number of Observation points
const int NITERGLB = 3; // iteration times to fit global parameters
const int NITERLCL = 3; // iteration times to fit local parameters
extern const double a[MO][MT]; // observation point
extern const double res[MO]; // intrinsic resolution of each observation points
double fun(const double *a, const double *p); // objective function declaration

int main(){
	double dataA[9] = {1., 1., 0., 1., -1., 0., 0., 0., 0.}; // DEBUG
	double datab[3] = {2., 0., 0.};
	TAMatrixSym mA(3, dataA); // DEBUG
	TAMatrix mbb(3, 1, datab);
	TAMatrixSym mAINV = mA.Invert();
	TAMatrix solution = mAINV*mbb;
	mAINV.Show();
	solution.Show();
	return 0; // DEBUG

	TAFun f(fun, MT, MT); // ndim and npar corresponds to those for function fun()
	
	double y_r[MO], y[MO], p[MT];
	for(double &x : p) x = 0.; // all parameters are initialized to zero.
	// A_ij: partial_f_i / partial_a_j; Ag: global coefficent matrix; Al: local coefficient matrix
	double Ag[MG*MG], Al[ML*ML], W[MO*MO], r[MO]; // W: V^{-1}, r_i: residual_i

	TFile *file = new TFile("calibration.root");
	TTree *treeData = (TTree*)file->Get("treeData");
	treeData->SetBranchAddress("y_r", y_r);
	const int n = treeData->GetEntries();
	TAMatrix ma(MG, 1); // to store the final solved global parameters

	int iter = 0; double chi2 = 0.;
	while(iter++ < NITERGLB){ // iteration times
		TAMatrixSym mCp(MG); // the coefficient matrix of the normal equation for global parameters
		TAMatrix mbp(MG, 1); // constant vector of the normal equation for global parameters

		for(int i = 0; i < 100; i++){ // loop over events
			treeData->GetEntry(i);
			// assign mAgi, mAli and r //
			int nF = 0, nu[MO]{}; // number of fired planar detectors
			for(int j = 0; j < MO; j++) if(y_r[j] != -9999.){
				nu[nF] = j;
				y[nF++] = y_r[j];
			}
			for(double &x : W) x = 0.;
			for(int j = 0; j < nF; j++) W[j*nF+j] = 1./(res[nu[j]]*res[nu[j]]);
			TAMatrix mW(nF, nF, W);
			TAMatrix mAgi(nF, MG); TAMatrix mAli(nF, ML);
			TAMatrixSym mGammai(ML); TAMatrixSym mGammaiInv(ML);
			TAMatrix mri(nF, 1); TAMatrix malphai(ML, 1); TAMatrix mbetai(ML, 1);
			// implement local fit
			int iterLoc = 0;
			while(iterLoc++ < NITERLCL){
				#include "localfit.C" // fit the i-th track
			}

			for(int j = 0; j < nF; j++) r[j] = y[j] - f.Eval(a[j], p); // update residuals
			mri.SetData(r);
			// chisquare distribution expected.
			chi2 = (mri.Transpose()*mW*mri)[0][0];///(nF-ML);
//			if(chi2 ... ) continue;
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

