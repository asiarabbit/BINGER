	// nuclide specific information //
	// experiment information
	const char *reaction = "260 MeV/u 18O->12C => 14N";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(pO18), pT(pO18), pR(pO18), pD(pO18);
	// contamination rate into the target nuclide zone
	const ExpData eL(eL_N), eR(eR_N);
	ExpData eD(eD_N); eD *= 1.; // partically aligned vertically

	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(0.9345, 0.0066), e_SVT(0.9521, 0.0082);

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(nN13, sqrt(nN13)), NT(nN14, sqrt(nN14)), NR(nN15, sqrt(nN15)), ND(nC12, sqrt(nC12));
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(nN13_n, sqrt(nN13_n)), NT_n(nN14_n, sqrt(nN14_n)), NR_n(nN15_n, sqrt(nN15_n)), ND_n(nC12_n, sqrt(nC12_n));

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(0.977, 0.);

