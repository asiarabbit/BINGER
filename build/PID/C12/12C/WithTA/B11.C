	// nuclide specific information //
	// experiment information
	const char *reaction = "230 MeV/u 12C->12C => 11B";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(0.8771, 0.0113), pT(0.903, 0.013), pR(0.896, 0.013), pD(0.9039, 0.0084);
	// contamination rate into the target nuclide zone
	const ExpData eL(0.01632, 0.00024), eR(0.01110, 0.00020);
	ExpData eD(0.02800, 0.00032); eD *= 0.5; // partically aligned vertically
	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(0.8834, 0.0165), e_SVT(0.965, 0.022);

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(nB10, sqrt(nB10)), NT(nB11, sqrt(nB11)), NR(nB12, sqrt(nB12)), ND(nBe9, sqrt(nBe9));
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(nB10_n, sqrt(nB10_n)), NT_n(nB11_n, sqrt(nB11_n)), NR_n(nB12_n, sqrt(nB12_n)), ND_n(nBe9_n, sqrt(nBe9_n));

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(0.97932, 0.00551);

