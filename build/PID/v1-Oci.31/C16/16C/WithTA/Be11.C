	// nuclide specific information //
	// experiment information
	const char *reaction = "240 MeV/u 16C->12C => 11Be";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(0.962, 0.012), pT(pL), pR(pL), pD(pL);
	// contamination rate into the target nuclide zone
	const ExpData eL(0., 0.), eR(0.0043, 0.0006);
	ExpData eD(0.00049, 0.00020); eD *= 0.3; // partically aligned vertically
	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(0.732, 0.013), e_SVT(0.978, 0.010);

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(nBe10, sqrt(nBe10)), NT(nBe11, sqrt(nBe11)), NR(nBe12, sqrt(nBe12)), ND(nLi8, sqrt(nLi8));
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(nBe10_n, sqrt(nBe10_n)), NT_n(nBe11_n, sqrt(nBe11_n)), NR_n(nBe12_n, sqrt(nBe12_n)), ND_n(nLi8_n, sqrt(nLi8_n));

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(0.98, 0.);

