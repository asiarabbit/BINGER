# #########################################################
# Explanation file for config directory
# Author: SUN Yazhou   Email: asia.rabbit@163.com
# Creation Date: September 23rd, 2017.
# Last modified: October 24th, 2017, SUN Yazhou.
# #########################################################

**beamTest_2016Nov:**
> Beam Test using 311-AMeV 40Ar, mainly for testing MWDC performance in particle tracking.
MWDC anode HV: 1000V -> 900V, TOFWall PMT HV: -1500V
MWDCArrayL not commissioned.

pion_2016Nov
Pion ratio experiment using 311-AMeV 40Ar. SiPM Plastic strip array is used to track post-target particles before the dipole magnet. MWDC anode HV: 1500V, TOFWall PMT HV: -1500V. Both MWDC arrays were commissioned in the experiment.

pion_2017Oct
Pion ratio experiment to be conducted.... A SiPM plastic strip barrel is used to detect impact parameters by its hit multiplicity. Tracking detectors were deployed more compactly to detect more pions before they decay and die out.


FOR USERS TO SET THE PROGRAM CONFIGURATION PARAMETERS <INCLUDING POSITION PARAMETERS AND DELAYS>
1. Most of the program control parameters (tracking precision parameters, all kinds of cuts, calibration file path) are found in the source file of class TACtrlPara;
2. Some of the delay parameters (mostly for MWDCs and TOFWall) are stored in the source and header of class TADeployPara;
3. Individually defined ones would show themselves by turning on TAPopMsg::ConfigInfo(...) method.
4. The delay of DC anodes are set in the TAAnode constructor. To alter the value, one can modify the set values in TADeployPara:GetMWDCDelay() and TACtrlPara::T_wireMean();

* For any confusions, please don't hesitate to query the author SUN Yazhou via asia.rabbit@163.com.


