void pile(){
    TRandom3 r;
    double p[6], pp;
    double dm;
    int cnt = 0;
    const int n = 10000000;
    TH1F *h = new TH1F("h", "h", 500, -100., 2500.);
    for(int i = 0; i < n; i++){
        dm = 2500.;
        pp = r.Uniform(0., 2400.);
        for(int j = 0; j < 6; j++){
            p[j] = r.Uniform(0., 2400.);
            double dd = fabs(p[j]-pp);
            if(dd < dm) dm = dd;
        }
        h->Fill(dm);
        if(dm < 10.) cnt++;
    }
    h->Draw();
    cout << "cnt/n: " << cnt/double(n) << endl;
}
