	// nuclide specific information //
	// experiment information
	const char *reaction = "230 MeV/u 16O->12C => 12C";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(0.8869, 0.0039), pT(pL), pR(0.8732, 0.0096), pD(0.8771, 0.0113);
	// contamination rate into the target nuclide zone
	const ExpData eL(0.02647, 0.00031), eR(0.02451, 0.00030), eD(0.02041, 0.00027);
	// for target nuclide: detecting efficiency, geometrical efficiency and survival rate penetrating the target
	const ExpData e_dT(0.9211, 0.0054), e_gT(0.99547, 0.00117), e_SVT(0.9624, 0.0069);
	const ExpData e_e = e_dT*e_gT*e_SVT;

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(267, sqrt(267)), NT(841, sqrt(841)), NR(493, sqrt(493)), ND(231, sqrt(231)), N0(319248, 0);
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(25, sqrt(25)), NT_n(48, sqrt(48)), NR_n(31, sqrt(31)), ND_n(10, sqrt(10)), N0_n(130508, 0);

