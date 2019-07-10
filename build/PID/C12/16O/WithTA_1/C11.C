	// nuclide specific information //
	// experiment information
	const char *reaction = "230 MeV/u 16O->12C => 11C";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(0.8869, 0.0039), pT(pL), pR(0.8732, 0.0096), pD(0.8771, 0.0113);
	// contamination rate into the target nuclide zone
	const ExpData eL(0.02647, 0.00031), eR(0.02451, 0.00030);
	ExpData eD(0.02041, 0.00027); eD *= 0.6; // partically aligned vertically
	// for target nuclide: detecting efficiency, geometrical efficiency and survival rate penetrating the target
	const ExpData e_dT(0.9211, 0.0054), e_gT(0.9671, 0.0098), e_SVT(0.9624, 0.0069);
	const ExpData e_e = e_dT*e_gT*e_SVT;

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(28, sqrt(28)), NT(267, sqrt(267)), NR(841, sqrt(841)), ND(0, sqrt(0)), N0(319248, 0);
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(0, sqrt(0)), NT_n(25, sqrt(25)), NR_n(48, sqrt(48)), ND_n(0, sqrt(0)), N0_n(130508, 0);

