	// nuclide specific information //
	// experiment information
	const char *reaction = "230 MeV/u 16C->12C => 14C";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(0.964, 0.0096), pT(0.964, 0.0027), pR(0.964, 0.0036), pD(0.964, 0.0100);
	// contamination rate into the target nuclide zone
	const ExpData eL(0.015, 0.0), eR(0.0, 0.0);
	ExpData eD(0.00065, 0.0); eD *= 0.5; // partically aligned vertically
	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(0.9142, 0.0037), e_SVT(0.9626, 0.0048);

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(nC13, sqrt(nC13)), NT(nC14, sqrt(nC14)), NR(nC15, sqrt(nC15)), ND(nB12, sqrt(nB12));
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(nC13_n, sqrt(nC13_n)), NT_n(nC14_n, sqrt(nC14_n)), NR_n(nC15_n, sqrt(nC15_n)), ND_n(nB12_n, sqrt(nB12_n));

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(0.945, 0.007);

