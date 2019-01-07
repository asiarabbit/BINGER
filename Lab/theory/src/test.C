// test.C -- test the eikonal and Glauber theory //
// Author: SUN Yazhou, Nov., 2018. Last Modified: Nov. 26th, 2018

#include <iostream>
#include <cstdlib>

// ROOT header files
#include "TCanvas.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TFile.h"

#include "TACS.h"


using std::cout;
using std::endl;
using std::flush;

int main(){
	const int ZP = 6, NP = 6;
	const int ZT = 6, NT = 6;
	// projectile and target nucleon density distribution function
	const int lenP = 2, lenT = 2;
	double alpha[lenP] = {1.470, 1.636}, miu[lenP] = {-1.233, 1.385};
	double beta[lenT] = {1.470, 1.636}, niu[lenT] = {-1.233, 1.385};
	const double energy_projectile = 240.; // unit: MeV/u

	TACS *cs = new TACS(ZP, NP, ZT, NT);
	cs->SetProjectileNucleonDensity(lenP, alpha, miu);
	cs->SetTargetNucleonDensity(lenT, beta, niu);
	cs->SetIncidentBeamEnergy(energy_projectile);
	cs->SetAlphaNN(0.);
	cs->SetR0(1.0);
	cs->SetIntegrationStep(1e-4);
	cs->SetMaxImpactParameter(50.);

	const int n = 100;
	double cs_tot[n], cs_el[n], cs_rec[n], cs_int[n], e[n]; // cross section; energy
	double emin = 5., emax = 1200., de = (emax - emin) / n;
	for(int i = 1; i <= n; i++){
		e[i] = i*de;
		e[i] = 240.; // DEBUG
		cs->SetIncidentBeamEnergy(e[i]);

		cs_tot[i] = cs->TotalCS();
		cs_el[i] = cs->ElasticCS();
		cs_rec[i] = cs->ReactionCS();
		cs_int[i] = cs->InteractionCS();

		cs->ReactionInfo();
		cout << "e[" << i << "]: " << e[i] << "MeV/u" << endl;
		exit(1); // DEBUG
	} // end for over i
	TGraph *gcs_tot = new TGraph(n, e, cs_tot);
	TGraph *gcs_el = new TGraph(n, e, cs_el);
	TGraph *gcs_rec = new TGraph(n, e, cs_rec);
	TGraph *gcs_int = new TGraph(n, e, cs_int);

	gcs_tot->SetNameTitle("gcs_tot", "Total CS");
	gcs_el->SetNameTitle("gcs_el", "Elastic CS");
	gcs_rec->SetNameTitle("gcs_rec", "Reaction CS");
	gcs_int->SetNameTitle("gcs_int", "Interaction CS");

	gcs_tot->SetLineColor(1); gcs_tot->SetLineWidth(2);
	gcs_el->SetLineColor(2); gcs_el->SetLineWidth(2);
	gcs_rec->SetLineColor(3); gcs_rec->SetLineWidth(2);
	gcs_int->SetLineColor(4); gcs_int->SetLineWidth(2);

	TCanvas *c = new TCanvas("c", "cs", 800, 600);
	gcs_tot->Draw();
	gcs_el->Draw("samel");
	gcs_rec->Draw("samel");
	gcs_int->Draw("samel");
	
	TLegend *lg = new TLegend(0.6, 0.7, 0.89, 0.89);
	lg->SetBorderSize(0);
	lg->AddEntry(gcs_tot, "\\sigma_{tot}");
	lg->AddEntry(gcs_el, "\\sigma_{el}");
	lg->AddEntry(gcs_rec, "\\sigma_{rec}");
	lg->AddEntry(gcs_int, "\\sigma_{int}");
	lg->Draw();

	TFile *f = new TFile("cs_test.root", "UPDATE");
	gcs_tot->Write("", TObject::kOverwrite);
	gcs_el->Write("", TObject::kOverwrite);
	gcs_rec->Write("", TObject::kOverwrite);
	gcs_int->Write("", TObject::kOverwrite);
	c->Write("", TObject::kOverwrite);
	f->Close();

	return 0;
}







