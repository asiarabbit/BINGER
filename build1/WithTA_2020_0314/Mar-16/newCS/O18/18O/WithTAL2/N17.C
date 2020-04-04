	// nuclide specific information //
	// experiment information
	const char *reaction = "280 MeV/u 18O->12C => 17N";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(pO18), pT(pO18), pR(pO18), pD(pO18);
	// contamination rate into the target nuclide zone
	const ExpData eL(eL_N), eR(eR_N);
	ExpData eD(eD_N); eD *= 0.6; // partically aligned vertically

	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(ed_T_N), e_SVT(0.9572, 0.0068);

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(nN16, sqrt(nN16)), NT(nN17, sqrt(nN17)), NR(nN18, sqrt(nN18)), ND(nC15, sqrt(nC15));
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(nN16_n, sqrt(nN16_n)), NT_n(nN17_n, sqrt(nN17_n)), NR_n(nN18_n, sqrt(nN18_n)), ND_n(nC15_n, sqrt(nC15_n));

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(1., 0.);

