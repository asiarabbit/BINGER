{
//========= Macro generated from object: A0/Graph
//========= by ROOT version6.13/02
   
   TCutG *cutg = new TCutG("A0",7);
   cutg->SetVarX("tof1");
   cutg->SetVarY("dE0");
   cutg->SetTitle("Graph");
   cutg->SetFillStyle(1000);
   cutg->SetPoint(0,133.947,1.88183);
   cutg->SetPoint(1,133.84,0.135504);
   cutg->SetPoint(2,134.277,-0.0877101);
   cutg->SetPoint(3,135.129,0.0961134);
   cutg->SetPoint(4,135.165,1.06775);
   cutg->SetPoint(5,135.05,1.84244);
   cutg->SetPoint(6,133.947,1.88183);
   cutg->Draw("A0.C");
}
