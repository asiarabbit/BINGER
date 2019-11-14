	// nuclide specific information //
	// experiment information
	const char *reaction = "240 MeV/u 12C->12C => C10";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(0.926775, 0.00395508), pT(pL), pR(pL), pD(0.944, 0.012);
	// contamination rate into the target nuclide zone
	const ExpData eL(0., 0.), eR(0.0186326, 0.000407753);
	ExpData eD(0.00482, 0.00064); eD *= 0.4; // partically aligned vertically
	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(0.923611, 0.00583145);
	const ExpData sigma0(770.8620, 17.7015); // interaction c.s. in mb
	const ExpData e_SVT(Exp(sigma0*(-1e-27)*t5mmC)); // sigma0: interaction cross section

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(nC9, sqrt(nC9)), NT(nC10, sqrt(nC10)), NR(nC11, sqrt(nC11)), ND(nBe6, sqrt(nBe6));
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(nC9_n, sqrt(nC9_n)), NT_n(nC10_n, sqrt(nC10_n)), NR_n(nC11_n, sqrt(nC11_n)), ND_n(nBe6_n, sqrt(nBe6_n));

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(0.89, 0.05);

