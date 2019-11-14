	// nuclide specific information //
	// experiment information
	const char *reaction = "240 MeV/u 16C->12C => 14C";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(0.9573, 0.0034), pT(pL), pR(pL), pD(0.9624, 0.0050);
	// contamination rate into the target nuclide zone
	const ExpData eL(0., 0.), eR(0.0180, 0.0003);
	ExpData eD(0.00086, 0.00011); eD *= 0.5; // partically aligned vertically
	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(0.8729, 0.0042), e_SVT(0.9761, 0.0021);

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(nC13, sqrt(nC13)), NT(nC14, sqrt(nC14)), NR(nC15, sqrt(nC15)), ND(nB12, sqrt(nB12));
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(nC13_n, sqrt(nC13_n)), NT_n(nC14_n, sqrt(nC14_n)), NR_n(nC15_n, sqrt(nC15_n)), ND_n(nB12_n, sqrt(nB12_n));

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(0.9578, 0.001);

