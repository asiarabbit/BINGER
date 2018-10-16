	// nuclide specific information //
	// experiment information
	const char *reaction = "230 MeV/u 14N->12C => 12C";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(0.8869, 0.0039), pT(pL), pR(0.8732, 0.0096), pD(0.8771, 0.0113);
	// contamination rate into the target nuclide zone
	const ExpData eL(0.02647, 0.00031), eR(0.02451, 0.00030);
	ExpData eD(0.02041, 0.00027); eD *= 1.; // partically aligned vertically
	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(0.9211, 0.0054), e_SVT(0.9624, 0.0069);

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(98, sqrt(98)), NT(577, sqrt(577)), NR(99, sqrt(99)), ND(85, sqrt(85)), N0(84901, 0);
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(15, sqrt(15)), NT_n(71, sqrt(71)), NR_n(8, sqrt(8)), ND_n(8, sqrt(8)), N0_n(34381, 0);

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(0.99547, 0.00117);
	const ExpData e_e = e_dT*e_gT*e_SVT; // total efficiency

