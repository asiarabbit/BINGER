///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAMath/bfgs.C														 //
//   bfgs.C -- source file for method TAMath::refinedFitBFGS						 //
//   Introduction: BFGS algorithm for unrestrained multi-dimensional minimization	 //
// problems. This is one is dedicated to two-dimension minmization, which can be 	 //
// easily adapted to minimization of other dimensions.								 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/9.															     //
// Last modified: 2018/3/22, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

static double fun(const double *xk, const double *par, int gGOOD, const int *LAYER);
static void funxy(double *xk, double *par, double *gk, int gGOOD, const int *LAYER);
static void goldCut(double *xk, double *par, double *gk, int gGOOD, const int *LAYER);
static void Ak1(double (*Ak)[2], double *dxk, double *dgk); // to refresh Ak
static unsigned long long ii = 0, jj = 0; // DEBUG

double TAMath::refinedFitBFGS(const double *x, const double *y, const double *r, double &kL, double &bL, int gGOOD, const int *LAYER, double d2PerDot){
	double par[20] = {0.};
	//	par[18] = k0; par[19] = b0;
	//	Dsquare(x, y, par[18], par[19], gGOOD, LAYER, d2PerDot); // deprecated
	// set the iteration origin
//	cout << "Mark 1" << endl; getchar(); // DEBUG
	double d2min; // the minimum value of fun.
	d2min = iterativeFit(x, y, r, par[18], par[19], gGOOD, LAYER, d2PerDot);
//	cout << "TACtrlPara::Instance()->IsCoarseFit(): " << TACtrlPara::Instance()->IsCoarseFit() << endl; getchar(); // DEBUG
	if(TACtrlPara::Instance()->IsCoarseFit()){ // if use dsquare as the fit method
		kL = par[18]; bL = par[19]; return d2min;
	} // end if
	for(int i = 0; i < 6; i++){
		par[i]      = x[i];
		par[6 + i]  = y[i];
		par[12 + i] = r[i];
	}

	double xk[2] = {0., 0.}; // start of the iteration; xk[0]: dk, xk[1]: db
//	cout << "Initially, fun(" << xk[0] << ", "; // DEBUG
//	cout << xk[1] << ") = " << fun(xk, par, gGOOD, LAYER) << endl; // DEBUG

	const double dxM = 1E-5; // error control constant
	const double gM = 1E-2; // error control constant
	double d2 = 0.; // chi2
	double xkm[2] = {0., 0.}; // the optimal xk set.
	double dx = 1E200, g = 1E200; // dx = ||(xk1 - xk)||, g = ||(gk1)||
	double dxk[2] = {0.}; // xk1 - xk
	double dgk[2] = {0.}; // gk1 - gk

	double Ak[2][2] =
	{
		{1., 0.},
		{0., 1.}
	};
	double gk[2] = {0.};
	funxy(xk, par, gk, gGOOD, LAYER); // assign gk
	double pk[2] = { // pk = -Ak.gk
		-(Ak[0][0] * gk[0] + Ak[0][1] * gk[1]),
		-(Ak[1][0] * gk[0] + Ak[1][1] * gk[1])
	};

	int k = 0, i = 0;
	int reincarnated = 0; // DEBUG
	jj = ii; // DEBUG
	while(i < 1000)
	{
		i++;
		dxk[0] = -xk[0]; dxk[1] = -xk[1]; // dxk = -xk
		goldCut(xk, par, pk, gGOOD, LAYER); // refresh xk
		dxk[0] += xk[0]; dxk[1] += xk[1]; // dxk = xk1 - xk
		dx = sqrt(dxk[0] * dxk[0] + dxk[1] * dxk[1]);
//		cout << "dx = " << dx << endl; getchar(); // DEBUG
		
		dgk[0] = -gk[0]; dgk[1] = -gk[1]; // dgk = -gk
		funxy(xk, par, gk, gGOOD, LAYER); // refresh gk
		dgk[0] += gk[0]; dgk[1] += gk[1]; // dgk = gk1 - gk;
		g = sqrt(gk[0] * gk[0] + gk[1] * gk[1]);
//		cout << "g = " << g << endl; getchar(); // DEBUG
//		cout << "xk[0]: " << xk[0] << "\txk[1]: " << xk[1] << endl; // DEBUG
//		cout << "gk[0]: " << gk[0] << "\tgk[1]: " << gk[1] << endl; // DEBUG
//		getchar(); // DEBUG
		d2 = fun(xk, par, gGOOD, LAYER);
		if(dx < dxM && g < gM){
			if(fabs(xk[0]) < 0.50) break;
			else if(reincarnated > 300) break; // a spoiled start needs a special treatment.
			//else {};
		}
		if(d2 < d2min){
			d2min = d2;
			xkm[0] = xk[0];
			xkm[1] = xk[1];
		}
		if(k == 2){
//			cout << "____________Start again_________" << endl; // DEBUG
			k = 0;
			Ak[0][0] = 1.; Ak[0][1] = 0.;
			Ak[1][0] = 0.; Ak[1][1] = 1.;
			reincarnated++; // DEBUG
		}
		else{
//			cout << "dxk[0]: " << dxk[0] << "\tdxk[1]: " << dxk[1] << endl; // DEBUG
//			cout << "dgk[0]: " << dgk[0] << "\tdgk[1]: " << dgk[1] << endl; // DEBUG
			Ak1(Ak, dxk, dgk);
			k++;
		}
		pk[0] = -(Ak[0][0] * gk[0] + Ak[0][1] * gk[1]); // pk = -Ak.gk
		pk[1] = -(Ak[1][0] * gk[0] + Ak[1][1] * gk[1]); // pk = -Ak.gk
	} // end of while
	kL = par[18] * (1. + xkm[0]); bL = par[19] * (1. + xkm[1]); // the final resolved result

//	cout << "The iteration is implemented " << i << " times, "; // DEBUG
//	cout << "reincarnated " << reincarnated << " times." << endl; // DEBUG
//	cout << "dx = " << dx << "\tg = " << g << endl; // DEBUG
//	cout << "The minimum of fun(x, y, gGOOD, LAYER) is "; // DEBUG
//	cout << "fun(" << xk[0] << ", " << xk[1] << ") = " << d2min << endl; // DEBUG

//	cout << setprecision(9); // DEBUG
//	cout << "xkm[0]: " << xkm[0] << "\txkm[1]: " << xkm[1] << endl; // DEBUG
//	cout << "k0: " << par[18] << "\tb0: " << par[19] << endl; // DEBUG
//	cout << "kL: " << kL << "\tbL: " << bL << endl; // DEBUG
//	cout << "______fun has been called " << ii - jj << " times." << endl; // DEBUG
//	cout << "d2min: " << d2min << endl; // DEBUG
//	cout << "ii: " << ii << "\tjj: " << jj << endl;
//	getchar(); // DEBUG

//	cout << "Mark 1" << endl; getchar(); // DEBUG

	return d2min;
	
} // end of function refinedFit


