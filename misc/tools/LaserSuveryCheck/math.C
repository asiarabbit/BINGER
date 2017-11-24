// math.C -- to help pinpoint the detector position

double L(const double *p0, const double *p1){
	if((0. == p1[0] && 0. == p1[1] && 0. == p1[2]) || (0. == p0[0] && 0. == p0[1] && 0. == p0[2])){
//		cout << "WRONG" << endl; getchar(); // DEBUG
		return -1.;
	}
	double d[3]{}, sum = 0.;
	for(int i = 3; i--;){ // loop over x-y-z
		d[i] = p0[i] - p1[i];
		sum += d[i] * d[i];
	}

#ifdef DEBUG
	cout << "p0: ";
	for(int i = 0; i < 3; i++) cout << p0[i] << "\t"; cout << endl;
	cout << "p1: ";
	for(int i = 0; i < 3; i++) cout << p1[i] << "\t"; cout << endl;
	cout << "L: " << sqrt(sum) << endl; getchar();
#endif

	return sqrt(sum);
}

void BEE(int i, int j, int k0, int k1, double err, short opt){
	char LR[] = "LR";
	cout << "DC arr#" << LR[i] << "-DC#" << j;
	cout << "\tk0: " << k0 << "\tk1: " << k1 << "\t\033[31;1merr: " << err << "\tOpt: " << opt << "\033[0m";
	cout << "\a";
	getchar();
}

// phi angle using two horizontal points
double phi(const double *p0, const double *p1){
	if((0. == p1[0] && 0. == p1[1] && 0. == p1[2]) || (0. == p0[0] && 0. == p0[1] && 0. == p0[2])){
		cout << "WRONG" << endl; getchar(); // DEBUG
		return -1.;
	}
	return atan(-(p0[2]-p1[2])/(p0[0]-p1[0])) / DEGREE;
}
// yaw angle
double yaw(const double *p0, const double *p1){ return phi(p0, p1); }
// theta angle using two horizontal points
double theta(const double *p0, const double *p1){
	if((0. == p1[0] && 0. == p1[1] && 0. == p1[2]) || (0. == p0[0] && 0. == p0[1] && 0. == p0[2])){
		cout << "WRONG" << endl; getchar(); // DEBUG
		return -1.;
	}
	return atan((p0[1]-p1[1])/(p0[0]-p1[0])) / DEGREE;
}
// roll angle
double roll(const double *p0, const double *p1){ return theta(p0, p1); }
double pitch(const double *p0, const double *p1){
	if((0. == p1[0] && 0. == p1[1] && 0. == p1[2]) || (0. == p0[0] && 0. == p0[1] && 0. == p0[2])){
		cout << "WRONG" << endl; getchar(); // DEBUG
		return -1.;
	}
	return atan(-(p0[1]-p1[1])/(p0[2]-p1[2])) / DEGREE;
}
// signed distance of a point to a line
double d(const double *p0, const double *p1, const double *p){
	if((0. == p1[0] && 0. == p1[1] && 0. == p1[2]) || (0. == p0[0] && 0. == p0[1] && 0. == p0[2])){
//		cout << "WRONG" << endl; getchar(); // DEBUG
		return -1.;
	}
	double s[3] = {p0[0]-p1[0], p0[1]-p1[1], p0[2]-p1[2]}; // vector of l:p0-p1
	double m[3] = {p[0]-p1[0], p[1]-p1[1], p[2]-p1[2]}; // vector of p-p1
	double c[3] = {s[1]*m[2]-s[2]*m[1], s[2]*m[0]-s[0]*m[2], s[0]*m[1]-s[1]*m[0]}; // s cross m
	return sqrt(c[0]*c[0]+c[1]*c[1]+c[2]*c[2]) / sqrt(s[0]*s[0]+s[1]*s[1]+s[2]*s[2]); // |c|/|s|
}
// signed distance of a point to a plane
double dp(const double *p0, const double *p1, const double *p2, const double *p){
	if((0. == p1[0] && 0. == p1[1] && 0. == p1[2]) || (0. == p0[0] && 0. == p0[1] && 0. == p0[2])){
//		cout << "WRONG" << endl; getchar(); // DEBUG
		return -1.;
	}
	if((0. == p2[0] && 0. == p2[1] && 0. == p2[2]) || (0. == p[0] && 0. == p[1] && 0. == p[2])){
//		cout << "WRONG" << endl; getchar(); // DEBUG
		return -1.;
	}
	double s0[3] = {p0[0]-p1[0], p0[1]-p1[1], p0[2]-p1[2]}; // vector of l:p0-p1
	double s1[3] = {p1[0]-p2[0], p1[1]-p2[1], p1[2]-p2[2]}; // vector of l:p1-p2
	double m[3] = {p[0]-p1[0], p[1]-p1[1], p[2]-p1[2]}; // vector of p-p1
	double c[3] = {s0[1]*s1[2]-s0[2]*s1[1],s0[2]*s1[0]-s0[0]*s1[2],s0[0]*s1[1]-s0[1]*s1[0]}; // s0×s1
	return fabs(c[0]*m[0]+c[1]*m[1]+c[2]*m[2]) / sqrt(c[0]*c[0]+c[1]*c[1]+c[2]*c[2]); // |c|/|s|
}
// show vector p0->p1
void vec(const double *p0, const double *p1){
	if((0. == p1[0] && 0. == p1[1] && 0. == p1[2]) || (0. == p0[0] && 0. == p0[1] && 0. == p0[2])){
//		cout << "WRONG" << endl; getchar(); // DEBUG
		return;
	}
	cout << "v: ";
	for(int i = 0; i < 3; i++) cout << p1[i]-p0[i] << " ";
	cout << endl;
}
// output vector p0->p1
void vec(const double *p0, const double *p1, double *p){
	if((0. == p1[0] && 0. == p1[1] && 0. == p1[2]) || (0. == p0[0] && 0. == p0[1] && 0. == p0[2])){
//		cout << "WRONG" << endl; getchar(); // DEBUG
		return;
	}
	for(int i = 0; i < 3; i++) p[i] = p1[i]-p0[i];
}

// angle0: yaw, angle1: pitch, angle2: roll
void rotate(const double *angIn, const double *pIn, double *pOut){
	double s1 = sin(angIn[0]), s2 = sin(angIn[1]), s3 = sin(angIn[2]);
	double c1 = cos(angIn[0]), c2 = cos(angIn[1]), c3 = cos(angIn[2]);
	pOut[0] = (c1*c3-s1*s2*s3)*  pIn[0] -c2*s3* pIn[1] +(c1*s2*s3+s1*c3)* pIn[2];
	pOut[1] = (c1*s3+s1*s2*c3)*  pIn[0] +c2*c3* pIn[1] +(s1*s3-c1*s2*c3)* pIn[2];
	pOut[2] = -s1*c2*            pIn[0] +s2*    pIn[1] +c1*c2*            pIn[2];	
}
// r_global = R.r_local+r0
void globalPos(const double *p0, const double *angIn, const double *pIn, double *pOut){
	rotate(angIn, pIn, pOut);
	pOut[0] += p0[0]; pOut[1] += p0[1]; pOut[2] += p0[2];
}
double chi2(const double *p0, const double *p1){
	return (p0[0]-p1[0])*(p0[0]-p1[0]) + (p0[1]-p1[1])*(p0[1]-p1[1]) + (p0[2]-p1[2])*(p0[2]-p1[2]);
}









