	// nuclide specific information //
	// experiment information
	const char *reaction = "240 MeV/u 13C->12C => 11B";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(0.944, 0.012), pT(pL), pR(pL), pD(0.944, 0.012);
	// contamination rate into the target nuclide zone
	const ExpData eL(0., 0.), eR(0.01050, 0.00095);
	ExpData eD(0.00482, 0.00064); eD *= 0.5; // partically aligned vertically
	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(0.856, 0.015), e_SVT(0.9777, 0.0083);

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(nB10, sqrt(nB10)), NT(nB11, sqrt(nB11)), NR(nB12, sqrt(nB12)), ND(nBe9, sqrt(nBe9));
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(nB10_n, sqrt(nB10_n)), NT_n(nB11_n, sqrt(nB11_n)), NR_n(nB12_n, sqrt(nB12_n)), ND_n(nBe9_n, sqrt(nBe9_n));

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(1., 0.);

