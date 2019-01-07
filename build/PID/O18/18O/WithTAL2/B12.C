	// nuclide specific information //
	// experiment information
	const char *reaction = "260 MeV/u 18O->12C => 12B";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(pO18), pT(pO18), pR(pO18), pD(pO18);
	// contamination rate into the target nuclide zone
	const ExpData eL(eL_B), eR(eR_B);
	ExpData eD(eD_B); eD *= 0.2; // partically aligned vertically

	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(ed_T_B), e_SVT(0.964, 0.017);

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(nB11, sqrt(nB11)), NT(nB12, sqrt(nB12)), NR(nB13, sqrt(nB13)), ND(nBe10, sqrt(nBe10));
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(nB11_n, sqrt(nB11_n)), NT_n(nB12_n, sqrt(nB12_n)), NR_n(nB13_n, sqrt(nB13_n)), ND_n(nBe10_n, sqrt(nBe10_n));

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(1., 0.);

