	// nuclide specific information //
	// experiment information
	const char *reaction = "260 MeV/u 18O->12C => 15N";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(pO18), pT(pO18), pR(pO18), pD(pO18);
	// contamination rate into the target nuclide zone
	const ExpData eL(eL_N), eR(eR_N);
	ExpData eD(eD_N); eD *= 0.8; // partically aligned vertically

	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(ed_T_N), e_SVT(0.9521, 0.0082);

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(nN14, sqrt(nN14)), NT(nN15, sqrt(nN15)), NR(nN16, sqrt(nN16)), ND(nC13, sqrt(nC13));
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(nN14_n, sqrt(nN14_n)), NT_n(nN15_n, sqrt(nN15_n)), NR_n(nN16_n, sqrt(nN16_n)), ND_n(nC13_n, sqrt(nC13_n));
	
	const ExpData e_gT(0.985, 0.);

