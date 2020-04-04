	// nuclide specific information //
	// experiment information
	const char *reaction = "260 MeV/u 18O->12C => 15C";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(pO18), pT(pO18), pR(pO18), pD(pO18);
	// contamination rate into the target nuclide zone
	const ExpData eL(eL_C), eR(eR_C);
	ExpData eD(eD_C); eD *= 0.4; // partically aligned vertically

	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(ed_T_C), e_SVT(0.9619, 0.0064);

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(nC14, sqrt(nC14)), NT(nC15, sqrt(nC15)), NR(nC16, sqrt(nC16)), ND(nB13, sqrt(nB13));
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(nC14_n, sqrt(nC14_n)), NT_n(nC15_n, sqrt(nC15_n)), NR_n(nC16_n, sqrt(nC16_n)), ND_n(nB13_n, sqrt(nB13_n));

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(1., 0.);

