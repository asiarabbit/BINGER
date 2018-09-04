# #########################################################
# Explanation file for config file system: beamTest_2016Nov
# Author: SUN Yazhou   Email: asia.rabbit@163.com
# Creation Date: March 26th, 2018.
# Last modified: March 26th, 2018, SUN Yazhou.
# #########################################################

This directory is for holding configuration files for the data analysis to run, which includes:

	1. Detector calibration parameters
	2. Detector layout parameters
	3. Detecting units and channel id map table
	4. Space - Time Relation (STR)
	5. program controlling parameters

The parameters would be classied in a hierarchical directory system, classified in the way the detector units are arranged physically. Config files are grouped by different experiments. A text file named "content" would be generated in the root directory of an experiment config file hierarchy to register the containing files of the directory. 2018/3/26.
