	// nuclide specific information //
	// experiment information
	const char *reaction = "240 MeV/u 16C->12C => 14B";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(0.956011, 0.004842), pT(pL), pR(pL), pD(0.956, 0.012);
	// contamination rate into the target nuclide zone
	const ExpData eL(0., 0.), eR(0.012853, 0.000404);
	ExpData eD(0.00049, 0.00020); eD *= 0.7; // partically aligned vertically
	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(0.888889, 0.0062123);
	const ExpData sigma0(912.6504, 23.8303); // interaction c.s. in mb
	const ExpData e_SVT(Exp(sigma0*(-1e-27)*t5mmC)); // sigma0: interaction cross section

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(nB13, sqrt(nB13)), NT(nB14, sqrt(nB14)), NR(nB15, sqrt(nB15)), ND(nBe11, sqrt(nBe11));
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(nB13_n, sqrt(nB13_n)), NT_n(nB14_n, sqrt(nB14_n)), NR_n(nB15_n, sqrt(nB15_n)), ND_n(nBe11_n, sqrt(nBe11_n));

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(0.99997, 0.00010);

