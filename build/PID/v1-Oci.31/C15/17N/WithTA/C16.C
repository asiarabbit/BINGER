	// nuclide specific information //
	// experiment information
	const char *reaction = "230 MeV/u 17N->12C => 16C";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(0.8835, 0.0036), pT(0.8932, 0.0032), pR(pT), pD(0.9053, 0.0047);
	// contamination rate into the target nuclide zone
	const ExpData eL(0.02647, 0.00031), eR(0.02451, 0.00030);
	ExpData eD(0.02041, 0.00027); eD *= 0.; // partically aligned vertically
	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(0.9058, 0.0044), e_SVT(0.9625, 0.0057);

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(nC15, sqrt(nC15)), NT(nC16, sqrt(nC16)), NR(nC17, sqrt(nC17)), ND(nB14, sqrt(nB14));
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(nC15_n, sqrt(nC15_n)), NT_n(nC16_n, sqrt(nC16_n)), NR_n(nC17_n, sqrt(nC17_n)), ND_n(nB14_n, sqrt(nB14_n));

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(1., 0.);

