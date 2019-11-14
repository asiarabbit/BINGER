	// nuclide specific information //
	// experiment information
	const char *reaction = "240 MeV/u 13C->12C => 12C";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(0.944233, 0.00591414), pT(pL), pR(pL), pD(0.952913, 0.0165478);
	// contamination rate into the target nuclide zone
	const ExpData eL(0., 0.), eR(0.0186716, 0.000601986);
	ExpData eD(0.00482, 0.00064); eD *= 1.; // partically aligned vertically
	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(0.934552, 0.00718666);
	const ExpData sigma0(834.6038, 28.4348); // interaction c.s. in mb
	const ExpData e_SVT(Exp(sigma0*(-1e-27)*t5mmC)); // sigma0: interaction cross section

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(nC11, sqrt(nC11)), NT(nC12, sqrt(nC12)), NR(nC13, sqrt(nC13)), ND(nB10, sqrt(nB10));
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(nC11_n, sqrt(nC11_n)), NT_n(nC12_n, sqrt(nC12_n)), NR_n(nC13_n, sqrt(nC13_n)), ND_n(nB10_n, sqrt(nB10_n));

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(0.98, 0.01);

