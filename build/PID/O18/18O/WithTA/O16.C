	// nuclide specific information //
	// experiment information
	const char *reaction = "260 MeV/u 18O->12C => 16O";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(pO18), pT(pO18), pR(pO18), pD(pO18);
	// contamination rate into the target nuclide zone
	const ExpData eL(eL_O), eR(eR_O);
	ExpData eD(eD_O); eD *= 1.; // partically aligned vertically

	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(ed_T_O), e_SVT(0.9552, 0.0042);

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(nO15, sqrt(nO15)), NT(nO16, sqrt(nO16)), NR(nO17, sqrt(nO17)), ND(nN14, sqrt(nN14));
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(nO15_n, sqrt(nO15_n)), NT_n(nO16_n, sqrt(nO16_n)), NR_n(nO17_n, sqrt(nO17_n)), ND_n(nN14_n, sqrt(nN14_n));

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(0.98, 0.);

