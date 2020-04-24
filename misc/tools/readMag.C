void readMag(){
    ifstream inFile("./mag_20180607.txt");
    string s;
    getline(inFile, s); // skip the commentary line
    double B; int i = 0;
    TGraph *g = new TGraph();
    while(inFile >> s >> s >> B >> s) g->SetPoint(i, i++, B);
    g->Draw("APL");
}
