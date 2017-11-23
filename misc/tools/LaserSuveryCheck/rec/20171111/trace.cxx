// trace.cxx -- tracing the position and angles of a rigid body.

#include "header.C"

int main(){
	// [DCArrL-R][DC0-1-2][A0-1-2-3][x-y-z]; default is Back(facing MG), f -> Front
	// in the global coordinate system
	double A[2][3][4][3]{}, Af[2][3][4][3]{}, B[2][3][4][3]{}, Bf[2][3][4][3]{};
	// in the local coordinate system
	double Ai[2][3][4][3]{}, Aif[2][3][4][3]{}, Bi[2][3][4][3]{}, Bif[2][3][4][3]{};
	// [DCArrL-R][L-R][U-D][x-y-z], default is Back.
	double TOFWi[2][2][2][3]{}, TOFW[2][2][2][3]{};
	#include "inputLocal.C" // assign Ai, Bi, Aif, Bif (local coordinate system)
	#include "inputGlobal.C" // assign A, B, Af, Bf (global coordinate system)

	/////////////////////////// TEST //////////////////////////////////////////
	int i = 0, j = 0;
	show(B[i][j][3]);
	vec(A[i][j][1], B[i][j][1]);
	double dd = d(A[i][j][0], A[i][j][2], A[i][j][1]);
	double ddp = dp(A[i][j][1], B[i][j][0], B[i][j][2], B[i][j][1]);
	cout << "dd: " << dd << "\tddp: " << ddp << endl; getchar();
	/////////////////////////// TEST //////////////////////////////////////////


	return 0;
}


#include "math.C"
