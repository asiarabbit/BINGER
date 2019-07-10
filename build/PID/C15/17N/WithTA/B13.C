	// nuclide specific information //
	// experiment information
	const char *reaction = "230 MeV/u 17N->12C => 13B";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(0.9052, 0.0100), pT(0.894, 0.012), pR(pT), pD(0.9039, 0.0084);
	// contamination rate into the target nuclide zone
	const ExpData eL(0.01632, 0.00024), eR(0.01110, 0.00020);
	ExpData eD(0.02800, 0.00032); eD *= 0.2; // partically aligned vertically
	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(0.8788, 0.0061), e_SVT(0.9635, 0.0082);

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(nB12, sqrt(nB12)), NT(nB13, sqrt(nB13)), NR(nB14, sqrt(nB14)), ND(nBe11, sqrt(nBe11));
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(nB12_n, sqrt(nB12_n)), NT_n(nB13_n, sqrt(nB13_n)), NR_n(nB14_n, sqrt(nB14_n)), ND_n(nBe11_n, sqrt(nBe11_n));

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(1., 0.);

