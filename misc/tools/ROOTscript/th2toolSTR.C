// th2toolSTR.C -- process TH2 objects: Projectioning and fitting - for assess2
#include "TAUSER.h"
#include "config/configure.C"

char dir[64] = "assessSTR1";
int main(int argc, char * argv[]){
	if(argc == 2) sprintf(dir, "assessSTR%s", argv[1]);
	cout << "assessSTR: specified folder name: " << dir << endl;
	cout << "input rootfile name: " << ccrootfile << endl;
	TFile *inFile = new TFile(ccSTRCaliFile);
	TF1 *RT2 = (TF1*)inFile->Get("RT2");
	inFile->Close();
	TFile *f = new TFile(ccrootfile.c_str(), "update");
	char name[64]; sprintf(name, "%s/hdrt04_STR_2", dir);
	TH2F *h2 = (TH2F*)f->Get(name);
	TH1D *hprojx = h2->ProjectionX();
	const int n = h2->GetNbinsX(), nn = n/3;
//	cout << "n: " << n << endl; getchar(); // DEBUG
	TF1 *fgaus = new TF1("fgaus", "gaus", -4., 4.);
//	fgaus->SetParameter(1, 0.); fgaus->SetParLimits(1, -2., 2.); // mean, unit: mm
//	fgaus->SetParameter(2, 0.2); fgaus->SetParLimits(2, 0., 2.); // sigma, unit: mm
	int gSigma_cnt = 0, gMean_cnt = 0;
	TGraph *gSigma = new TGraph(); // r-sigma for MWDC resolution estimation
	TGraph *gMean = new TGraph(); //r-dr for STR correction
	char title[128];
	sprintf(title, "Spatial Resolution v.s. Drift Distance(%s);r/mm;dr/mm", h2->GetTitle());
	gSigma->SetNameTitle("gSigma_4", title); gSigma->SetMarkerStyle(22);
	sprintf(title, "STR Correction v.s. Drift Distance(%s);r/mm;dr/mm", h2->GetTitle());
	gMean->SetNameTitle("gMean_4", title); gMean->SetMarkerStyle(22);
	for(int i = 0; i < nn; i++){
		TH1D *hproj = h2->ProjectionY("hproj", i*3+1, i*3+1+2);
		if(hproj->GetEntries() < 100) continue;
		hproj->Fit(fgaus, "NQR"); //
		double mean = fgaus->GetParameter("Mean");
		double sigma = fgaus->GetParameter("Sigma");
//		cout << "mean: " << mean << "\tsigma: " << sigma << endl; getchar(); // DEBUG
		if(fabs(mean) > 1.0 || sigma > 1.5) continue;
		double tm = hprojx->GetBinCenter(i*3+1+1);
		double rm = RT2->Eval(tm);
//		cout << "tm: " << tm << "\trm: " << rm << endl; getchar(); // DEBUG
		gSigma->SetPoint(gSigma_cnt++, rm, sigma);
		gMean->SetPoint(gMean_cnt++, rm, mean);
	} // end for over i

	if(!f->FindObjectAny(dir)) f->mkdir(dir);
	f->cd(dir);
	if(gSigma->GetN()) gSigma->Write("", TObject::kOverwrite);
	else cout << "\033[31;1mgSigma has no data.\033[0m\n";
	if(gMean->GetN()) gMean->Write("", TObject::kOverwrite);
	else cout << "\033[31;1mgMean has no data.\033[0m\n";

	return 0;
}
