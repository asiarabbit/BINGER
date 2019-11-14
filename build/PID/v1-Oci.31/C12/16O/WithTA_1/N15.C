	// nuclide specific information //
	// experiment information
	const char *reaction = "260 MeV/u 18O->12C => 15N";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(0.8717, 0.0046), pT(pL), pR(pL), pD(0.8869, 0.0039);
	// contamination rate into the target nuclide zone
	const ExpData eL(0.04017, 0.00038), eR(0.03043, 0.00033);
	ExpData eD(0.01410, 0.00022); eD *= 0.6; // partically aligned vertically

	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(0.9345, 0.0066), e_SVT(0.9521, 0.0082);

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(nN14, sqrt(nN14)), NT(nN15, sqrt(nN15)), NR(nN16, sqrt(nN16)), ND(nC13, sqrt(nC13));
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(nN14_n, sqrt(nN14_n)), NT_n(nN15_n, sqrt(nN15_n)), NR_n(nN16_n, sqrt(nN16_n)), ND_n(nC13_n, sqrt(nC13_n));

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(1., 0.);

