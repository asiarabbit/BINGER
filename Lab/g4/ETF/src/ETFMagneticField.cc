// ETFMagnetciField.cc -- implementation of the class
// created: Jan. 25, 2019, transcribed by Asia SUN, asia.rabbit@163.com
// Last modified: Jan. 25, 2019, by Aisa SUN

#include <fstream>

#include "ETFMagneticField.hh"

#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

// trilinear interpolation
static G4double trilinear(const G4double *f, const G4double *dp);

ETFMagneticField::ETFMagneticField()
 : G4MagneticField(), fMessenger(nullptr), fIsUniformMagneticField(false),
   fBy(-1.0*tesla), fIsFileLoaded(false){
	memset(fB, 0, sizeof(fB));
	if(!fIsUniformMagneticField)
		LoadMagneticFieldFile("magnetic.txt");

	// define commands for this clas
	DefineCommands();

//	fIsUniformMagneticField = true;

}

ETFMagneticField::~ETFMagneticField(){
	delete fMessenger;
}

// position and time
void ETFMagneticField::GetFieldValue(const G4double p[4], G4double *bField) const{
	if(fIsUniformMagneticField){
		bField[0] = 0.;
		bField[1] = fBy;
		bField[2] = 0.;
	}
	else{
		G4double pp[4] = {p[0], p[1], p[2], 0.};
//		pp[2] += 1.*m; // translate the field for -1m in z-direction
		GetMagneticIntensity(pp, bField);
	}
}

void ETFMagneticField::DefineCommands(){
	// define /ETF/field command directory using generic messenger class
	fMessenger = new G4GenericMessenger(this, "/ETF/field/", "Field control");

	// field value command. Type: embedded class of G4
	auto& valueCmd = fMessenger->DeclareMethodWithUnit("value", "tesla", &ETFMagneticField::SetField, "set field strength");
	valueCmd.SetParameterName("field", true);
	valueCmd.SetDefaultValue("-1.");
}

// read the magnetic field measurement file, generated from measurements
// using Hall sensor, ASSIGN fB ARRAY
void ETFMagneticField::LoadMagneticFieldFile(const G4String &file){
	if(fIsFileLoaded) return;

	const char *filename = file.c_str();
	std::ifstream inFile(filename);
	if(!inFile.is_open()){
		G4cout << "ERROR: ETFMagneticField::LoadMagneticFieldFile():" << G4endl;
		G4cout << "File " << filename << " doesn't exist. Exiting...\n";
		exit(EXIT_FAILURE);
	} // end if
	G4String s_t;
	for(G4int i = 5; i--;) getline(inFile, s_t); // skip the commentary lines

	G4double current, x, y, z, Bx, By, Bz, Ihall, Tpt; // y(x)
	while(inFile >> current >> x >> y >> z >> Bx >> By >> Bz >> Ihall >> Tpt){
		if(fabs(Bx) > 1E50 || fabs(By) > 1E50 || fabs(By) > 1E50){
			G4cout << "ETFMagneticField::LoadMagneticFieldFile(): \
				Abnormal B value: (" << G4BestUnit(Bx, "Magnetic flux density")
				<< ", " << G4BestUnit(By, "Magnetic flux density")
				<< ", " << G4BestUnit(Bz, "Magnetic flux density") << ")"
				<< G4endl;
		}
		G4int i = G4int(-x/10.); // x: -1000, 0
		G4int j = G4int(y/10.); // y: 0, 90
		G4int k = G4int(z/10.); // z: -20, 1050
		if(i >= 0 && i <= 100
		&& j >= 0 && j <= 9
		&& k >= 0 && k <= 105){
			fB[0][i][j][k] = Bx;
			fB[1][i][j][k] = By;
			fB[2][i][j][k] = Bz;
		} // end if
		while(inFile.get() != '\n') continue; // skip the rest of the line
	} // end while
	if(inFile.eof()){
		G4cout << "ETFMagneticField::LoadMagneticFieldFile: \
			End of the file reached.\nFile " << filename << " loaded successfully."
			<< G4endl;
		fIsFileLoaded = true;
	} // end if
	else if(inFile.fail()){
		G4cout << "ETFMagneticField::LoadMagneticFieldFile\
			: Input terminated by data mismatch." << G4endl;
		fIsFileLoaded = false;
	} // end else if
	else{
		G4cout << "ETFMagneticField::LoadMagneticFieldFile: WTF?" << G4endl;
		while(1) getchar();
	} // end else
	inFile.close();
} // end of function LoadMagneticFieldFile

