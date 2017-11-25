///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// BINGER/inc/etf/TAMath/bfgs.C														 //
//   bfgs4.C -- source file for method TAMath::BFGS4 and TAMath::dSkew				 //
//   Introduction: BFGS algorithm for unrestrained multi-dimensional minimization	 //
// problems. This is one is dedicated to four-dimension minmization, which can be 	 //
// easily adapted to minimization of other dimensions. This function is dedicated 	 //
// for 3-D linear tracking.															 //
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2017/10/9.															     //
// Last modified: 2017/10/9, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017, SUN Yazhou.												     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////


static double fun(const double *xk, const double Ag[][3], const double ag[][3], const double *p, const double *r, const int nF); // calculate chi2 of a track, evenly weighted
static void funxy(double *gk, const double *xk, const double Ag[][3], const double ag[][3], const double *p, const double *r, const int nF); // derivative of function fun over track parameters
static void goldCut(double *xk, const double Ag[][3], const double ag[][3], const double *p, const double *r, const int nF, const double *pk); // a one-dimensional minimization method
static void Ak1(double (*Ak)[4], const double *dxk, const double *dgk); // CORE of the BFGS algorithm

//#define DEBUG

double TAMath::BFGS4(const double Ag[][3], const double ag[][3], double *p, const double *r, const int nF){
	// start of the iteration; xk[0]: dk1, xk[1]: dk2, xk[2]: db1, xk[3]: db2
	double xk[4]{}; // relative search length vector
	double xkm[4]{}; // the optimal xk vector.
	double gk[4]{}; // gradient (derivative vector) of the objective function

	// TODO: the error control constant is to be reevaluated.
	const double dxM = 1E-5; // error control constant ||dx||: variation of relative search length
	const double gM = 1E-2; // error control constant ||gk||: length of function derivative
	double d2 = 0., d2min = 1E200; // the minimum value of fun.
	double dx = 1E200, g = 1E200; // dx = ||(xk1 - xk)||, g = ||(gk1)||
	double dxk[4]{}, dgk[4]{}; // xk1 - xk, gk1 - gk

//	cout << "The initial d2min: " << fun(xk, Ag, ag, p, r, nF); getchar(); // DEBUG
	// the so-called Ak matrix: an asymptotic substitute for Hesse matrix
	double Ak[4][4]{}; for(int i = 0; i < 4; i++) Ak[i][i] = 1.; // initialized to a unit matrix
	funxy(gk, xk, Ag, ag, p, r, nF); // assign gk
	double pk[4] = { // pk = -Ak.gk
		-(Ak[0][0] * gk[0] + Ak[0][1] * gk[1] + Ak[0][2] * gk[2] + Ak[0][3] * gk[3]),
		-(Ak[1][0] * gk[0] + Ak[1][1] * gk[1] + Ak[1][2] * gk[2] + Ak[1][3] * gk[3]),
		-(Ak[2][0] * gk[0] + Ak[2][1] * gk[1] + Ak[2][2] * gk[2] + Ak[2][3] * gk[3]),
		-(Ak[3][0] * gk[0] + Ak[3][1] * gk[1] + Ak[3][2] * gk[2] + Ak[3][3] * gk[3])
	};

	int k = 0, i = 0, reincarnated = 0;
	jj = ii; // DEBUG
	while(i < 4000)
	{
		i++;
		memcpy(dxk, xk, sizeof(xk)); // dxk = xk
		goldCut(xk, Ag, ag, p, r, nF, pk); // refresh xk -> the optimal search step length
		for(int i = 0; i < 4; i++) dxk[i] = xk[i] - dxk[i]; // dxk = xk1 - xk
		dx = sqrt(dxk[0] * dxk[0] + dxk[1] * dxk[1] + dxk[2] * dxk[2] + dxk[3] * dxk[3]);

		memcpy(dgk, gk, sizeof(gk)); // dgk = gk
		funxy(gk, xk, Ag, ag, p, r, nF); // refresh gk
		for(int i = 0; i < 4; i++) dgk[i] = gk[i] - dgk[i]; // dgk = gk1 - gk;
		g = sqrt(gk[0] * gk[0] + gk[1] * gk[1] + gk[2] * gk[2] + gk[3] * gk[3]);

		if(dx < dxM && g < gM){
			if(fabs(xk[0]) < 0.50) break;
			else if(reincarnated > 300) break;
		}
		d2 = fun(xk, Ag, ag, p, r, nF);
		if(d2 < d2min){
			d2min = d2;
			memcpy(xkm, xk, sizeof(xk));
#ifdef DEBUG
			cout << "d2min: " << d2min << endl; getchar(); // DEBUG
#endif
		}
		if(k == 4){ // a spoiled start needs a special treatment.
			k = 0;
			memset(Ak, 0, sizeof(Ak));
			for(int i = 0; i < 4; i++) Ak[i][i] = 1.;
//			cout << "Ak:\n"; // DEBUG
//			for(int i = 0; i < 4; i++){ // DEBUG
//				for(double x : Ak[i]) cout << x << " "; cout << endl; // DEBUG
//			} // DEBUG
			reincarnated++; // DEBUG
		}
		else{
			Ak1(Ak, dxk, dgk); // refresh Ak
			k++;
		} // end else
		// pk = -Ak.gk -> the vmm search direction
		pk[0] = -(Ak[0][0] * gk[0] + Ak[0][1] * gk[1] + Ak[0][2] * gk[2] + Ak[0][3] * gk[3]);
		pk[1] = -(Ak[1][0] * gk[0] + Ak[1][1] * gk[1] + Ak[1][2] * gk[2] + Ak[1][3] * gk[3]);
		pk[2] = -(Ak[2][0] * gk[0] + Ak[2][1] * gk[1] + Ak[2][2] * gk[2] + Ak[2][3] * gk[3]);
		pk[3] = -(Ak[3][0] * gk[0] + Ak[3][1] * gk[1] + Ak[3][2] * gk[2] + Ak[3][3] * gk[3]);
//		cout << "pk[0]: " << pk[0] << " pk[1]: " << pk[1] << endl; // DEBUG
//		cout << "pk[2]: " << pk[2] << " pk[3]: " << pk[3] << endl; // DEBUG
//		getchar(); // DEBUG
	} // end of while
	for(int i = 0; i < 4; i++) p[i] *= 1. + xkm[i];

#ifdef DEBUG
	// adopt the final resolved result
	cout << "xkm[0]: " << xkm[0] << " xkm[1]: " << xkm[1] << endl; // DEBUG
	cout << "xkm[2]: " << xkm[2] << " xkm[3]: " << xkm[3] << endl; // DEBUG
	getchar(); // DEBUG
	cout << "The iteration is implemented " << i << " times, "; // DEBUG
	cout << "reincarnated " << reincarnated << " times." << endl; // DEBUG
	cout << "dx = " << dx << "\tg = " << g << endl; // DEBUG
	cout << "The minimum of fun(...) is "; // DEBUG
	cout << "fun(" << xk[0] << ", " << xk[1] << ", " << xk[2] << ", " << xk[3] << ") = "; // DEBUG
	cout << d2min << endl; // DEBUG

	cout << setprecision(9); // DEBUG
	cout << "______fun has been called " << ii - jj << " times." << endl; // DEBUG
	cout << "ii: " << ii << "\tjj: " << jj << endl; // DEBUG
	getchar(); // DEBUG
#endif

	return d2min;	
} // end of function refinedFit

