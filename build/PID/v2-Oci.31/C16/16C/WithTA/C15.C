	// nuclide specific information //
	// experiment information
	const char *reaction = "240 MeV/u 16C->12C => 15C";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(0.9573, 0.0034), pT(pL), pR(pL), pD(0.9624, 0.0050);
	// contamination rate into the target nuclide zone
	const ExpData eL(0., 0.), eR(0.0180, 0.0003);
	ExpData eD(0.00086, 0.00011); eD *= 0.2; // partically aligned vertically
	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(0.8691, 0.0044), e_SVT(0.9724, 0.0031);

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(nC14, sqrt(nC14)), NT(nC15, sqrt(nC15)), NR(nC16, sqrt(nC16)), ND(nB13, sqrt(nB13));
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(nC14_n, sqrt(nC14_n)), NT_n(nC15_n, sqrt(nC15_n)), NR_n(nC16_n, sqrt(nC16_n)), ND_n(nB13_n, sqrt(nB13_n));

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(1., 0.);

