	// nuclide specific information //
	// experiment information
	const char *reaction = "240 MeV/u 15C->12C => C15Ori";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(0.9512, 0.0038), pT(1., 0.), pR(pL), pD(0.9565, 0.0104);
	// contamination rate into the target nuclide zone
	const ExpData eL(0., 0.), eR(0.01675, 0.00037);
	ExpData eD(0.00335, 0.00044); eD *= 0.5; // partically aligned vertically
	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(0.8704, 0.0047), e_SVT(0.9724, 0.0031);

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(nC14, sqrt(nC14)), NT(nC15Ori, sqrt(nC15Ori)), NR(nC16, sqrt(nC16)), ND(nB13, sqrt(nB13));
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(nC14_n, sqrt(nC14_n)), NT_n(nC15Ori_n, sqrt(nC15Ori_n)), NR_n(nC16_n, sqrt(nC16_n)), ND_n(nB13_n, sqrt(nB13_n));

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(1., 0.);

