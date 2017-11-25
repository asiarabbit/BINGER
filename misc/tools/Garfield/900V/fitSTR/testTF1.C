// testTF1.C -- about Npx() function
void testTF1(){
	TFile *f = new TFile("STR/aaa900.root");
	TF1 *fSTR = (TF1*)f->Get("RT5");
	double r = fSTR->Eval(100.);
	cout << "r: " << r << endl;
}
