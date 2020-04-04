	// nuclide specific information //
	// experiment information
	const char *reaction = "230 MeV/u 17N->12C => 9Be";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(0.9039, 0.0084), pT(pL), pR(pL), pD(pL);
	// contamination rate into the target nuclide zone
	const ExpData eL(0.01632, 0.00024), eR(0.01110, 0.00020);
	ExpData eD(0.02800, 0.00032); eD *= 0.; // partically aligned vertically
	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(0.7285, 0.0089), e_SVT(0.965, 0.014);

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(nBe8, sqrt(nBe8)), NT(nBe9, sqrt(nBe9)), NR(nBe10, sqrt(nBe10)), ND(nLi7, sqrt(nLi7));
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(nBe8_n, sqrt(nBe8_n)), NT_n(nBe9_n, sqrt(nBe9_n)), NR_n(nBe10_n, sqrt(nBe10_n)), ND_n(nLi7_n, sqrt(nLi7_n));

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(0.98, 0.);

