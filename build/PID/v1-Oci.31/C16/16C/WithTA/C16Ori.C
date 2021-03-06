	// nuclide specific information //
	// experiment information
	const char *reaction = "240 MeV/u 16C->12C => C16Ori";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(0.9573, 0.0034), pT(1., 0.), pR(pL), pD(0.9624, 0.0050);
	// contamination rate into the target nuclide zone
	const ExpData eL(0., 0.), eR(0.0180, 0.0003);
	ExpData eD(0.00086, 0.00011); eD *= 0.6; // partically aligned vertically
	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(0.8729, 0.0042), e_SVT(0.9727, 0.0027);

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(nC15, sqrt(nC15)), NT(nC16Ori, sqrt(nC16Ori)), NR(nC17, sqrt(nC17)), ND(nB13, sqrt(nB13));
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(nC15_n, sqrt(nC15_n)), NT_n(nC16Ori_n, sqrt(nC16Ori_n)), NR_n(nC17_n, sqrt(nC17_n)), ND_n(nB13_n, sqrt(nB13_n));

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(1., 0.);

