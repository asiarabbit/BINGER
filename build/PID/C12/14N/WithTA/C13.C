	// nuclide specific information //
	// experiment information
	const char *reaction = "230 MeV/u 14N->12C => 13C";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(0.8869, 0.0039), pT(0.8732, 0.0096), pR(pT), pD(0.903, 0.013);
	// contamination rate into the target nuclide zone
	const ExpData eL(0.02647, 0.00031), eR(0.02451, 0.00030);
	ExpData eD(0.02041, 0.00027); eD *= 0.6; // partically aligned vertically
	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(0.9290, 0.0137), e_SVT(0.956, 0.017);

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(577, sqrt(577)), NT(99, sqrt(99)), NR(12, sqrt(12)), ND(123, sqrt(123)), N0(84901, 0);
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(71, sqrt(71)), NT_n(8, sqrt(8)), NR_n(4, sqrt(4)), ND_n(8, sqrt(8)), N0_n(34381, 0);

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(0.98745, 0.0067);
	const ExpData e_e = e_dT*e_gT*e_SVT; // total efficiency

