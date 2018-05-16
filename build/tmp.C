struct tEntry{ // struct of reading one channel from binary files.
	char name[256]; // entry name, showing the identity of the entry.
	int index;
	int channelId;
	int nl; // count of leading edges in one data section.
	int nt; // count of trailing edges in one dat section.
	double leadingTime[10];
	double trailingTime[10];
	bool is_V;
	int bunchId; // trigger time in one time cycle

	tEntry();
	void initialize();	
};

void tmp(){
	TFile *f = new TFile("./daq.052_0.root");

	// define the data tree
	int index;
	tEntry entry_temp;
	TTree *treeDataVME = (TTree*)f->Get("treeDataVME");
	treeDataVME->SetBranchAddress("index", &entry_temp.index); // run id
	treeDataVME->SetBranchAddress("channelId", &entry_temp.channelId);
	treeDataVME->SetBranchAddress("nl", &entry_temp.nl);
	treeDataVME->SetBranchAddress("nt", &entry_temp.nt);
	treeDataVME->SetBranchAddress("leadingTime", entry_temp.leadingTime);
	treeDataVME->SetBranchAddress("trailingTime", entry_temp.trailingTime);
	treeDataVME->SetBranchAddress("is_V", &entry_temp.is_V);
	treeDataVME->SetBranchAddress("bunchId", &entry_temp.bunchId);


	TH1F *h = new TH1F("h", "h", 500, -100., 100.);
	const int n = treeDataVME->GetEntries();
	cout << "n: " << n << endl;
	vector<tEntry *> ve;
	int i = 0;
	while(i < n){
		while(1){
			entry_temp.initialize();
			treeDataVME->GetEntry(i++);
			if(-2 != entry_temp.index){ // index == -2 marks end of one data section
				ve.push_back(new tEntry(entry_temp));
				index = entry_temp.index;
			}
			else break;
		} // entry assignment for the data section complete
		double trig1 = -9999., trig2 = -9999.;
		for(tEntry *e : ve) if(e){
			if(8010 == e->channelId) trig1 = e->leadingTime[0];
			if(8210 == e->channelId) trig2 = e->leadingTime[0];
		}
		if(-9999. != trig1 && -9999. != trig2){
//			cout << "trig1: " << trig1 << "\ttrig2: " << trig2 << endl; // DEBUG
//			cout << "trig1 - trig2: " << trig1 - trig2 << endl; getchar(); // DEBUG
			h->Fill(trig1 - trig2);
		}

		// clear for next event
		for(tEntry *&e : ve) if(e){
			delete e; e = nullptr;
		}
		ve.clear();
	} // end for over entries in treeDataVME
	
	TCanvas *c = new TCanvas("c", "c", 800, 600);
	h->Draw();	
} // end function tmp









tEntry::tEntry(){
	initialize();
} // end of the constructor

void tEntry::initialize(){
	index = -1; channelId = -1;
	nl = -1; nt = -1;
	for(int i = 0; i < 10; i++){
		leadingTime[i] = -9999.;
		trailingTime[i] = -9999.;
	} // end for over i
	is_V = false;
	strcpy(name, "");
} // end of function initialize









