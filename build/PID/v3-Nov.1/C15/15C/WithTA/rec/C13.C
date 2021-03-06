	// nuclide specific information //
	// experiment information
	const char *reaction = "230 MeV/u 15C->12C => 13C";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(0.967, 0.0096), pT(0.964, 0.0027), pR(0.964, 0.0036), pD(0.964, 0.0100);
	// contamination rate into the target nuclide zone
	const ExpData eL(0.012, 0.0), eR(0.0, 0.0);
	ExpData eD(0.0016, 0.0); eD *= 0.5; // partically aligned vertically
	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(0.9290, 0.0137), e_SVT(0.956, 0.017);

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(nC12, sqrt(nC12)), NT(nC13, sqrt(nC13)), NR(nC14, sqrt(nC14)), ND(nB11, sqrt(nB11));
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(nC12_n, sqrt(nC12_n)), NT_n(nC13_n, sqrt(nC13_n)), NR_n(nC14_n, sqrt(nC14_n)), ND_n(nB11_n, sqrt(nB11_n));

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(0.844, 0.005);

