	// nuclide specific information //
	// experiment information
	const char *reaction = "240 MeV/u 16C->12C => 15B";
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
	const ExpData NL(nB14, sqrt(nB14)), NT(nB15, sqrt(nB15)), NR(nB16, sqrt(nB16)), ND(nBe12, sqrt(nBe12));
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(nB14_n, sqrt(nB14_n)), NT_n(nB15_n, sqrt(nB15_n)), NR_n(nB16_n, sqrt(nB16_n)), ND_n(nBe12_n, sqrt(nBe12_n));

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(0.99560, 0.0042);

