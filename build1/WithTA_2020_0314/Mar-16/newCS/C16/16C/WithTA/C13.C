	// nuclide specific information //
	// experiment information
	const char *reaction = "240 MeV/u 16C->12C => 13C";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(0.95117, 0.00336), pT(pL), pR(pL), pD(0.956011, 0.004842);
	// contamination rate into the target nuclide zone
	const ExpData eL(0., 0.), eR(0.0168924, 0.000323544);
	ExpData eD(0.00086, 0.00011); eD *= 0.7; // partically aligned vertically
	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(0.908396, 0.00459323);
	const ExpData sigma0(908.9607, 23.1691); // interaction c.s. in mb
	const ExpData e_SVT(Exp(sigma0*(-1e-27)*t5mmC)); // sigma0: interaction cross section

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(nC12, sqrt(nC12)), NT(nC13, sqrt(nC13)), NR(nC14, sqrt(nC14)), ND(nB11, sqrt(nB11));
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(nC12_n, sqrt(nC12_n)), NT_n(nC13_n, sqrt(nC13_n)), NR_n(nC14_n, sqrt(nC14_n)), ND_n(nB11_n, sqrt(nB11_n));

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(0.50, 0.05);

