	// nuclide specific information //
	// experiment information
	const char *reaction = "240 MeV/u 15C->12C => 13B";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(0.9565, 0.0104), pT(pL), pR(pL), pD(0.953, 0.013);
	// contamination rate into the target nuclide zone
	const ExpData eL(0., 0.), eR(0.00121, 0.0010);
	ExpData eD(0.00331, 0.00037); eD *= 0.3; // partically aligned vertically
	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(0.825, 0.012), e_SVT(0.976, 0.010);

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(nB12, sqrt(nB12)), NT(nB13, sqrt(nB13)), NR(nB14, sqrt(nB14)), ND(nBe10, sqrt(nBe10));
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(nB12_n, sqrt(nB12_n)), NT_n(nB13_n, sqrt(nB13_n)), NR_n(nB14_n, sqrt(nB14_n)), ND_n(nBe10_n, sqrt(nBe10_n));

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(1., 0.);

