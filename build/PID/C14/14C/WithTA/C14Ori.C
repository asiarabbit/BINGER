	// nuclide specific information //
	// experiment information
	const char *reaction = "240 MeV/u 14C->12C => C14Ori";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(0.9478, 0.0026), pT(1., 0.), pR(pL), pD(0.944, 0.012);
	// contamination rate into the target nuclide zone
	const ExpData eL(0., 0.), eR(0.01563, 0.00024);
	ExpData eD(0.00482, 0.00064); eD *= 0.7; // partically aligned vertically
	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(0.8797, 0.0032), e_SVT(0.9761, 0.0021);

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(nC13, sqrt(nC13)), NT(nC14Ori, sqrt(nC14Ori)), NR(nC15, sqrt(nC15)), ND(nB12 sqrt(nB12));
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(nC13_n, sqrt(nC13_n)), NT_n(nC14Ori_n, sqrt(nC14Ori_n)), NR_n(nC15_n, sqrt(nC15_n)), ND_n(nB12_n, sqrt(nB12_n));

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(1., 0.);

