	// nuclide specific information //
	// experiment information
	const char *reaction = "240 MeV/u 13C->12C => 12C";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(0.9455, 0.0061), pT(pL), pR(pL), pD(0.944, 0.012);
	// contamination rate into the target nuclide zone
	const ExpData eL(0., 0.), eR(0.01563, 0.00024);
	ExpData eD(0.00482, 0.00064); eD *= 1.; // partically aligned vertically
	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(0.8923, 0.0070), e_SVT(0.9771, 0.0037);

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(nC11, sqrt(nC11)), NT(nC12, sqrt(nC12)), NR(nC13, sqrt(nC13)), ND(nB10, sqrt(nB10));
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(nC11_n, sqrt(nC11_n)), NT_n(nC12_n, sqrt(nC12_n)), NR_n(nC13_n, sqrt(nC13_n)), ND_n(nB10_n, sqrt(nB10_n));

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(0.98, 0.01);

