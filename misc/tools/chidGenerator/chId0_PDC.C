// chId0_DCTa.C -- to be included by build....C. User-defined file to automatically generate chid config-files.

tChId_PDC chIdU = { // Upstream of the target
	{
	{8065}, // PDC-L1X (MWDC1): 16 ch
	{8081}, // PDC-L1Y (MWDC1): 16 ch
	},

	{
	{8097}, // PDC-L2X (MWDC2): 16 ch
	{8113}, // PDC-L2Y (MWDC2): 16 ch
	}
};

tChId_PDC chIdD = { // downstream of the target
	{
	{8297}, // PDC-R2X (MWDC3): 16 ch
	{8313}, // PDC-R2Y (MWDC3): 16 ch
	},

	{
	{8265}, // PDC-R1X (MWDC4): 16 ch
	{8281}, // PDC-R1Y (MWDC4): 16 ch
	}
};

