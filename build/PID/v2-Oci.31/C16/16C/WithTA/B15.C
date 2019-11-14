	// nuclide specific information //
	// experiment information
	const char *reaction = "240 MeV/u 16C->12C => 15B";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(0.9624, 0.0050), pT(pL), pR(pL), pD(0.962, 0.012);
	// contamination rate into the target nuclide zone
	const ExpData eL(0., 0.), eR(0.0116, 0.0004);
	ExpData eD(0.00049, 0.00020); eD *= 1.; // partically aligned vertically
	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(0.8482, 0.0059), e_SVT(0.9759, 0.0039);

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(nB14, sqrt(nB14)), NT(nB15, sqrt(nB15)), NR(nB16, sqrt(nB16)), ND(nBe12, sqrt(nBe12));
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(nB14_n, sqrt(nB14_n)), NT_n(nB15_n, sqrt(nB15_n)), NR_n(nB16_n, sqrt(nB16_n)), ND_n(nBe12_n, sqrt(nBe12_n));

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(0.99560, 0.0042);

