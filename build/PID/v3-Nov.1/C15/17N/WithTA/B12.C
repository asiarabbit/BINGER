	// nuclide specific information //
	// experiment information
	const char *reaction = "230 MeV/u 17N->12C => 12B";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(0.903, 0.013), pT(0.9052, 0.0100), pR(0.894, 0.012), pD(0.9039, 0.0084);
	// contamination rate into the target nuclide zone
	const ExpData eL(0.01632, 0.00024), eR(0.01110, 0.00020);
	ExpData eD(0.02800, 0.00032); eD *= 0.2; // partically aligned vertically
	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(0.8512, 0.0126), e_SVT(0.964, 0.017);

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(nB11, sqrt(nB11)), NT(nB12, sqrt(nB12)), NR(nB13, sqrt(nB13)), ND(nBe10, sqrt(nBe10));
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(nB11_n, sqrt(nB11_n)), NT_n(nB12_n, sqrt(nB12_n)), NR_n(nB13_n, sqrt(nB13_n)), ND_n(nBe10_n, sqrt(nBe10_n));

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(0.98983, 0.00270);

