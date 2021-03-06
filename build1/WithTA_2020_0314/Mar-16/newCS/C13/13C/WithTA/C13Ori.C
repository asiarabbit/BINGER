	// nuclide specific information //
	// experiment information
	const char *reaction = "240 MeV/u 13C->12C => C13Ori";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(0.9455, 0.0061), pT(pL), pR(pL), pD(0.944, 0.012);
	// contamination rate into the target nuclide zone
	const ExpData eL(0., 0.), eR(0.01563, 0.00024);
	ExpData eD(0.00482, 0.00064); eD *= 0.8; // partically aligned vertically
	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(0.8923, 0.0070), e_SVT(0.9755, 0.0049);

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(nC12, sqrt(nC12)), NT(nC13Ori, sqrt(nC13Ori)), NR(nC14, sqrt(nC14)), ND(nB11, sqrt(nB11));
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(nC12_n, sqrt(nC12_n)), NT_n(nC13Ori_n, sqrt(nC13Ori_n)), NR_n(nC14_n, sqrt(nC14_n)), ND_n(nB11_n, sqrt(nB11_n));

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(1., 0.);

