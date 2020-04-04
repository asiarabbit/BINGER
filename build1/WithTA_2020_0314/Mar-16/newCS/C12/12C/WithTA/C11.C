	// nuclide specific information //
	// experiment information
	const char *reaction = "240 MeV/u 12C->12C => C11";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(0.929867, 0.00395842), pT(pL), pR(pL), pD(pL);
	// contamination rate into the target nuclide zone
	const ExpData eL(0., 0.), eR(0.0160837, 0.000377751);
	ExpData eD(0.00482, 0.00064); eD *= 0.5; // partically aligned vertically
	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(0.923763, 0.00582709);
	const ExpData sigma0(762.1677, 18.1800); // interaction c.s. in mb
	const ExpData e_SVT(Exp(sigma0*(-1e-27)*t5mmC)); // sigma0: interaction cross section

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(nC10, sqrt(nC10)), NT(nC11, sqrt(nC11)), NR(nC12, sqrt(nC12)), ND(nB9, sqrt(nB9));
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(nC10_n, sqrt(nC10_n)), NT_n(nC11_n, sqrt(nC11_n)), NR_n(nC12_n, sqrt(nC12_n)), ND_n(nB9_n, sqrt(nB9_n));

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(1., 0.);

