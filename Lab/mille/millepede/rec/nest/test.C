// test.C -- test script for matrix package written for millepede project

#include "TAMatrix.h"
#include "TAMatrixSym.h"
#include "TAFun.h"
// ROOT header files
#include "TFile.h"
#include "TTree.h"

const int NG = 9, NL = 2, NT = NG + NL; // Number of Global, Local and Total parameters
const int NO = 10; // Number of Observation points
const int NITERGLB = 1; // iteration times to fit global parameters
const int NITERLCL = 1; // iteration times to fit local parameters
extern const double a[NO][NT]; // observation point
extern const double res[NO]; // intrinsic resolution of each observation points
double fun(const double *a, const double *p); // objective function declaration
double ChisquareDistribution(double );

TAMatrixSym main2(){
	TAFun f(fun, NT, NT); // ndim and npar corresponds to those for function fun()
	
	double y_r[NO], p[NT]; // p[0~NG-1: global; NG~NT: local]
	for(double &x : p) x = 0.; // all parameters are initialized to zero. //
	// A_ij: partial_f_i / partial_a_j; Ag: global coefficent matrix; Al: local coefficient matrix
	double Ag[NO*NG], Al[NO*NL], W[NO*NO], r[NO]; // W: V^{-1}, r_i: residual_i

	TFile *file = new TFile("calibration.root");
	TTree *treeData = (TTree*)file->Get("treeData");
	double k, b;
	treeData->SetBranchAddress("y_r", y_r);
	treeData->SetBranchAddress("k", &k);
	treeData->SetBranchAddress("b", &b);
	const int n = treeData->GetEntries();

	TAMatrixSym mCp(NG); // the coefficient matrix of the normal equation for global parameters
	TAMatrix mbp(NG, 1); // constant vector of the normal equation for global parameters
	TAMatrix ma(NG, 1); // to store the final solved global parameters

	int iter = 0; double chi2 = 0.;
	while(iter++ < NITERGLB){ // iteration times
		mCp.Initialize(); mbp.Initialize();
		for(int i = 0; i < 10; i++){ // loop over events
			treeData->GetEntry(i);
			// assign mAgi, mAli and r //
			int nF = 0, nu[NO]; // number of fired planar detectors
			for(int &x : nu) x = -1;
			for(int j = 0; j < NO; j++) if(y_r[j] != -9999.) nu[nF++] = j;
//			cout << "nF: " << nF << endl; getchar(); // DEBUG
			for(double &x : W) x = 0.;
			for(int j = 0; j < nF; j++) W[j*nF+j] = 1./(res[nu[j]]*res[nu[j]]);
			// matrices for local fit of track i
			TAMatrixSym mW(nF, W);
			TAMatrix mAgi(nF, NG); TAMatrix mAli(nF, NL);
			TAMatrixSym mGammai(NL); TAMatrixSym mGammaiInv(NL);
			TAMatrix mri(nF, 1); TAMatrix malphai(NL, 1); TAMatrix mbetai(NL, 1);
			// implement local fit
			int iterLoc = 0;
//			for(int j = 0; j < NO; j++){ // DEBUG
//				cout << "nu[" << j << "]: " << nu[j]; // DEBUG
//				cout << "\ty_r[" << j << "]: " << y_r[j] << endl; // DEBUG
//			} // end for over j // DEBUG
//			cout << "mW.Show():\n"; // DEBUG
//			mW.Show(); // DEBUG
//			getchar(); // DEBUG
			for(int j = NG; j < NT; j++) p[j] = 0.; // initialize local par for each track
			while(iterLoc++ < NITERLCL){
				#include "localfit.C" // fit the i-th track
			}
//			if(fabs(atan(p[NG+1])-atan(k))/atan(k) > 0.1){ // DEBUG
//				cout << "This is the track " << i << endl; // DEBUG
//				cout << "Bad local fit occured.\n"; // DEBUG
//				cout << "Fitted k: " << p[NG+1] << " true k: " << k << endl; // DEBUG
//				cout << "angle relative error: " << fabs(atan(p[NG+1])-atan(k))/atan(k) << endl; // DEBUG
//				getchar(); // DEBUG
//			} // DEBUG
			// outlier check
//			chi2 = (mri.Transpose()*mW*mri)[0][0];///(nF-NL); // DEBUG
//			cout << "chi2: " << chi2 << endl; getchar(); // DEBUG
////			TAMatrixSym mWr = mAli*mGammaiInv*mAli.Transpose(); // covariance matrix of the residual
//			double pull[NO]{};
//			for(int j = 0; j < nF; j++){ pull[j] = malphai[j][0]/mWr[j][j]; } // pull function, N(0,1) distribution expected.
//			if(chi2 ... ) continue;
			// %%%%%%%%%%%%%%%%%%%% MILLEPEDE SUBROUTINE %%%%%%%%%%%%%%%%%%%%%%%%%% //
			TAMatrixSym mCi = mAgi.Transpose()*mW*mAgi;
			TAMatrix mGi = mAgi.Transpose()*mW*mAli;
			TAMatrix mbi = mAgi.Transpose()*mW*mri;
//			cout << "mCi.Show():\n"; // DEBUG
//			mCi.Show(); getchar(); // DEBUG
//			cout << "mCp.Show():\n"; // DEBUG
//			mCp.Show(); getchar(); // DEBUG

//			cout << "mCp0.Show():\n"; // DEBUG
//			mCp.Show(); getchar(); // DEBUG
//			cout << "mbp0.Show():\n"; // DEBUG
//			mbp.Show(); getchar(); // DEBUG
			mCp += mCi - mGi*mGammaiInv*mGi.Transpose();
			mbp += mbi - mGi*malphai;
			cout << "Track " << i << " processed.\r" << flush;
//			cout << "\n\nmalphai.Show():\n"; // DEBUG
//			malphai.Show(); getchar(); // DEBUG
			// %%%%%%%%%%%%%%%%%%%% END OF MILLEPEDE SUBROUTINE %%%%%%%%%%%%%%%%%%%%%% //
//			cout << endl << endl; // DEBUG
//			cout << "mCp1.Show():\n"; // DEBUG
//			mCp.Show(); getchar(); // DEBUG
//			cout << "mbp1.Show():\n"; // DEBUG
//			mbp.Show(); getchar(); // DEBUG
		} // end for over i (tracks)
		// #################********* SOLVE GLOBAL PARAMETERS *********################# //
		cout << "GLOBAL ITERATION " << iter << " completed, \n"; // DEBUG
		mCp.Show();
		cout << "mCp.GetNRow(): " << mCp.GetNRow();
		cout << "\nmCp.GetNColumn(): " << mCp.GetNColumn();
		TAMatrixSym mCpInv = TAMatrixSym::SPMINV(mCp);
		cout << "mCpInv.Show():\n"; getchar(); // DEBUG
		mCpInv.Show(); // DEBUG
		mbp.Show();
		getchar();
		return mCp;
		
		ma = mCpInv*mbp; // mCp.Invert(): potentially most time-consuming
		ma.AppendData(p); // update global parameters :):):):):)

		cout << "mCp*mCpInv:\n"; getchar(); // DEBUG
		(mCp*mCp.Invert()).Show(); // DEBUG
		cout << "The solved global parameters:\n"; // DEBUG
		ma.Show(); // DEBUG
		cout << "Press ENTER to start the next iteration\n"; getchar(); // DEBUG
		// #############********* END OF SOLVING GLOBAL PARAMETERS *********############# //
	} // end of the outmost loop
	cout << "MILIPEDE IMPLEMENTATION COMPLETE!\n";
	cout << "PRESS ENTER TO SEE THE RESULT...";
	getchar();
	ma.Show(); // HAHA
	getchar();

	file->Close();
	delete file;
	return mCp;
} // end of main function

#include "coefficient.C"

