	// nuclide specific information //
	// experiment information
	const char *reaction = "260 MeV/u 18O->12C => 12C";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(pO18), pT(pO18), pR(pO18), pD(pO18);
	// contamination rate into the target nuclide zone
	const ExpData eL(eL_C), eR(eR_C);
	ExpData eD(eD_C); eD *= 1.; // partically aligned vertically

	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(ed_T_C), e_SVT(0.9624, 0.0069);

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(nC11, sqrt(nC11)), NT(nC12, sqrt(nC12)), NR(nC13, sqrt(nC13)), ND(nB10, sqrt(nB10));
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(nC11_n, sqrt(nC11_n)), NT_n(nC12_n, sqrt(nC12_n)), NR_n(nC13_n, sqrt(nC13_n)), ND_n(nB10_n, sqrt(nB10_n));

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(0.968, 0.);

