	// nuclide specific information //
	// experiment information
	const char *reaction = "230 MeV/u 14N->12C => 10B";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(0.8771, 0.0113), pT(pL), pR(0.903, 0.013), pD(0.9039, 0.0084);
	// contamination rate into the target nuclide zone
	const ExpData eL(0.01632, 0.00024), eR(0.01110, 0.00020);
	ExpData eD(0.02800, 0.00032); eD *= 1.; // partically aligned vertically
	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(0.891, 0.015), e_SVT(0.9602, 0.0201);

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(0, sqrt(0)), NT(85, sqrt(85)), NR(123, sqrt(123)), ND(0, sqrt(0)), N0(84901, 0);
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(0, sqrt(0)), NT_n(8, sqrt(8)), NR_n(4, sqrt(4)), ND_n(0, sqrt(0)), N0_n(34381, 0);

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(0.99286, 0.0039);
	const ExpData e_e = e_dT*e_gT*e_SVT; // total efficiency

