	// nuclide specific information //
	// experiment information
	const char *reaction = "240 MeV/u 12C->12C => 10B";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(0.8771, 0.0113), pT(pL), pR(0.903, 0.013), pD(0.9039, 0.0084);
	// contamination rate into the target nuclide zone
	const ExpData eL(0.01632, 0.00024), eR(0.01110, 0.00020);
	ExpData eD(0.02800, 0.00032); eD *= 1.; // partically aligned vertically
	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(0.8958, 0.0164);
	const ExpData sigma0(766.5016, 17.7258); // interaction c.s. in mb
	const ExpData e_SVT(Exp(sigma0*(-1e-27)*t5mmC)); // sigma0: interaction cross section

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(nB9, sqrt(nB9)), NT(nB10, sqrt(nB10)), NR(nB11, sqrt(nB11)), ND(nBe8, sqrt(nBe8));
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(nB9_n, sqrt(nB9_n)), NT_n(nB10_n, sqrt(nB10_n)), NR_n(nB11_n, sqrt(nB11_n)), ND_n(nBe8_n, sqrt(nBe8_n));

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(0.99286, 0.0039);

