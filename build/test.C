void test(){
    TFile *f = new TFile("assessmergeWithT0.root");
    new TBrowser();
    TH1F *hr_ = (TH1F*)f->Get("assess0R/misc0R/hr_");
    hr_->Draw();
}
