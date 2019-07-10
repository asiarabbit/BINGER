	// nuclide specific information //
	// experiment information
	const char *reaction = "230 MeV/u 16O->12C => 14N";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(0.8717, 0.0046), pT(pL), pR(pL), pD(0.8869, 0.0039);
	// contamination rate into the target nuclide zone
	const ExpData eL(0.04017, 0.00038), eR(0.03043, 0.00033), eD(0.01410, 0.00022);
	// for target nuclide: detecting efficiency, geometrical efficiency and survival rate penetrating the target
	const ExpData e_dT(0.9345, 0.0066), e_gT(0.99983, 0.0001), e_SVT(0.9521, 0.0082);
	const ExpData e_e = e_dT*e_gT*e_SVT;
	

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(151, sqrt(151)), NT(806., sqrt(806)), NR(962, sqrt(962)), ND(841, sqrt(841)), N0(319248, 0);
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(10, sqrt(10)), NT_n(85, sqrt(85)), NR_n(74, sqrt(74)), ND_n(48, sqrt(48)), N0_n(130508, 0.);