double fun(const double *xk, const double *par, int gGOOD, const int *LAYER){
	ii++; // DEBUG
//	double x[6], y[6], r[6];
//	for(int i = 0; i < 6; i++){
//		x[i] = par[i];
//		y[i] = par[6 + i];
//		r[i] = par[12 + i];
//	}
	 // k0 = par[18], b0 = par[19];
//	double kt = par[18] * (1. + xk[0]);
//	double bt = par[19] * (1. + xk[1]);
//	return TAMath::minid2(x, y, r, kt, bt, gGOOD, LAYER);
	return TAMath::minid2(par, par+6, par+12, par[18] * (1. + xk[0]), par[19] * (1. + xk[1]), gGOOD, LAYER);
}
// assign first partial derivatives of function fun to gk
void funxy(double *xk, double *par, double *gk, int gGOOD, const int *LAYER){ // note that k0 = par[18], b0 = par[19];
	double xxp[2] = {0.};
	double xxm[2] = {0.};
	double xdelta = 1E-8; // delta_k / k0
	xxp[0] = xk[0] + xdelta;
	xxp[1] = xk[1];
	xxm[0] = xk[0] - xdelta;
	xxm[1] = xk[1];
	gk[0] = (fun(xxp, par, gGOOD, LAYER) - fun(xxm, par, gGOOD, LAYER)) / (2. * xdelta);
	double ydelta = 1E-7; // delta_d / d0
//	cout << endl << "ydelta = " << ydelta << endl; // DEBUG
	xxp[0] = xk[0];
	xxp[1] = xk[1] + ydelta;
	xxm[0] = xk[0];
	xxm[1] = xk[1] - ydelta;
	gk[1] = (fun(xxp, par, gGOOD, LAYER) - fun(xxm, par, gGOOD, LAYER)) / (2. * ydelta);
}