// from point xk to find min xk1 and assign lambda.
void goldCut(double *xk, const double Ag[][3], const double ag[][3], const double *p, const double *r, const int nF, const double *pk){
	double pkL = sqrt(pk[0] * pk[0] + pk[1] * pk[1] + pk[2] * pk[2] + pk[3] * pk[3]);
	double ppk[4] = {pk[0]/pkL, pk[1]/pkL, pk[2]/pkL, pk[3]/pkL};
	
	double a = 0., b = 0.008; // a and b is the border of the search domain b: 0.03
	double x = b - TAMath::Alpha() * (b - a); // interpolation point one
	double xp = a + TAMath::Alpha() * (b - a); // interpolation point two
	const double dfM = 1E-10; // error control constant. unit: mm 12
	const double dxM = 1E-8; // error control constant. unit: mm 9
	double xx[4]{};
	// x = x0 + x * pk
	// x: the search step length, pk: the optimal search direction, analogous to Newton method
	for(int i = 0; i < 4; i++) xx[i] = xk[i] + x  * ppk[i];
	double f = fun(xx, Ag, ag, p, r, nF);
	for(int i = 0; i < 4; i++) xx[i] = xk[i] + xp * ppk[i];
	double fp = fun(xx, Ag, ag, p, r, nF);

	while(fabs(fp - f) > dfM || fabs(xp - x) > dxM){
		if(f < fp){ // minimal in (a, xp), xp=>b, x=>xp
			b = xp;
			xp = x;
			fp = f;
			x = b - TAMath::Alpha() * (b - a);
			
			for(int i = 0; i < 4; i++) xx[i] = xk[i] + x  * ppk[i];
			f = fun(xx, Ag, ag, p, r, nF);
		}
		else{ // minimal in (x, b), x=>a, xp=>x
			a = x;
			x = xp;
			f = fp;
			xp = a + TAMath::Alpha() * (b - a);

			for(int i = 0; i < 4; i++) xx[i] = xk[i] + xp * ppk[i];
			fp = fun(xx, Ag, ag, p, r, nF);
		}
	} // end while
	double xstar = (x + xp) / 2.;
	// apply the obtained optimal search length.
	for(int i = 0; i < 4; i++) xk[i] += xstar * ppk[i];
} // end of function goldCut

