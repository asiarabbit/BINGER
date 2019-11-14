	// nuclide specific information //
	// experiment information
	const char *reaction = "240 MeV/u 16C->12C => 14C";
	// dispersion rate XXX: L: left; T: target nuclide; R: right; D: down (under)
	const ExpData pL(0.95117, 0.00336), pT(pL), pR(pL), pD(0.956011, 0.004842);
	// contamination rate into the target nuclide zone
	const ExpData eL(0., 0.), eR(0.0197838, 0.000351204);
	ExpData eD(0.00086, 0.00011); eD *= 0.5; // partically aligned vertically
	// for target nuclide: detecting efficiency and survival rate penetrating the target
	const ExpData e_dT(0.908074, 0.00459552);
	const ExpData sigma0(912.6504, 23.8303); // interaction c.s. in mb
	const ExpData e_SVT(Exp(sigma0*(-1e-27)*t5mmC)); // sigma0: interaction cross section

	// USER INPUT - THE REAL AND SMEARED YIELD: WithTa //
	const ExpData NL(nC13, sqrt(nC13)), NT(nC14, sqrt(nC14)), NR(nC15, sqrt(nC15)), ND(nB12, sqrt(nB12));
	// USER INPUT - THE REAL AND SMEARED YIELD: NOTA //
	const ExpData NL_n(nC13_n, sqrt(nC13_n)), NT_n(nC14_n, sqrt(nC14_n)), NR_n(nC15_n, sqrt(nC15_n)), ND_n(nB12_n, sqrt(nB12_n));

	// the most tricky one: the geometrical efficiency //
	const ExpData e_gT(0.9578, 0.001);

