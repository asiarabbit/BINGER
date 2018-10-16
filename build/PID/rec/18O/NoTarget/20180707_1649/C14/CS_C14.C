// CS: calculate cross section

// error propagation
// pp: assign the final result
void rate(double N1, double N2, double *pp = nullptr);
void errDiv(double x1, double dx1, double x2, double dx2, double *pp = nullptr);
void errProd(double x1, double dx1, double x2, double dx2, double *pp = nullptr);

void CS_C14(){

	// target information: 5mmC
	const double t = 4.5126e22, dt = 0.0027e22; // cm-2, target density
	const double N0 = 42176.; // number of total incident particles
	double deltaN = 2.; // target fragment count: C14


	// calculate efficiency
	double p[4]{}; // a temporary array
	double eff_e = 0.859, deff_e = 0.022; // detecting efficiency
	double eff_PU = 0.92420, deff_PU = 0.00059; // pileUp compensation
	double eff_g = 1., deff_g = 0.;
	errProd(eff_e, deff_e, eff_PU, deff_PU, p);
	errProd(p[0], p[1], eff_g, deff_g, &p[2]);
	const double eff = p[2], deff = p[3];
	cout << endl << "__________$$$$$$$$$$***********$$$$$$$$$$____________" << endl;
	cout << "total efficiency: " << eff << "(" << deff << ")" << endl << endl;

	errDiv(deltaN, sqrt(deltaN), eff, deff, p);
	errDiv(p[0], p[1], N0, sqrt(N0), &p[2]);
	errDiv(p[2], p[3], t, dt, p);
	const double cs = p[0]*1e27, dcs = p[1]*1e27; // unit: mb

	cout << endl;
	cout << "\033[32;1mThe final cross section is: ";
	cout << "\033[33;1m" << cs << "(" << dcs << ") mb." << endl << endl;
	cout << "\033[34;1mDONE\033[0m" << endl;
}




void rate(double N1, double N2, double *pp){ // pp: store result
	const double rate = N2 / N1;
	const double rel = sqrt(1. / N1 + 1. / N2);
	const double sigma = rate * rel;

	cout << "N1: " << N1 << "\tN2: " << N2 << endl;
	cout << "relative error: " << rel << endl;
	cout << rate << "(" << sigma << ")" << endl;

	pp[0] = rate; pp[1] = sigma;
}

void errDiv(double x1, double dx1, double x2, double dx2, double *pp){
	const double rate = x1/x2;
	const double rel1 = dx1/x1, rel2 = dx2/x2;
	const double rel = sqrt(rel1*rel1 + rel2*rel2);
	const double sigma = rate * rel;

	cout << "x1: " << x1 << "\tdx1: " << dx1 << endl;
	cout << "x2: " << x2 << "\tdx2: " << dx2 << endl;
	cout << "relative error: " << rel << endl;
	cout << rate << "(" << sigma << ")" << endl;

	pp[0] = rate; pp[1] = sigma;
}

void errProd(double x1, double dx1, double x2, double dx2, double *pp){
	const double prod = x1*x2;
	const double rel1 = dx1/x1, rel2 = dx2/x2;
	const double rel = sqrt(rel1*rel1 + rel2*rel2);
	const double sigma = prod * rel;

	cout << "x1: " << x1 << "\tdx1: " << dx1 << endl;
	cout << "x2: " << x2 << "\tdx2: " << dx2 << endl;
	cout << "relative error: " << rel << endl;
	cout << prod << "(" << sigma << ")" << endl;

	pp[0] = prod; pp[1] = sigma;

}