// calculate chi2 of a track, evenly weighted
// Ag[nF][3], ag[nF][3], p[4], r[nF].
// xk[4]: the real variables of the functino fun, correction to p[4].
double fun(const double *xk, const double Ag[][3], const double ag[][3], const double *p, const double *r, const int nF){
	ii++; // DEBUG
	double dr[nF]{}, pp[4]{}, d2 = 0.;
//	cout << "xk[0]: " << xk[0] << " xk[1]: " << xk[1] << " xk[2]: " << xk[2] << " xk[3]: " << xk[3] << endl; // DEBUG
//	cout << "p[0]: " << p[0] << " p[1]: " << p[1] << " p[2]: " << p[2] << " p[3]: " << p[3] << endl; // DEBUG
	pp[0] = p[0] * (1 + xk[0]); pp[1] = p[1] * (1 + xk[1]); // k1, k2
	pp[2] = p[2] * (1 + xk[2]); pp[3] = p[3] * (1 + xk[3]);	// b1, b2
	for(int i = 0; i < nF; i++){
		dr[i] = TAMath::dSkew(Ag[i], ag[i], pp) - r[i];
//		cout << "i: " << i << " dr[i]: " << dr[i] << endl; // DEBUG
//		getchar(); // DEBUG
		d2 += dr[i] * dr[i];
	} // end for over i
	return d2;
} // en d function fun

// derivarives of function fun over xk[4], which would be assigned to array gk[4].
void funxy(double *gk, const double *xk, const double Ag[][3], const double ag[][3], const double *p, const double *r, const int nF){
	double xxp[4]{}, xxm[4]{};
	double xdelta[4] = {1E-8, 1E-8, 1E-7, 1E-7}; // dx to calculate derivatives
	for(int i = 0; i < 4; i++){ // loop to calculate the i-th derivative
		memcpy(xxp, xk, sizeof(xxp)); xxp[i] += xdelta[i];
		memcpy(xxm, xk, sizeof(xxm)); xxm[i] -= xdelta[i];
		gk[i] = (fun(xxp, Ag, ag, p, r, nF) - fun(xxm, Ag, ag, p, r, nF)) / (2. * xdelta[i]);
	} // end for over i
//	cout << "gk[0]: " << gk[0] << " gk[1]: " << gk[1] << " gk[2]: " << gk[2] << " gk[3]: " << gk[3] << endl; getchar(); // DEBUG
} // end function funx

// distance between two skew lines
// Ag[3], ag[3], p[4] = {k1, k2, b1, b2}; B[3], b[3]: counterpart of Ag and ag for another line
double TAMath::dSkew(const double *ag, const double *Ag, const double *p){
	// 3-D track line parameters. B[3]: one point in the line; b[3]: direction vector
	double b[3] = {0., 0., 1.}, B[3] = {0., 0., 0.}; // B[2] = 0. and b[2] = 1.; preset values.
	B[0] = p[2]+B[2]*p[0]; // B[0] = b1+B[2]*k1;
	B[1] = p[3]+B[2]*p[1]; // B[1] = b2+B[2]*k2;
	b[0] = b[2]*p[0]; // b[0] = b[2]*k1;
	b[1] = b[2]*p[1]; // b[1] = b[2]*k2;
	return dSkew(ag, Ag, b, B);
}
double TAMath::dSkew(const double *ag, const double *Ag, const double *b, const double *B){
	double bb = sqrt(b[0]*b[0]+b[1]*b[1]+b[2]*b[2]); // DEBUG
	double ab[3] = // cross product of vector ag and b.  (dR.R.al)×b
	 {ag[1]*b[2]-ag[2]*b[1], ag[2]*b[0]-ag[0]*b[2], ag[0]*b[1]-ag[1]*b[0]};
	// (B-Ag).(ag×b)/|ag×b|
	return fabs((B[0]-Ag[0])*ab[0]+(B[1]-Ag[1])*ab[1]+(B[2]-Ag[2])*ab[2])/
	 sqrt(ab[0]*ab[0]+ab[1]*ab[1]+ab[2]*ab[2]);
} // end of function dSkew

