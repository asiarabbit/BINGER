	// nuclide specific information //
	// experiment information
	const char *reaction = "260 MeV/u 18O->12C => 11B";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(pO18), pT(pO18), pR(pO18), pD(pO18);
	// contamination rate into the target nuclide zone
	const ExpData eL(eL_B), eR(eR_B);
	ExpData eD(eD_B); eD *= 0.5; // partically aligned vertically

	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(ed_T_B), e_SVT(0.965, 0.022);

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(nB10, sqrt(nB10)), NT(nB11, sqrt(nB11)), NR(nB12, sqrt(nB12)), ND(nBe9, sqrt(nBe9));
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(nB10_n, sqrt(nB10_n)), NT_n(nB11_n, sqrt(nB11_n)), NR_n(nB12_n, sqrt(nB12_n)), ND_n(nBe9_n, sqrt(nBe9_n));

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(0.99, 0.);

