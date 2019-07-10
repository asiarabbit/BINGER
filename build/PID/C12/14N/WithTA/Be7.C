	// nuclide specific information //
	// experiment information
	const char *reaction = "230 MeV/u 14N->12C => 7Be";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(0.9039, 0.0084), pT(pL), pR(pL), pD(0.9039, 0.0084);
	// contamination rate into the target nuclide zone
	const ExpData eL(0.01632, 0.00024), eR(0.01110, 0.00020);
	ExpData eD(0.02800, 0.00032); eD *= 1.; // partically aligned vertically
	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(0.7285, 0.0089), e_SVT(0.965, 0.014);

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(0, sqrt(0)), NT(45, sqrt(45)), NR(0, sqrt(0)), ND(0, sqrt(0)), N0(84901, 0);
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(0, sqrt(0)), NT_n(1, sqrt(1)), NR_n(0, sqrt(0)), ND_n(0, sqrt(0)), N0_n(34381, 0);

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(0.87076, 0.08);
	const ExpData e_e = e_dT*e_gT*e_SVT; // total efficiency