///////////////////////// refresh Ak -- CORE of the BFGS algorithm //////////////////////////////////
void Ak1(double (*Ak)[4], const double *dxk, const double *dgk){
	double Bk[4][4] = { // Bk = dxk.dxkT, Bkd: Bkdenominator = dxkT.dgk
		{dxk[0] * dxk[0], dxk[0] * dxk[1], dxk[0] * dxk[2], dxk[0] * dxk[3]},
		{dxk[1] * dxk[0], dxk[1] * dxk[1], dxk[1] * dxk[2], dxk[1] * dxk[3]},
		{dxk[2] * dxk[0], dxk[2] * dxk[1], dxk[2] * dxk[2], dxk[2] * dxk[3]},
		{dxk[3] * dxk[0], dxk[3] * dxk[1], dxk[3] * dxk[2], dxk[3] * dxk[3]}
	}, Bkd = dxk[0] * dgk[0] + dxk[1] * dgk[1] + dxk[2] * dgk[2] + dxk[3] * dgk[3];
	double zk[4] = { // zk = Ak.dgk
		Ak[0][0] * dgk[0] + Ak[0][1] * dgk[1] + Ak[0][2] * dgk[2] + Ak[0][3] * dgk[3],
		Ak[1][0] * dgk[0] + Ak[1][1] * dgk[1] + Ak[1][2] * dgk[2] + Ak[1][3] * dgk[3],
		Ak[2][0] * dgk[0] + Ak[2][1] * dgk[1] + Ak[2][2] * dgk[2] + Ak[2][3] * dgk[3],
		Ak[3][0] * dgk[0] + Ak[3][1] * dgk[1] + Ak[3][2] * dgk[2] + Ak[3][3] * dgk[3]
	};
	double Ck[4][4]  = { // Ck = zk.zkT, Ckd: Ckdenominator = zkT.dgk
		{zk[0] * zk[0], zk[0] * zk[1], zk[0] * zk[2], zk[0] * zk[3]},
		{zk[1] * zk[0], zk[1] * zk[1], zk[1] * zk[2], zk[1] * zk[3]},
		{zk[2] * zk[0], zk[2] * zk[1], zk[2] * zk[2], zk[2] * zk[3]},
		{zk[3] * zk[0], zk[3] * zk[1], zk[3] * zk[2], zk[3] * zk[3]}
	}, Ckd = zk[0] * dgk[0] + zk[1] * dgk[1] + zk[2] * dgk[2] + zk[3] * dgk[3];

	double xzk[4][4]; // xzk = dxk.zkT + zk.dxkT
	for(int i = 0; i < 4; i++) for(int j = 0; j <= i; j++)
		xzk[i][j] = dxk[i] * zk[j] + zk[i] * dxk[j];
	for(int i = 0; i < 4; i++) for(int j = i + 1; j < 4; j++)
		xzk[i][j] = xzk[j][i];
	// BFGS algorithm
	double BBC = (Bkd + Ckd) / Bkd;
	for(int i = 0; i < 4; i++) for(int j = 0; j < 4; j++)
		Ak[i][j] += (BBC * Bk[i][j] - xzk[i][j]) / Bkd;

//	cout << "Bkd: " << Bkd << "\tCkd: " << Ckd << "\tBBC: " << BBC << endl; // DEBUG
//	cout << "xzk:\n"; // DEBUG
//	for(int i = 0; i < 4; i++){ // DEBUG
//		for(int j = 0; j < 4; j++) cout << setw(15) << xzk[i][j] << " "; // DEBUG
//		cout << endl; // DEBUG
//	} // DEBUG
//	cout << "Ak:\n"; // DEBUG
//	for(int i = 0; i < 4; i++){ // DEBUG
//		for(int j = 0; j < 4; j++) cout << setw(15) << Ak[i][j] << " "; // DEBUG
//		cout << endl; // DEBUG
//	} // DEBUG
//	getchar(); // DEBUG
} // end of function Ak1






