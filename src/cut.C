/**
	\file assess.C
	\example assess.C
	\brief to select cut for gui
	\author SUN Yazhou, asia.rabbit@163.com.
	\date Created: 2018/6/29 Last revised: 2018/6/30, SUN Yazhou.
	\copyright 2017-2018, SUN Yazhou.
*/

// cut.C -- to select cut for gui
// Created: Jun. 29th, 2018; Last Modified: Jun. 30th, 2018, Author: SUN Yazhou

#include <iostream>
#include <cstdlib>
#include <string>

#include "TFile.h"
#include "TTree.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TCutG.h"
#include "TCut.h"
#include "TApplication.h"

using std::cout;
using std::endl;
using std::string;

TFile *cut(const char *file){
	if(0 != access(file, F_OK)){
		cout << file << " does not exist. Exiting...\n";
		exit(1);
	}
	TFile *f = new TFile(file, "UPDATE");
	TTree *treeTrack = (TTree*)f->Get("treeTrack");
	TTree *vme = (TTree*)f->Get("vme");
	TTree *treeshoot = (TTree*)f->Get("treeshoot");
	TTree *treeSi = (TTree*)f->Get("treeSi");
	treeTrack->AddFriend(vme);
	treeTrack->AddFriend(treeshoot);
	treeTrack->AddFriend(treeSi);

	TCanvas *c = new TCanvas("c", "CUTG Selector", 1600, 1400);
	c->Divide(2, 2);

	TCut cut0("pileUpSCA==1"); // &&t0_1_ok
	string title;
	c->cd(1);
	treeTrack->Draw("(adc[9]+adc[10]+adc[11]):(adc[8])>>h0(500, 0., 2500., 500, 0., 1800.)", cut0, "col");
	// set the name of the X-axis and the Y-axis
	TH2F *h0 = (TH2F*)f->Get("h0"); title = h0->GetTitle();
	title += ";MUSIC_BeforeTA [ch];MUSIC_AfterTA [ch]"; h0->SetTitle(title.c_str());
	
	c->cd(2);
	treeTrack->Draw("adc[12]:(adc[9]+adc[10]+adc[11])>>h1(500, 0., 3000., 500, 0., 3500.)", cut0, "col");
	TH2F *h1 = (TH2F*)f->Get("h1"); title = h1->GetTitle();
	title += ";MUSIC_AfterTA [ch];Si_AfterTA [ch]"; h1->SetTitle(title.c_str());

	c->cd(3);
	treeTrack->Draw("adc[12]:(adc[8])>>h2(500, 0., 2500., 500, 0., 3000.)", cut0, "col");
	// set the name of the X-axis and the Y-axis
	TH2F *h2 = (TH2F*)f->Get("h2"); title = h2->GetTitle();
	title += ";MUSIC_BeforeTA [ch];Si_AfterTA [ch]"; h2->SetTitle(title.c_str());
	
	c->cd(4);
	treeTrack->Draw("adc[12]*pow(beta2[0],2):(adc[9]+adc[10]+adc[11])*pow(beta2[0],2)>>h3(500, 0., 1200., 500, 0., 2000.)", cut0&&"beta2[0]>0.", "col");
	TH2F *h3 = (TH2F*)f->Get("h3"); title = h3->GetTitle();
	title += ";Z^{2}-MUSIC_AfterTA [arb.];Z^{2}-Si_AfterTA [arb.]"; h3->SetTitle(title.c_str());

	c->ToggleToolBar();
	c->Update();
	c->SetGrid();
	c->Write("", TObject::kOverwrite);
	h0->Write("", TObject::kOverwrite);
	h1->Write("", TObject::kOverwrite);
	h2->Write("", TObject::kOverwrite);
	h3->Write("", TObject::kOverwrite);

	return f;
}

int main(int argc, char *argv[]){
	if(argc <= 1){
		cout << "Usage: ./cut <rootfile>" << endl;
		exit(1);
	}
	const char *file = argv[1];
//	TApplication theApp("app", &argc, argv);

	TFile *f = cut(file);
	
//	theApp.Run();
	f->Close();
	return 0;
}







