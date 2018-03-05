// test.C -- test script for matrix package written for millepede project
//#define DEBUG
//#define OFFSET // Since it's millepede's purpose to work out offsets, this should be switched off.
#include "TAUSER.h"
#include "config/configure.C"
#include "millepede/coefficient.C" // user specifications

int main(){
	#include "millepede/ROOTConfig.C"

	double p[NT]; // p[0~NG-1: global; NG~NT: local]
	for(double &x : p) x = 0.; // all parameters are initialized to zero. //
	// import detector configuration information
	TAMWDCArray *dcArrR = new TAMWDCArray(ccChId_DCArrR, ccLayout_DCArrR, "MWDCArray-MILLEPEDE", 100999999);
	const double phiAvrg = dcArrR->GetPhiAvrg();
	TAFun f(fun, NT, NT, dcArrR); // ndim and npar corresponds to those for function fun()
	// A_ij: partial_f_i / partial_a_j; Ag: global coefficent matrix; Al: local coefficient matrix
	double Ag[NO*NG], Al[NO*NL], W[NO*NO]; // W: V^{-1}
	TAMatrixSym mCp(NG); // the coefficient matrix of the normal equation for global parameters
	TAMatrix mbp(NG, 1); // constant vector of the normal equation for global parameters
	TAMatrix ma(NG, 1); // to store the final solved global parameters
	// constraints
	TAMatrix mCons(NC, NG, cons); // constraint coefficient matrix
	TAMatrix mcons(NC, 1, consc); // constant vector for equality constraints
	// minmization with linear constraints
	TAMatrixSym mCpEx(NG+NC); // accommodates mCp and mCons
	TAMatrix mbpEx(NG+NC, 1); // accommodates mbp and mcos
	TAMatrix maEx(NG+NC, 1); // store the expanded solution (including Lagrange multipliers)

	// chi20: to estimate the track quality of the pattern recogniation
	int iter = 0; double chi2 = 0., chi20 = 0.;
	int outlierCnt[NITERGLB-1]{}, nF_tot = 0;
	cout << n << " tracks are stored in tree." << endl;
//	getchar(); // DEBUG
	while(iter < NITERGLB){ // iteration times
		chi2 = 0.;
		mCp.Initialize(); mbp.Initialize(); // set matrices to zero
		for(int &x : trkId[0]) x = -1; for(int &x : trkId[1]) x = -1; for(int &x : trkId[2]) x = -1;
		for(int i = 0; i < n; i++){ // loop over events
			memcpy(dr, initializer, sizeof(dr));
			memcpy(pull_, initializer, sizeof(pull_));
			treeTrack->GetEntry(i); // Data of one data section has been transferred.
			// assign mAgi, mAli and r //
			////// recognize and cache 3-D tracks
			n3DtrX = 0; n3DtrU = 0; n3DtrV = 0;
			// loop over grouped track projections
			for(int j = 0; j < ntr; j++) if(id[j] != -1){
				if(type[j] == 110){
					trkId[0][id[j]] = j; // DCArrR-X
					n3DtrX++;
				}
				if(type[j] == 111){
					trkId[1][id[j]] = j; // DCArrR-U
					n3DtrU++;
				}
				if(type[j] == 112){
					trkId[2][id[j]] = j; // DCArrR-V
					n3DtrV++;
				}
			} // end for over j and if
			if(n3DtrX != n3DtrU || n3DtrX != n3DtrV){
				cout << "This is odd... track projections of X, U and V are not consistent.\n";
				cout << "n3DtrX: " << n3DtrX << " n3DtrU: " << n3DtrU << " n3DtrV: " << n3DtrV << endl;
				getchar();
			} // end if
			n3Dtr = n3DtrX;
		  // // // ^^^^^^^ circulation over 3-D tracks in one data section ^^^^^^^ // // //
		      for(int jj = 0; jj < n3Dtr; jj++){ // loop over 3D tracks in a data section.
			int nFX = 0, nFU = 0, nFV = 0; // fired anode layers in 
			for(int j = 0; j < 6; j++){ // count effective measurements
				if(nu[trkId[0][jj]][j] != -1) nFX++;
				if(nu[trkId[1][jj]][j] != -1) nFU++;
				if(nu[trkId[2][jj]][j] != -1) nFV++;
			} // end for over j
			const int nF = nFX + nFU + nFV; // number of measured points
			if(iter == NITERGLB - 1) nF_tot += nF;
//			cout << "nFX: " << nFX << "\tnFU: " << nFU << "\tnFV: " << nFV << endl; // DEBUG
//			cout << "nF: " << nF << endl; getchar(); // DEBUG
			// // // X*6-U*6-V*6 -> W // assign W <-> V^{-1} // // //
			for(double &x : W) x = 0.; int tmp = 0;
			for(int k = 0; k < 3; k++){ // loop over X-U-V
				for(int j = 0; j < 6; j++){
					if(nu[trkId[k][jj]][j] != -1){
						// sigma varies with drift distance
						double sigma = res[j]; // *sigmaGain(r[trkId[k][jj]][j]);
//						cout << "res[j]: " << sigma << endl; getchar(); // DEBUG
						W[tmp*nF+tmp] = 1./(sigma*sigma); tmp++;
						// shadow U measurements by settig the corresponding weight to zero
//						if(k == 1) W[tmp*nF+tmp] = 0.;
						// shadow V measurements by settig the corresponding weight to zero
//						if(k == 2) W[tmp*nF+tmp] = 0.;
					}
				} // end for over j
			} // end for over k
			// // // // // // END OF W ASSIGNMENT // // // // // // //
			
			// matrices for local fit of track i-jj
			TAMatrixSym mW(nF, W);
//			mW.Show(); getchar(); // DEBUG
			// parameter derivatives - rectangular matrices
			TAMatrix mAgi(nF, NG); TAMatrix mAli(nF, NL);
			// coefficient matrices of local fit normal equation (LSM)
			TAMatrixSym mGammai(NL); TAMatrixSym mGammaiInv(NL);
			TAMatrix mri(nF, 1); TAMatrix mrip(nF, 1); // mrip: post-fit residual
			// alphai: solution of local parameters in local fit;
			// betai: the global counterpart
			TAMatrix malphai(NL, 1); TAMatrix mbetai(NL, 1);
			// implement local fit
			int iterLoc = 0; // iteration times of local fit
			// initialize local par for each track
			for(int j = NG; j < NT; j++) p[j] = 0.;
			double a[nF][2], rserial[nF]; // measure points; measured value

			// paramters of the 3-D track. This is mainly for a good iteration start.
			p[18] = k[trkId[0][jj]]; // k1	
			p[20] = b[trkId[0][jj]]; // b1
			p[19] = TAMWDCArray::kUV_Y(phiAvrg, k[trkId[1][jj]], k[trkId[2][jj]]); // k2
			p[21] = TAMWDCArray::bUV_Y(phiAvrg, k[trkId[1][jj]], k[trkId[2][jj]], b[trkId[1][jj]], b[trkId[2][jj]]); // b2
#ifdef DEBUG
			cout << "\nThis is the index " << i << endl; // DEBUG
			cout << "Fitted:\n"; // DEBUG
			cout << "k1: " << p[18] << " k2: " << p[19]; // DEBUG
			cout << "\tb1: " << p[20] << " b2: " << p[21] << endl; // DEBUG
			getchar(); // DEBUG
#endif
			int additionalFitCnt = 0; // dynamically weighted fit depending on fit quality
			double pull[nF]{};
			while(iterLoc < NITERLCL){
				#include "millepede/kalmanFilter.C" // optimize the measurements
				#include "millepede/localfit.C" // fit the i-th track
				iterLoc++;
			}
			k1[jj] = p[18]; b1[jj] = p[20];
			k2[jj] = p[19]; b2[jj] = p[21];
#ifdef DEBUG
			cout << "After fit, this is the index " << i << endl; // DEBUG
			cout << "Fitted:\n"; // DEBUG
			cout << "k1: " << p[18] << " k2: " << p[19]; // DEBUG
			cout << "\tb1: " << p[20] << " b2: " << p[21] << "\n\n"; // DEBUG
//			getchar(); // DEBUG
#endif

			// %%%%%%%%%%%%%%%%%%%% MILLEPEDE SUBROUTINE %%%%%%%%%%%%%%%%%%%%%%%%%% //
			TAMatrixSym mCi = mAgi.Transpose()*mW*mAgi;
			TAMatrix mGi = mAgi.Transpose()*mW*mAli;
			TAMatrix mbi = mAgi.Transpose()*mW*mri;

			mCp += mCi - mGi*mGammaiInv*mGi.Transpose();
			mbp += mbi - mGi*malphai;
//			mCi.Show(); getchar(); // DEBUG
//			mbi.Show(); getchar(); // DEBUG
//			mGi.Show(); getchar(); // DEBUG
			cout << "Track " << i << " processed.\r" << flush;
			// %%%%%%%%%%%%%%%%%%%% END OF MILLEPEDE SUBROUTINE %%%%%%%%%%%%%%%%%%%%%% //
		      } // end for over (jj) tracks
		      treeTrackMil[iter]->Fill();
		  // // ^^^^^^^ end of circulation over 3-D tracks in one data section ^^^^^^^ // //

		} // end for over i (index). Note that more than one track may exist in an entry.

		// #################********* SOLVE GLOBAL PARAMETERS *********################# //
		// establish mCpEx and mbpEx to accommodate constraints
		TAMatrix mcons_t = mcons - mCons*ma;
		for(int j = 0; j < NG+NC; j++){ // row
			for(int k = 0; k < NG+NC; k++){ // column
				if(j < NG){
					if(k < NG) mCpEx[j][k] = mCp[j][k];
					else mCpEx[j][k] = mCons[k-NG][j];
				}
				else if(k < NG) mCpEx[j][k] = mCons[j-NG][k];
				else mCpEx[j][k] = 0.;
			} // end for over k
			if(j < NG) mbpEx[j][0] = mbp[j][0];
			else mbpEx[j][0] = mcons_t[j-NG][0];
		} // end for over j

//		cout << "THIS IS THE GLOBAL FIT PART.\n"; getchar(); // DEBUG
//		cout << "mbpEx.Show():\n"; mbpEx.Show(); getchar(); // DEBUG
//		cout << "mCpEx.Show():\n"; mCpEx.Show(); getchar(); // DEBUG
		TAMatrixSym mCpExInv = mCpEx.Invert();
		maEx = mCpExInv*mbpEx;
		maEx.AppendData(p, NG); // update global parameters :):):):):)
		ma.SetData(p); // assign g to ma.
#ifdef DEBUG
#endif
		cout << "(mCpExInv*mCpEx).Show():\n"; // getchar(); // DEBUG
		(mCpExInv*mCpEx).Show(); // DEBUG
		cout << "maEx.Show():\n"; // getchar(); // DEBUG
		maEx.Show(); // DEBUG
		cout << "The solved global parameters:\n";
		for(int j = 0; j < NG; j++){
			cout << "p[" << j << "]: " << p[j] << " ";
			if((j+1)%6 == 0) cout << endl;
		}
		cout << "\nchi2: " << chi2 << endl;
		cout << "chi2 of tracks from raw pattern recognition result: " << chi20 << endl;
		if(iter <= NITERGLB){ // DEBUG
			if(iter <= NITERGLB){
				cout << "pull mean: " << hpull[iter]->GetMean() << endl;
				cout << "pull sigma: " << hpull[iter]->GetRMS() << endl;
				cout << "Press ENTER to start the next iteration\n"; // getchar(); // DEBUG
			}
		} // DEBUG
		treeTrackMil[iter]->Write("", TObject::kOverwrite);
		iter++;
		// #############********* END OF SOLVING GLOBAL PARAMETERS *********############# //

	} // end of the outmost loop
	cout << "MILIPEDE IMPLEMENTATION COMPLETE!\n";
	cout << "The solved global parameters:\n";
	for(int j = 0; j < NG; j++){
			cout << "p[" << j << "]: " << p[j] << " ";
			if((j+1)%6 == 0) cout << endl;
	}
	getchar();
	cout << "\033[32;1m_________OUTLIER STATISTICS_________\033[0m\n";
	for(int i = 0; i < NITERGLB - 1; i++){
		cout << "Global iteration " << i + 1 << endl;
		cout << "outlierCnt[" << i << "]: " << outlierCnt[i] << endl;
		cout << "outlierCnt[" << i << "] / nF_tot: " << double(outlierCnt[i]) / nF_tot;
		cout << endl;
	}

//	delete dcArrR;
	sleep(2); // DEBUG
	for(int i = 0; i < NITERGLB; i++){
		cout << "i: " << i << endl; sleep(1); // DEBUG
		hpull[i]->Write("", TObject::kOverwrite);
		hrt[i]->Write("", TObject::kOverwrite);
	}
	file->Close();
//	delete [] hpull; delete [] hrt;
//	getchar();
	return 0;
} // end of main function


