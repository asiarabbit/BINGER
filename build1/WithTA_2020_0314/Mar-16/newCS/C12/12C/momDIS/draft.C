TFile *f = new TFile("pplTA_N0_129309.root", "recreate");
h->SetName("hmomdisTA");
hmomdisTA->Write();
hmomdisTA->GetMean()
f->Close();



TFile *f = new TFile("pplNOTA_N0_52587.root", "recreate");
h->SetName("hmomdisNOTA");
hmomdisNOTA->Write();
f->Close();
