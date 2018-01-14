void test1(){
    TFile *f = new TFile("20161123_2247.dat_0.root");
    TBrowser *b = new TBrowser();
    TH1F *h = (TH1F*)f->Get("hTOFInVMEvsTOFInPXI");
    h->Draw();
}
