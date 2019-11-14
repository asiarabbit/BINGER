	// nuclide specific information //
	// experiment information
	const char *reaction = "230 MeV/u 16O->12C => 13C";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(0.8869, 0.0039), pT(0.8732, 0.0096), pR(0.8859, 0.0027), pD(0.8771, 0.0113);
	// contamination rate into the target nuclide zone
	const ExpData eL(0.02647, 0.00031), eR(0.02451, 0.00030);
	ExpData eD(0.02041, 0.00027); eD *= 0.6; // partically aligned vertically
	// for target nuclide: detecting efficiency, geometrical efficiency and survival rate penetrating the target
	const ExpData e_dT(0.9290, 0.0137), e_gT(0.98214, 0.00383), e_SVT(0.956, 0.017);
	const ExpData e_e = e_dT*e_gT*e_SVT;

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(841, sqrt(841)), NT(493, sqrt(493)), NR(119, sqrt(119)), ND(373, sqrt(373)), N0(319248, 0);
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(48, sqrt(48)), NT_n(31, sqrt(31)), NR_n(3, sqrt(3)), ND_n(17, sqrt(17)), N0_n(130508, 0);

