///////////////////////////////////////////////////////////////////////////////////////
// Data Analysis Code Project for the External Target Facility, HIRFL-CSR, @IMP      //
//																				     //
// etf/inc/read.C																	 //
//   read.C -- source file for method TABUAA::ReadOffline							 //
//   Introduction: This application is an implementation of VME raw datafile decoding//
//																				     //
// Author: SUN Yazhou, asia.rabbit@163.com.										     //
// Created: 2016/11/15, transported: 2018/1/9.									     //
// Last modified: 2018/5/23, SUN Yazhou.										     //
//																				     //
//																				     //
// Copyright (C) 2017-2018, SUN Yazhou.											     //
// All rights reserved.															     //
///////////////////////////////////////////////////////////////////////////////////////

void TABUAA::ReadOffline(){
	//////////////* open a binary data file *////////////////
//	cout << "TABUAA::ReadOffline() entered."; // DEBUG
//	cout << "The input datafile is " << fDatafile << endl; // DEBUG
//	cout << "Press ENTER to start interpreting the binary data file..."; // DEBUG
//	getchar(); // DEBUG
	
	FILE *fp; // file pointer
	// open the input binary file
	if ((fp = fopen(fDatafile.c_str(), "rb")) == NULL){
		cout << "File " << fDatafile << " cannot be opened." << endl;
		exit(EXIT_FAILURE);
	} // end if
	const int blkSize = 8192; // size of one block, in sizeof(int)
	int buffer[blkSize]; // cache block data
	int pos = 0; // for reading buffer
	int event_num = 0; // number of events processed
	int block_num = 0; // number of blocks processed.
	tVME_block block; // temporarily stores block information.
	tVME_event event; // temporarily stores event information.

	string rootfile;
	if(strcmp(fROOTFileName.c_str(), "")) rootfile = fROOTFileName;
	else rootfile = fDatafile+"_vme.root";
	if(access(rootfile.c_str(), 0) == 0) return; // file already exists.
	
	TFile* f = new TFile(rootfile.c_str(),"RECREATE");

	TTree *vme = new TTree("vme", "VME Data for External Target Facility");
	vme->Branch("tdc0", event.tdc[0], "tdc0[32]/I");
	vme->Branch("tdc1", event.tdc[1], "tdc1[32]/I");  //hw
	vme->Branch("adc", event.adc, "adc[32]/I");
	vme->Branch("qdc0", event.qdc[0], "qdc0[32]/I");
	vme->Branch("qdc1", event.qdc[1], "qdc1[32]/I");  //hw
	vme->Branch("qdc2", event.qdc[2], "qdc2[32]/I");
	vme->Branch("hit", event.hit, "hit[32]/I");
	vme->Branch("mtdc", event.mtdc0, "mtdc[32][10]/I");
	vme->Branch("sca", event.sca, "sca[16]/I");
	vme->Branch("dsca", event.dsca, "dsca[16]/I");


	///////////////* read the valid data *///////////////////
	if(fread(buffer, sizeof(int), blkSize, fp) <= 0){ // reading failure
		cout << strerror(errno) << endl;
		exit(EXIT_FAILURE);
	} // end if
	else memset(buffer, 0, sizeof(buffer)); // abandon the first block
	block_num++;

	while(fread(buffer, sizeof(int), blkSize, fp) > 0){
		block.index = buffer[0];
		block.event_num = buffer[1];
		block.lastPos = buffer[2];
//		cout << "buffer[0]: " << buffer[0] << endl;
//		cout << "block index: " << block.index << endl; // DEBUG
//		cout << "block event number: " << block.event_num << endl; // DEBUG
//		cout << "block last Postion: " << block.lastPos << endl; // DEBUG
//		getchar(); // DEBUG
		pos = 3;
		// read all the events one by one in the current buffer.
		for(int i = 0; i < block.event_num; i++){
			event.initialize(); // initialize event.
			event.length = buffer[pos];
			event.index = buffer[pos + 1];
			event.VMEReadCount = buffer[pos + 2];
			analyze(&buffer[pos + 3], event); // read one event, and assign event.

//			cout << "event index: " << event.index << endl; // DEBUG
//			cout << "event length: " << event.length << endl; // DEBUG
//			cout << "event VMEReadCount: " << event.VMEReadCount << endl; // DEBUG
//			getchar(); // DEBUG

			vme->Fill();
//			cout << "Mark 1, block.event.size(): " << block.event.size() << endl; // DEBUG
//			cout << "pos: " << pos << endl; // DEBUG
			block.event.push_back(event); // store one event in block
			pos += event.length;
//			cout << "Mark 2, block.event.size(): " << block.event.size() << endl; // DEBUG
//			cout << "pos: " << pos << endl; // DEBUG
//			cout << "event_num: " << event_num << endl; // DEBUG
//			getchar(); // DEBUG
			event_num++; // event number increments.
		} // end for over i
		block.event.clear();
		block_num++;
		cout << "Block " << block_num << " processed.\r" << flush;
		memset(buffer, 0, sizeof(buffer)); // initialize buffer.
	} // end while
	
	cout << block_num << " blocks and " << event_num << " events has been processed." << endl;
	// close the binary file
	fclose(fp);

	vme->Write("", TObject::kOverwrite);
	f->Close();
} // end of function read.