// from point xk to find min xk1 and assign lambda.
void goldCut(double *xk, double *par, double *pk, int gGOOD, const int *LAYER){
//	cout << "Before normalized, pk[0]: " << pk[0] << " pk[1]: " << pk[1] << endl; // DEBUG
	double pkL = sqrt(pk[0] * pk[0] + pk[1] * pk[1]);
	if(pkL > 1.){ // normalize vector pk
		pk[0] /= pkL;
		pk[1] /= pkL;
	}
	
	double a = 0., b = 0.008; // a and b is the border of the search domain b: 0.03
	double x = b - TAMath::Alpha() * (b - a); // interpolation point one
	double xp = a + TAMath::Alpha() * (b - a); // interpolation point two
	const double dfM = 1E-10; // error control constant. unit: mm 12
	const double dxM = 1E-8; // error control constant. unit: mm 9
	double xx[2] = {0.};
	xx[0] = xk[0] + x * pk[0];
	xx[1] = xk[1] + x * pk[1];
	double f = fun(xx, par, gGOOD, LAYER);
//	cout << "pk[0]: " << pk[0] << "\tpk[1]: " << pk[1] << endl; // DEBUG
//	cout << "xx[0]: " << xx[0] << "\txx[1]: " << xx[1] << endl; 
//	cout << "f = " << f << endl; // DEBUG
	xx[0] = xk[0] + xp * pk[0];
	xx[1] = xk[1] + xp * pk[1];
	double fp = fun(xx, par, gGOOD, LAYER);
//	cout << "xx[0]: " << xx[0] << "\txx[1]: " << xx[1] << endl; 
//	cout << "f = " << f << endl; // DEBUG
//	int cnt = 0; // DEBUG
	while(fabs(fp - f) > dfM || fabs(xp - x) > dxM){
		if(f < fp){
			b = xp;
			xp = x;
			fp = f;
			x = b - TAMath::Alpha() * (b - a);
			
			xx[0] = xk[0] + x * pk[0];
			xx[1] = xk[1] + x * pk[1];
			f = fun(xx, par, gGOOD, LAYER);
//			cout << "fun(" << xx[0] << ", "; // DEBUG
//			cout << xx[1] << ") = " << f << endl; // DEBUG
		}
		else{
			a = x;
			x = xp;
			f = fp;
			xp = a + TAMath::Alpha() * (b - a);

			xx[0] = xk[0] + xp * pk[0];
			xx[1] = xk[1] + xp * pk[1];
			fp = fun(xx, par, gGOOD, LAYER);
//			cout << "fun(" << xx[0] << ", "; // DEBUG
//			cout << xx[1] << ") = " << fp << endl; // DEBUG
		}
//		cout << "fabs(fp - f): " << fabs(fp - f); // DEBUG
//		cout << "\tfabs(xp - x): " << fabs(xp - x) << endl; // DEBUG
//		getchar(); // DEBUG
//		cnt++; // DEBUG
	}
//	cout << "goldcut cnt: " << cnt << endl; // DEBUG
//	cout << "fabs(fp - f): " << fabs(fp - f) << endl; // DEBUG
//	cout << "fabs(xp - x): " << fabs(xp - x) << endl; // DEBUG
	
	double xstar = (x + xp) / 2.;
	xk[0] += xstar * pk[0];
	xk[1] += xstar * pk[1];

//	cout << "xstar: " << xstar << endl; // DEBUG
//	cout << "After being refreshed, xk[0]: " << xk[0]; // DEBUG
//	cout << ", xk[1]: " << xk[1] << endl; // DEBUG
} // end of function goldCut
// refresh Ak
void Ak1(double (*Ak)[2], double *dxk, double *dgk){
	double Bk[2][2] = {
		{dxk[0] * dxk[0], dxk[0] * dxk[1]},
		{dxk[1] * dxk[0], dxk[1] * dxk[1]}
	}, Bkd = dxk[0] * dgk[0] + dxk[1] * dgk[1]; // Bkdenominator
	double zk[2] = { // zk = Ak.dgk
		Ak[0][0] * dgk[0] + Ak[0][1] * dgk[1], 
		Ak[1][0] * dgk[0] + Ak[1][1] * dgk[1]};
//	double Ck[2][2]  = {
//		{zk[0] * zk[0], zk[0] * zk[1]},
//		{zk[1] * zk[0], zk[1] * zk[1]}
//	};
	double Ckd = zk[0] * dgk[0] + zk[1] * dgk[1]; // Ckdenominator
	
	double xzk[2][2] = { // xzk = x.zT + z.xT
		{2. * dxk[0] * zk[0], dxk[0] * zk[1] + dxk[1] * zk[0]},
		{dxk[1] * zk[0] + dxk[0] * zk[1], 2. * dxk[1] * zk[1]}
	};
	// BFGS algorithm
	Ak[0][0] += ((Bkd + Ckd) * Bk[0][0] / Bkd - xzk[0][0]) / Bkd;
	Ak[0][1] += ((Bkd + Ckd) * Bk[0][1] / Bkd - xzk[0][1]) / Bkd;
	Ak[1][0] += ((Bkd + Ckd) * Bk[1][0] / Bkd - xzk[1][0]) / Bkd;
	Ak[1][1] += ((Bkd + Ckd) * Bk[1][1] / Bkd - xzk[1][1]) / Bkd;
//	cout << "Bk[0][0]: " << Bk[0][0] << "\tBk[0][1]: " << Bk[0][1] << endl; // DEBUG
//	cout << "Bk[1][0]: " << Bk[1][0] << "\tBk[1][1]: " << Bk[1][1] << endl; // DEBUG
//	cout << "Bkd: " << Bkd << endl; getchar(); // DEBUG
//	cout << "xzk[0][0]: " << xzk[0][0] << "\txzk[0][1]: " << xzk[0][1] << endl; // DEBUG
//	cout << "xzk[1][0]: " << xzk[1][0] << "\txzk[1][1]: " << xzk[1][1] << endl; // DEBUG
//	cout << "Ckd: " << Ckd << endl; getchar(); // DEBUG
	
}

//#include "deviation/six.C"

