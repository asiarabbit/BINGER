	// nuclide specific information //
	// experiment information
	const char *reaction = "230 MeV/u 17N->12C => 15C";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(0.8859, 0.0027), pT(0.8835, 0.0036), pR(0.8932, 0.0032), pD(0.9053, 0.0047);
	// contamination rate into the target nuclide zone
	const ExpData eL(0.02647, 0.00031), eR(0.02451, 0.00030);
	ExpData eD(0.02041, 0.00027); eD *= 0.1; // partically aligned vertically
	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(0.9031, 0.0049), e_SVT(0.9619, 0.0064);

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(nC14, sqrt(nC14)), NT(nC15, sqrt(nC15)), NR(nC16, sqrt(nC16)), ND(nB13, sqrt(nB13));
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(nC14_n, sqrt(nC14_n)), NT_n(nC15_n, sqrt(nC15_n)), NR_n(nC16_n, sqrt(nC16_n)), ND_n(nB13_n, sqrt(nB13_n));

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(1., 0.);

