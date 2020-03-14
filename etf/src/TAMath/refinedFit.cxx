///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAMath/refinedFit.C												 //
//   refinedFit.C -- source file for method TAMath::refinedFit						 //
//   Introduction: to get the bm and km from array set (x, y, r) using iterative and //
// direct research method. This is a coordinate alternation method, time-consuming	 //
// yet precise in two-dimensional minimization.										 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/9.															     //
// Last modified: 2018/4/26, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

//#define FAST
// x0,y0: geometirc center of the MWDC array
double TAMath::refinedFit(const double *x, const double *y, const double *r, double &k, double &b, int gGOOD, const int *LAYER, int isTest, double x0, double y0, double d2ThrePerDot, bool isDEBUG){
//	cout << "Enters refinedFit." << endl; getchar(); // DEBUG

	// prepare for the refined fit
	int i, j, ki; // temporary loop control variables
//	static int failedcnt = 0; // count of events obstinately difficult to resolve.
	const double sigma0 = clp->GetSimSpatialResolution();
	double sigma = sigma0; // um;
	double n_d2Thre = gGOOD;
	if(gGOOD == 2) n_d2Thre = 4; // count of fired andoe layers.
	double d2MAX = pow(sigma / 1000., 2) * n_d2Thre; // the expected MWDC position resolution.


//	for(int ii = 0; ii < 6; ii++) cout << fixed << setprecision(9) << "r[" << ii << "] = " << r[ii] << endl; // DEBUG
//	getchar(); // DEBUG

//////////// lalalalalalalalalalalalalalalalalalalalalalalalalalalalalalalalalalalalalalalala ////
	// prepare for the refined fit
	double kc = 0., bc = 0.;
	double d2min, d2 = 0.;
//	d2min = Dsquare(x, y, kc, bc, gGOOD, LAYER, d2ThrePerDot); // deprecated.
	d2min = iterativeFit(x, y, r, kc, bc, gGOOD, LAYER, d2ThrePerDot); // set the iteration origin.
//	if(isDEBUG){ // DEBUG
//		cout << "kc: " << kc << "\tbc: " << bc << endl; // DEBUG
//	} // end if // DEBUG
	double kt = 0., bt = 0.; // linear parameters of the temporary line
	double k0 = kc, b0 = bc; // start of the iteration
	double km = kc, bm = bc; // the optimal linear parameter set
	if(TACtrlPara::Instance()->IsCoarseFit()){ // if use dsquare as the fit method.
		k = km; b = bm; return d2min;
	} // end if
		
	double theta0 = 25. * DEGREE(); // span of theta variation. unit: degree. 25. * Pi / 180.
	double theta = theta0;
	double thetai = 0.;
	double denoi = 0.; // the denominator

	int n = 50;
	int nn = 50;
	double d0 = 12.; // span of d variation. unit: mm
	double d = d0, di = 0.;

	int nnn = 8;
	int loop = 0;
	int loopM = 8, nIncre = 10, nnIncre = 10, nnnM = 30; // parameters to control calculation precision.
	int loop1 = 0, loop1M = 5; // parameters to control calculation precision.
	switch(isTest){ // switch calculation precision according to the global variable isTest.
		case -1:
			loopM = 5;
			nIncre = 5;
			nnIncre = 5;
			nnnM = 8;
			sigma = sigma0*2.; // um;
			d2MAX = pow(sigma / 1000., 2) * n_d2Thre; // the expected MWDC position resolution.
			break;
		case 0:
			loopM = 6;
			nIncre = 20;
			nnIncre = 20;
			nnnM = 12;
			sigma = sigma0*0.5; // um;
			d2MAX = pow(sigma / 1000., 2) * n_d2Thre; // the expected MWDC position resolution.
			break;
		case 1:
			loopM = 8;
			nIncre = 50;
			nnIncre = 50;
			nnnM = 16;
			sigma = sigma0*0.5;
			d2MAX = pow(sigma / 1000., 2) * n_d2Thre;
			break;
		case 2:
			loopM = 10;
			nIncre = 50;
			nnIncre = 50;
			nnnM = 20;
			sigma = sigma0*0.5;
			d2MAX = pow(sigma / 1000., 2) * n_d2Thre;
			break;
		case 3:
			loopM = 20;
			nIncre = 100;
			nnIncre = 100;
			nnnM = 25;
			sigma = sigma0*0.5;
			d2MAX = pow(sigma / 1000., 2) * n_d2Thre;
			break;
		default: // default is reserved for test, and with the lowest calculation precision.
			loopM = 2;
			nIncre = 3;
			nnIncre = 3;
			nnnM = 4;
			sigma = sigma0*2.;
			d2MAX = pow(sigma / 1000., 2) * n_d2Thre;
			break;
	} // end of switch
	nnn = nnnM;
//	cout << "d2min: " << d2min << endl; // DEBUG
//	cout << "d2MAX: " << d2MAX << endl; // DEBUG
//	getchar(); // DEBUG
	while(d2min > d2MAX){ // HEHE // 0.2 * 0.2 * 6 = 0.24, sigma = 200um
		n += nIncre; // HEHE
		nn += nnIncre; // HEHE
		theta = theta0; d = d0; // HEHE
		k0 = km; b0 = bm; // HEHE
		loop++; // HEHE
//		cout << endl << "loop " << loop << endl; // HEHE
//		cout << "n = " << n << " nn = " << nn << endl; // DEBUG
//		getchar(); // DEBUG
		for(j = 0; j < nnn; j++){
			////////////////// to rotate the initial line around (x0, y0) step by step firstly ///////////////////
			for(i = 0; i <= n; i++){
				thetai = theta * (2. * i - n) / n;
				denoi = cos(thetai) - k0 * sin(thetai);
				kt = (k0 * cos(thetai) + sin(thetai)) / denoi;
				if(fabs(kt) > 1E4){
					if(kt > 1E4)
						kt = 1E4;
					else if(kt < -1E4)
						kt = -1E4;
					bt = -kt * x[0];
				} // end for over i
				else bt = (k0 * x0 + b0 - y0 + (y0 - k0 * x0) * cos(thetai) - (k0 * y0 + x0) * sin(thetai)) / denoi;
				d2 = minid2(x, y, r, kt, bt, gGOOD, LAYER);
//				cout << "d2 = " << d2 << endl; getchar(); // DEBUG
				if(d2 < d2min){
					d2min = d2;
					km = kt;
					bm = bt;
				} // end if
#ifdef FAST
				if(d2min <= d2MAX) break; // FAST
#endif
			} // end for over i
			
			kt = km; // translational motion, kt would not be modified
			b0 = bm;
//			cout << "kt = " << kt << " b0 = " << b0 << endl; getchar(); // DEBUG
			//////////////////////////// to move the line along its normal step by step ////////////////////////////////
			for(i = 0; i <= nn; i++){
				di = d * (2. * i - nn) / nn;
				bt = b0 + di * sqrt(kt * kt + 1.); // move upward for distance d along normal of the line.
				d2 = minid2(x, y, r, kt, bt, gGOOD, LAYER);
//				cout << "kt = " << kt; // DEBUG
//				cout << "bt = " << bt << endl; getchar(); // DEBUG
//				cout << " d2 = " << d2 << endl; getchar(); // DEBUG
				if(d2 < d2min){
					d2min = d2;
					bm = bt;
				}
#ifdef FAST
				if(d2min <= d2MAX) break; // FAST
#endif
			} // end for over i
			// refresh the iteration start
			k0 = km;
			b0 = bm;
			// narrow the search scope
			theta /= (0.4 * n);
			d /= (0.4 * nn);
		} // end for over j
//		cout << "At the end of loop " << loop; // HEHE // DEBUG
//		cout << " d2min = " << d2min << endl; // DEBUG
		if(loop > loopM){
//			cout << "loop0 can't do it." << endl << endl << endl;
			break; // turn to another minimization method.
		}
	} // HEHE // end of the first while
//			if(isDEBUG){ // DEBUG
//				cout << "km: " << km << "\tbm: " << bm << endl; // DEBUG
//				getchar(); // DEBUG
//			} // end if // DEBUG
	
	///////////////////////////// the enhanced method ///////////////////////////////////
	switch(isTest){ // switch calculation precision according to the global variable isTest.
		case -1:
			loop1M = 3;
			nIncre = 5;
			nnIncre = 5;
			nnnM = 8;
			sigma = sigma0*2.; // um;
			d2MAX = pow(sigma / 1000., 2) * n_d2Thre; // the expected MWDC position resolution.
			break;
		case 0:
			loop1M = 3;
			nIncre = 10;
			nnIncre = 10;
			nnnM = 12;
			sigma = sigma0; // um;
			d2MAX = pow(sigma / 1000., 2) * n_d2Thre; // the expected MWDC position resolution.
			break;
		case 1:
			loop1M = 5;
			nIncre = 15;
			nnIncre = 15;
			nnnM = 16;
			sigma = sigma0;
			d2MAX = pow(sigma / 1000., 2) * n_d2Thre;
			break;
		case 2:
			loop1M = 8;
			nIncre = 20;
			nnIncre = 20;
			nnnM = 20;
			sigma = sigma0*0.5;
			d2MAX = pow(sigma / 1000., 2) * n_d2Thre;
			break;
		case 3:
			loop1M = 15;
			nIncre = 40;
			nnIncre = 30;
			nnnM = 25;
			sigma = sigma0*0.5;
			d2MAX = pow(sigma / 1000., 2) * n_d2Thre;
			break;
		default: // default is reserved for test, and with the lowest calculation precision.
			loopM = 2;
			nIncre = 3;
			nnIncre = 3;
			nnnM = 4;
			sigma = sigma0*2.;
			d2MAX = pow(sigma / 1000., 2) * n_d2Thre;
			break;
	} // end of switch
//	if(0)
//	int tempCnt = 0; // DEBUG
	if(loop > loopM){
		n = 2; nn = 2; nnn = nnnM;
//		cout << "d2min: " << d2min << endl; // DEBUG
//		cout << "d2MAX: " << d2MAX << endl; // DEBUG
//		getchar(); // DEBUG
		while(d2min > d2MAX){ // HEHE
			n += nIncre; // HEHE
			nn += nnIncre; // HEHE
			theta = theta0; d = d0;
			k0 = km; b0 = bm;
			loop1++;
//			cout << "loop1 " << loop1 << endl; // HEHE // DEBUG
//			cout << "n = " << n << " nn = " << nn << endl; // DEBUG
//			getchar(); // DEBUG
			for(j = 0; j <= nnn; j++){
				////////////////// to rotate the initial line around (x0, y0) step by step firstly ///////////////////
				for(i = 0; i <= n; i++){
					for(ki = 0; ki <= nn; ki++){
						thetai = theta * (2. * i - n) / n;
						denoi = cos(thetai) - k0 * sin(thetai);
						kt = (k0 * cos(thetai) + sin(thetai)) / denoi;
						if(fabs(kt) > 1E4){
							if(kt > 1E4)
								kt = 1E4;
							else if(kt < -1E4)
								kt = -1E4;
							bt = -kt * x[0];
						}
						else bt = (k0 * x0 + b0 - y0 + (y0 - k0 * x0) * cos(thetai) - (k0 * y0 + x0) * sin(thetai)) / denoi;
	
					//////////////////////////// to move the line along its normal step by step ////////////////////////////////
						di = d * (2. * ki - nn) / nn;
						bt += di * sqrt(kt * kt + 1.); // move upward for distance d along normal of the line.
						d2 = minid2(x, y, r, kt, bt, gGOOD, LAYER);
						if(d2 < d2min){
							d2min = d2;
							km = kt;
							bm = bt;
						}
//						if(tempCnt++ < 500){ // DEBUG
//							cout << "tempCnt: " << tempCnt << "\td2: " << d2 << endl; // DEBUG
//							getchar(); // DEBUG
//						} // DEBUG
#ifdef FAST
						if(d2min <= d2MAX) break; // FAST
#endif
					} // end for over ki
				} // end for over i
				// refresh the iteration start
				k0 = km;
				b0 = bm;
//			if(isDEBUG){ // DEBUG
//				cout << "km: " << km << "\tbm: " << bm << endl; // DEBUG
//			} // end if // DEBUG
				// narrow the search scope
				theta /= (0.4 * n);
				d /= (0.4 * nn);
//				if(d2min < 0.24) break; // 0.2 * 0.2 * 6 = 0.24, sigma = 200um
			} // end for over j
//			cout << "At the end of loop1 " << loop1; // HEHE
//			cout << " d2min = " << d2min << endl;
//			getchar(); // DEBUG

			// the desired resolution hasn't been reached
			// after 1000 iteration. So let's just end all of this.
			// Probably this is the best resolution of the detector.
			if(loop1 > loop1M){
//				failedcnt++;
//				cout << failedcnt << endl;
//				cout << "This is limp. d2min = " << d2min;
//				cout << " count: " << failedcnt << endl; // << endl;
				break;
			}
		} // end of the second while
	} // end if(loop > loopM)
//////////// lalalalalalalalalalalalalalalalalalalalalalalalalalalalalalalalalalalalalalalala ////////
//			if(isDEBUG){ // DEBUG
//				cout << "Exiting the refinedFit function..." << endl;
//				cout << "km: " << km << "\tbm: " << bm << endl; // DEBUG
//			} // end if // DEBUG



//	cout << "km = " << km << " bm = " << bm << endl; // DEBUG
//	cout << "The final dsquare is " << d2min << endl; // DEBUG
//	getchar(); // DEBUG
	
	k = km;
	b = bm;
//	if(isDEBUG){ // DEBUG
//		cout << "k: " << k << "b: " << b << endl; // DEBUG
//	} // end if // DEBUG
	return d2min;
} // end of function refinedFit(...)

