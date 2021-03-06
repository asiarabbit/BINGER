	// nuclide specific information //
	// experiment information
	const char *reaction = "230 MeV/u 14N->12C => 11B";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(0.8771, 0.0113), pT(0.903, 0.013), pR(0.896, 0.013), pD(0.9039, 0.0084);
	// contamination rate into the target nuclide zone
	const ExpData eL(0.01632, 0.00024), eR(0.01110, 0.00020);
	ExpData eD(0.02800, 0.00032); eD *= 0.5; // partically aligned vertically
	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(0.8834, 0.0165), e_SVT(0.965, 0.022);

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(85, sqrt(85)), NT(123, sqrt(123)), NR(12, sqrt(12)), ND(23, sqrt(23)), N0(84901, 0);
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(8, sqrt(8)), NT_n(4, sqrt(4)), NR_n(2, sqrt(2)), ND_n(2, sqrt(2)), N0_n(34381, 0);

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(0.97, 0.);
	const ExpData e_e = e_dT*e_gT*e_SVT; // total efficiency

