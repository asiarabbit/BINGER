// test.C -- test script for matrix package written for millepede project

#include "TAMatrix.h"
#include "TAMatrixSym.h"

double fun(double *x, double *p); // objective function declaration

int main(){
	double data[9] = {1., 2., 3., 4., 5., 6., 7., 8., 9.};
	double dataI[9] = {1., 0., 0., 0., 1., 0., 0., 0., 1.};
	double dataSym[16] = {
			      1., 2., 3., 4., 
			      2., 5., 6., 7., 
			      3., 6., 8., 9., 
			      4., 7., 9., 10.,
			      };

	TAMatrixSym ms = TAMatrixSym(4, 4, dataSym);
	ms.Show();
	ms.Invert().Show();
	(ms*ms.Invert()).Show();

	return 0;
} // end of main function

double fun(double *x, double *p){
	return x[0]*x[0]*p[0];
}