// assign B with the magnetic intensity vector at position (p[0], p[1], p[2])
// and time p[3]
G4bool ETFMagneticField::GetMagneticIntensity(const G4double p[4], G4double *B) const{
	// the position coordinates are deprived of their signs,
	// because of the symmetry of the magnetic field, the magnetic filed
	// intensity is irrelevant to the signs of the position coordinates.
	G4double pp[3] = {fabs(p[0]), fabs(p[1]), fabs(p[2])};

	// Expand the magnet field with measured B using extrapolation // ***********************
	// extrapolation for B at Y greater than 90mm and X greater than 390mm in the magnet
	if(pp[2] < 475.){ // inside the cavity
		if(pp[1] >= 90. && pp[1] <= 200.) pp[1] = 90. - 1E-2; // 170+30, including the V error
		if(pp[0] >= 390. && pp[0] <= 550.) pp[0] = 390. - 1E-2; // 500+50, including the H error
	}
	if(pp[2] >= 475. && pp[2] <= 690.){ // between the iron wall (z=475) and the z=690 plane
		if(pp[0] >= 390. && pp[0] < 700.){
			if(pp[2] - 690. < pp[0] - 390.) pp[2] = 690. + 1E-2; // z-ward extrapolation
			else pp[0] = 390. - 1E-2; // x-ward extrapolation
		}
		if(pp[1] >= 90. && pp[1] < 400.) pp[1] = 90. - 1E-2; // FEEL MY ECSTASY ~~~~~~~
	}
	if(pp[2] > 690.){ // out of the inner area
		if(pp[1] >= 90. && pp[1] < 500.) pp[1] = 90. - 1E-2; // AND YOU TOGETHER - ESCTASY ~~~
	}
	// Expand the magnet field with measured B using extrapolation // ***********************

	G4int ii[3] = {G4int(pp[0]), G4int(pp[1]), G4int(pp[2])};
	for(G4int &x : ii) x /= 10;
	G4double dp[3] = {pp[0]-ii[0]*10., pp[1]-ii[1]*10., pp[2]-ii[2]*10.}; // for interpolation
	G4double B_t[3][8]{}; // [xyz][8 vetices]
	G4bool isOut = false;
	static const G4double BMax = 1.21127*tesla;
	const G4double scale = std::fabs(fBy / BMax);

	if(ii[0] >= 0 && ii[0] < 100
	&& ii[1] >= 0 && ii[1] < 9
	&& ii[2] >= 0 && ii[2] < 105){
		for(G4int i = 0; i < 3; i++){ // loop over x, y and z
			// plane in the back
			B_t[i][0] = fB[i][ii[0]][ii[1]][ii[2]];
			B_t[i][1] = fB[i][ii[0]+1][ii[1]][ii[2]];
			B_t[i][2] = fB[i][ii[0]][ii[1]+1][ii[2]];
			B_t[i][3] = fB[i][ii[0]+1][ii[1]+1][ii[2]];
			// plane in the front
			B_t[i][4] = fB[i][ii[0]][ii[1]][ii[2]+1];
			B_t[i][5] = fB[i][ii[0]+1][ii[1]][ii[2]+1];
			B_t[i][6] = fB[i][ii[0]][ii[1]+1][ii[2]+1];
			B_t[i][7] = fB[i][ii[0]+1][ii[1]+1][ii[2]+1];
		} // end for over i
		// XXX: the maximum field is 1.2111 tesla
		B[0] = trilinear(B_t[0], dp)*gauss*scale;
		B[1] = trilinear(B_t[1], dp)*gauss*scale;
		B[2] = trilinear(B_t[2], dp)*gauss*scale;
	} // end if(...&&...&&...)
	else{
		B[0] = 0.; B[1] = 0.; B[2] = 0.;
		isOut = true;
	} // end else

	return isOut;
} // end of function GetMagneticIntensity


// trilinear interpolation
G4double trilinear(const G4double *f, const G4double *dp){
	G4double fx[4], fy[2], fz; // temporary variables
	// interpolation along x-axis
	for(G4int i = 0; i < 4; i++)
		fx[i] = (10.-dp[0])/10.*f[2*i] + dp[0]/10.*f[2*i+1];
	// interpolation along y-axis
	for(G4int i = 0; i < 2; i++)
		fy[i] = (10.-dp[1])/10.*fx[2*i] + dp[1]/10.*fx[2*i+1];
	// interpolation along z-axis
	fz = (10.-dp[2])/10.*fy[0] + dp[2]/10.*fy[1];
	
	return fz;
} // end of function trilinear interpolation

