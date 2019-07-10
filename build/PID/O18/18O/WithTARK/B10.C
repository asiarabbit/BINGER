	// nuclide specific information //
	// experiment information
	const char *reaction = "260 MeV/u 18O->12C => 10B";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(pO18), pT(pO18), pR(pO18), pD(pO18);
	// contamination rate into the target nuclide zone
	const ExpData eL(eL_B), eR(eR_B);
	ExpData eD(eD_B); eD *= 1.; // partically aligned vertically

	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(ed_T_B), e_SVT(0.9602, 0.0201);

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(nB9, sqrt(nB9)), NT(nB10, sqrt(nB10)), NR(nB11, sqrt(nB11)), ND(nBe8, sqrt(nBe8));
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(nB9_n, sqrt(nB9_n)), NT_n(nB10_n, sqrt(nB10_n)), NR_n(nB11_n, sqrt(nB11_n)), ND_n(nBe8_n, sqrt(nBe8_n));

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(0.90, 0.1);

