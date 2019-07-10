	// nuclide specific information //
	// experiment information
	const char *reaction = "230 MeV/u 13B->12C => 12Be";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(0.9039, 0.0084), pT(pL), pR(pL), pD(0.9039, 0.0084);
	// contamination rate into the target nuclide zone
	const ExpData eL(0.01632, 0.00024), eR(0.01110, 0.00020);
	ExpData eD(0.02800, 0.00032); eD *= 1.; // partically aligned vertically
	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(0.7285, 0.0089), e_SVT(0.965, 0.014);

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(nBe11, sqrt(nBe11)), NT(nBe12, sqrt(nBe12)), NR(nBe13, sqrt(nBe13)), ND(nLi9, sqrt(nLi9));
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(nBe11_n, sqrt(nBe11_n)), NT_n(nBe12_n, sqrt(nBe12_n)), NR_n(nBe13_n, sqrt(nBe13_n)), ND_n(nLi9_n, sqrt(nLi9_n));

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(1., 0.);

