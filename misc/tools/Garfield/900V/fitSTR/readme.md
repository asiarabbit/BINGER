#readme.md --the instruction for extracting STR from the garfield++ simulation data
To extract STRs under certain MWDC's HV value, follow the steps listed below:

	1. mkdir pdf and run root -l driftTime.C to extract drfit time - impact parameter scatter plot;
	2. run root -l fitSTR.C to fit STR from the 2-d scatter plot obtained in step 1. Note that the fitSTR.C should be run 6 times, each of which accounts for an angle interval of five degrees between the particle track and the drift cell. So STRid would be incremented for each time, from 9 to 14;
	3. mkdir STR and run STRSave.C, then a rootfile aaa_(HV).root would be created. The created rootfile can be put to direct use for data analysis in ETF project.
