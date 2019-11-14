	// nuclide specific information //
	// experiment information
	const char *reaction = "230 MeV/u 16C->12C => 14B";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(0.9053, 0.0047), pT(pL), pR(pL), pD(0.9039, 0.0084);
	// contamination rate into the target nuclide zone
	const ExpData eL(0.01632, 0.00024), eR(0.01110, 0.00020);
	ExpData eD(0.02800, 0.00032); eD *= 0.7; // partically aligned vertically
	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(0.8788, 0.0061), e_SVT(0.9635, 0.0082);

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(nB13, sqrt(nB13)), NT(nB14, sqrt(nB14)), NR(nB15, sqrt(nB15)), ND(nBe11, sqrt(nBe11));
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(nB13_n, sqrt(nB13_n)), NT_n(nB14_n, sqrt(nB14_n)), NR_n(nB15_n, sqrt(nB15_n)), ND_n(nBe11_n, sqrt(nBe11_n));

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(0.99997, 0.00010);

