	// nuclide specific information //
	// experiment information
	const char *reaction = "240 MeV/u 15C->12C => 10Be";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(0.9608, 0.0088), pT(pL), pR(pL), pD(pL);
	// contamination rate into the target nuclide zone
	const ExpData eL(0., 0.), eR(0.00417, 0.00041);
	ExpData eD(0.00331, 0.00037); eD *= 0.; // partically aligned vertically
	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(0.7084, 0.0086), e_SVT(0.9799, 0.0069);

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(nBe9, sqrt(nBe9)), NT(nBe10, sqrt(nBe10)), NR(nBe11, sqrt(nBe11)), ND(nLi7, sqrt(nLi7));
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(nBe9_n, sqrt(nBe9_n)), NT_n(nBe10_n, sqrt(nBe10_n)), NR_n(nBe11_n, sqrt(nBe11_n)), ND_n(nLi7_n, sqrt(nLi7_n));

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(1., 0.);

