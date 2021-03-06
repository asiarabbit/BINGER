/**
	\file TAMath.hpp
	\class TAMath
	\brief Definitions of the member template functions for class TAMath
	\date Created: 2019/9/28 Last revised: 2019/10/5, SUN Yazhou.
	\copyright 2017-2019, SUN Yazhou.
*/

//#define DEBUG_MATH

#include <iostream>
#include <iomanip>
#include <cstdlib>

using std::cout;
using std::endl;
using std::setprecision;

static unsigned long long jj4 = 0;

// a one-dimensional minimization method
// pk: the optimal search direction, xk: the minimal point in the search direction
template <typename FUNCTOR>
static void goldCut(FUNCTOR &fun, const double *pk, double *xk);

static void Ak1(double (*Ak)[4], const double *dxk, const double *dgk); // CORE of the BFGS algorithm

// the BFGS minimization algorithm
template <typename FUNCTOR>
double TAMath::BFGS4(FUNCTOR &fun){
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
	funxy4(fun, xk, gk); // assign gk
	double pk[4] = { // pk = -Ak.gk, the optimal search direction
		-(Ak[0][0] * gk[0] + Ak[0][1] * gk[1] + Ak[0][2] * gk[2] + Ak[0][3] * gk[3]),
		-(Ak[1][0] * gk[0] + Ak[1][1] * gk[1] + Ak[1][2] * gk[2] + Ak[1][3] * gk[3]),
		-(Ak[2][0] * gk[0] + Ak[2][1] * gk[1] + Ak[2][2] * gk[2] + Ak[2][3] * gk[3]),
		-(Ak[3][0] * gk[0] + Ak[3][1] * gk[1] + Ak[3][2] * gk[2] + Ak[3][3] * gk[3])
	};

	int k = 0, i = 0, reincarnated = 0;
	jj4 = kCallCnt; // DEBUG
	while(i < 4000)
	{
		i++;
		memcpy(dxk, xk, sizeof(xk)); // dxk = xk
		goldCut(fun, pk, xk); // refresh xk -> the optimal search step length
		for(int i = 0; i < 4; i++) dxk[i] = xk[i] - dxk[i]; // dxk = xk1 - xk
		dx = sqrt(dxk[0] * dxk[0] + dxk[1] * dxk[1] + dxk[2] * dxk[2] + dxk[3] * dxk[3]);

		memcpy(dgk, gk, sizeof(gk)); // dgk = gk
		funxy4(fun, xk, gk); // refresh gk
		for(int i = 0; i < 4; i++) dgk[i] = gk[i] - dgk[i]; // dgk = gk1 - gk;
		g = sqrt(gk[0] * gk[0] + gk[1] * gk[1] + gk[2] * gk[2] + gk[3] * gk[3]);

		d2 = fun(xk);
		if(d2 < d2min){
			d2min = d2;
			memcpy(xkm, xk, sizeof(xk));
#ifdef DEBUG_MATH
			cout << "d2min: " << d2min << endl; // DEBUG
//			getchar(); // DEBUG
#endif
		}
		if(dx < dxM && g < gM){
			if(fabs(xk[0]) < 0.50) break;
			else if(reincarnated > 300) break;
		}
		if(4 == k){ // a spoiled start needs a special treatment
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
//	for(int i = 0; i < 4; i++) p[i] *= 1. + xkm[i];

#ifdef DEBUG_MATH
	// adopt the final resolved result
	cout << "xkm[0]: " << xkm[0] << " xkm[1]: " << xkm[1] << endl; // DEBUG
	cout << "xkm[2]: " << xkm[2] << " xkm[3]: " << xkm[3] << endl; // DEBUG
	getchar(); // DEBUG
	cout << "The iteration is implemented " << i << " times, "; // DEBUG
	cout << "reincarnated " << reincarnated << " times." << endl; // DEBUG
	cout << "dx = " << dx << "\tg = " << g << endl; // DEBUG
	cout << "The minimum of fun(...) is "; // DEBUG
	cout << "fun(" << xkm[0] << ", " << xkm[1] << ", " << xkm[2] << ", " << xkm[3] << ") = "; // DEBUG
	cout << d2min << "\tfun(xkm): " << fun(xkm) << "\t" << fun(xk) << endl; // DEBUG

	cout << setprecision(9); // DEBUG
	cout << "______fun has been called " << kCallCnt - jj4 << " times." << endl; // DEBUG
	cout << "kCallCnt: " << kCallCnt << "\tjj4: " << jj4 << endl; // DEBUG
	getchar(); // DEBUG
#endif

	fun.UpdatePar(xkm);

	return d2min;
} // end of function BFGSFit


// from point xk to find min xk1 and assign lambda
// pk is the search direction
template <typename FUNCTOR>
void goldCut(FUNCTOR &fun, const double *pk, double *xk){
	double pkL = sqrt(pk[0] * pk[0] + pk[1] * pk[1] + pk[2] * pk[2] + pk[3] * pk[3]);
	double ppk[4] = {pk[0]/pkL, pk[1]/pkL, pk[2]/pkL, pk[3]/pkL};

	double a = 0., b = 0.01; // a and b is the border of the search domain b: 0.03
	double x = b - TAMath::Alpha() * (b - a); // interpolation point one
	double xp = a + TAMath::Alpha() * (b - a); // interpolation point two
	const double dfM = 1E-8; // error control constant. unit: mm 12
	const double dxM = 1E-6; // error control constant. unit: mm 9
	double xx[4]{};
	// x = x0 + x * pk
	// x: the search step length, pk: the optimal search direction, analogous to Newton method
	for(int i = 0; i < 4; i++) xx[i] = xk[i] + x  * ppk[i];
	double f = fun(xx);
	for(int i = 0; i < 4; i++) xx[i] = xk[i] + xp * ppk[i];
	double fp = fun(xx);

	while(fabs(fp - f) > dfM || fabs(xp - x) > dxM){
		if(f < fp){ // minimal in (a, xp), xp=>b, x=>xp
			b = xp;
			xp = x;
			fp = f;
			x = b - TAMath::Alpha() * (b - a);

			for(int i = 0; i < 4; i++) xx[i] = xk[i] + x  * ppk[i];
			f = fun(xx);
		}
		else{ // minimal in (x, b), x=>a, xp=>x
			a = x;
			x = xp;
			f = fp;
			xp = a + TAMath::Alpha() * (b - a);

			for(int i = 0; i < 4; i++) xx[i] = xk[i] + xp * ppk[i];
			fp = fun(xx);
		}
	} // end while
	double xstar = (x + xp) / 2.;
	// apply the obtained optimal search length.
	for(int i = 0; i < 4; i++) xk[i] += xstar * ppk[i];
} // end of function goldCut

//////////////////// refresh Ak -- CORE of the BFGS algorithm ////////////////
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
	double Ckd = zk[0] * dgk[0] + zk[1] * dgk[1] + zk[2] * dgk[2] + zk[3] * dgk[3];

	double xzk[4][4]; // xzk = dxk.zkT + zk.dxkT
	for(int i = 0; i < 4; i++) for(int j = 0; j <= i; j++)
		xzk[i][j] = dxk[i] * zk[j] + zk[i] * dxk[j];
	for(int i = 0; i < 4; i++) for(int j = i + 1; j < 4; j++)
		xzk[i][j] = xzk[j][i];
	// BFGS algorithm
	double BBC = (Bkd + Ckd) / Bkd;
	for(int i = 0; i < 4; i++) for(int j = 0; j < 4; j++)
		Ak[i][j] += (BBC * Bk[i][j] - xzk[i][j]) / Bkd;
} // end of function Ak1


// derivarives of function fun at xk[4], which would be assigned to array gk[4]
template <typename FUNCTOR>
void TAMath::funxy4(FUNCTOR &fun, const double *xk, double *gk){
	double xxp[4]{}, xxm[4]{};
	double xdelta[4] = {1E-8, 1E-8, 1E-7, 1E-7}; // dx to calculate derivatives
	for(int i = 0; i < 4; i++){ // loop to calculate the i-th derivative
		memcpy(xxp, xk, sizeof(xxp)); xxp[i] += xdelta[i];
		memcpy(xxm, xk, sizeof(xxm)); xxm[i] -= xdelta[i];
		gk[i] = (fun(xxp) - fun(xxm)) / (2. * xdelta[i]);
	} // end for over i
} // end function funx
