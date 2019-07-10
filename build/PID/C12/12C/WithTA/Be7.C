	// nuclide specific information //
	// experiment information
	const char *reaction = "230 MeV/u 12C->12C => 7Be";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(0.9039, 0.0084), pT(pL), pR(pL), pD(0.9039, 0.0084);
	// contamination rate into the target nuclide zone
	const ExpData eL(0.01632, 0.00024), eR(0.01110, 0.00020);
	ExpData eD(0.02800, 0.00032); eD *= 1.; // partically aligned vertically
	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(0.7285, 0.0089), e_SVT(0.965, 0.014);

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(nBe6, sqrt(nBe6)), NT(nBe7, sqrt(nBe7)), NR(nBe8, sqrt(nBe8)), ND(nLi5, sqrt(nLi5));
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(nBe6_n, sqrt(nBe6_n)), NT_n(nBe7_n, sqrt(nBe7_n)), NR_n(nBe8_n, sqrt(nBe8_n)), ND_n(nLi5_n, sqrt(nLi5_n));

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(0.87076, 0.08);

