// localfit.C -- local fit for millepede algorithm
//			cout << "Before, p:\n"; // DEBUG
//			for(int j = NG; j < NT; j++) cout << "p[" << j << "]: " << p[j] << endl; // DEBUG
//			getchar(); // DEBUG
			
			// // // assign parameter derivative matrices // // //
//			cout << "Local fit, assigning A matrices.\n"; getchar(); // DEBUG
//			cout << "f.Eval(a[tmp], p):\n";
			int tmp = 0;
			for(int l = 0; l < 3; l++){ // loop over X-U-V
				for(int j = 0; j < 6; j++){
					if(nu[trkId[l][jj]][j] != -1){ // one measure point
						a[tmp][0] = j + 6 * l; a[tmp][1] = nu[trkId[l][jj]][j];
						for(int k = 0; k < NG; k++) // loop over global parameters
							Ag[tmp*NG+k] = f.DerivativePar(k, a[tmp], p);
						for(int k = 0; k < NL; k++) // loop over local parameters
							Al[tmp*NL+k] = f.DerivativePar(k + NG, a[tmp], p);
						rserial[tmp] = r[trkId[l][jj]][j];
						mri[tmp][0] = rserial[tmp] - f.Eval(a[tmp], p);
//						cout << " " << f.Eval(a[tmp], p); // DEBUG
						tmp++;
					} // end if
				} // end for over j
			} // end for over l
			// // // assign parameter derivative matrices // // //

			// the track quality of the pattern recognition
			if(iterLoc == 0 && iter == 0)
				chi20 += (mri.Transpose()*mW*mri)[0][0]; // ndf: (nF-NL); // DEBUG

			mAgi.SetData(Ag); mAli.SetData(Al);
			mGammai = mAli.Transpose()*mW*mAli;
			mbetai = mAli.Transpose()*mW*mri;
			mGammaiInv = mGammai.Invert();
			malphai = mGammaiInv*mbetai;
			malphai.AppendData(p+NG); // update local parameters :):):)

			/////////// calculate pull and downweigh outliers ///////////
			mW.SetData(W);
			TAMatrixSym mVp = mAli*mGammaiInv*mAli.Transpose(); // covariance matrix of the fitted measurements
			for(int j = 0; j < nF; j++){
				mrip[j][0] = rserial[j] - f.Eval(a[j], p);
				pull[j] = mrip[j][0]/sqrt(fabs(1./mW[j][j] - mVp[j][j]));
				// after at least one global fit and local fit
				if(iter >= 1 && iterLoc >= 1 && fabs(pull[j]) > 3.){ // 3 sigma
					mW[j][j] = 1./(mrip[j][0]*mrip[j][0]); // sigma = infinite <=> 1/sigma^2 = 0.
					if(iterLoc == 1) outlierCnt[iter - 1]++; // before downweighing outliers
				} // end if
				if(iterLoc == NITERLCL - 1){ // the last local fit
					hpull[iter]->Fill(pull[j]);
					hrt[iter]->Fill(rserial[j], mrip[j][0]);
					// assign dr for STRCorrection
					int l = int(a[j][0]) / 6, j_ = int(a[j][0]) % 6;
					dr[trkId[l][jj]][j_] = -mrip[j][0]; // dr = rc - r
					pull_[trkId[l][jj]][j_] = pull[j];
				} // end if
			} // end for over j
			if(iterLoc == NITERLCL - 1){
				chi2Loc[jj] = (mrip.Transpose()*mW*mrip)[0][0];
				chi2 += chi2Loc[jj]; // ndf: (nF-NL); // DEBUG
				chi2LocOverNdf[jj] = chi2Loc[jj]/(nF-NL);
			}
			/////////// calculate pull and downweigh outliers ///////////


			///// ADDTIONAL ITERATION CHECK /////////
			// upon the last fit the result is still not satisfying
			if(iterLoc == NITERLCL - 1){
				bool isBadFit = false;
				if(fabs(malphai[0][0]) > 1e-5) isBadFit = true;
				if(fabs(malphai[1][0]) > 1e-5) isBadFit = true;
				if(isBadFit){
					if(additionalFitCnt < 4) iterLoc--;
					additionalFitCnt++;
				}
			} // end if
//			cout << "iterLoc: " << iterLoc << "\tadditionalFitCnt: " << additionalFitCnt << endl; getchar(); // DEBUG
			///// ADDTIONAL ITERATION CHECK /////////





#ifdef DEBUG
//			cout << "After, p:\n"; // DEBUG
//			for(int j = NG; j < NT; j++) cout << "p[" << j << "]: " << p[j] << endl; getchar(); // DEBUG
			
//			cout << "\nmAgi.Show():\n"; getchar(); // DEBUG
//			mAgi.Show(); getchar(); // DEBUG
//			cout << "mAli.Show():\n"; getchar(); // DEBUG
//			mAli.Show(); getchar(); // DEBUG
			cout << "Local fit " << iterLoc << " result:\n"; // DEBUG
			cout << "mri.Show():\n"; getchar(); // DEBUG
			mri.Show(); getchar(); // DEBUG
//			cout << "mGammai.Show():\n"; // DEBUG
//			mGammai.Show(); getchar(); // DEBUG
//			cout << "mGammaiInv.Show():\n"; // DEBUG
//			mGammaiInv.Show(); getchar(); // DEBUG
//			cout << "(mGammaiInv*mGammai).Show():\n"; // DEBUG
//			(mGammaiInv*mGammai).Show(); getchar(); // DEBUG
			cout << "malphai.Show():\n"; // DEBUG
			malphai.Show(); getchar(); // DEBUG
			chi2Loc[jj] = (mri.Transpose()*mW*mri)[0][0];
			chi2LocOverNdf[jj] = chi2Loc[jj]/(nF-NL);
			cout << "chi2Loc: " << chi2Loc[jj] << "\tchi2Loc/ndf: " << chi2LocOverNdf[jj] << endl << endl;
#endif




