{
    TChain chain("cccs");
    //chain.Add("work.root");
 chain.Add("14C_5mmC_0710.004.root");
 chain.Add("14C_5mmC_0710.005.root");
 chain.Add("14C_5mmC_0711.002.root");
 chain.Add("14C_5mmC_0711.003.root");


//basic cut for all channles of electronis before the reaction target
//TOF det.,MWDCs, MUSIC, Si0
    TCut basic = "qdc[2]>0&&qdc[3]>0&&qdc[4]>0&&qdc[5]>0&&qdc[6]>0&&qdc[7]>0&&adc[0]>200&&adc[0]<3000&&adc[1]>200&&adc[1]<3000&&adc[2]>1000&&adc[2]<2500";
//qdc[3]>10&&tdc[0]>10&&qdc[1]>10&&tdc[1]>10&&qdc[7]>10&&tdc[2]>10&&tdc[16]>10&&tdc[17]>10&&tdc[18]>10&&tdc[19]>10&&tdc[20]>10&&tdc[21]>10&&tdc[22]>10&&tdc[23]>10&&tdc[24]>10&&tdc[25]>10&&adc[4]>10&&adc[5]>10&&adc[14]>10";

//pileup cut with ADC31
TCut pileup = "adc[3]<15";

TCanvas *c1 = new TCanvas("c1","h1",0,0,400,400);
/*c1->SetLogy();
c1->cd();cccs->Draw("adc[3]>>Pileup(4000,0,4000)",basic);
Pileup->GetXaxis()->SetTitle("Pileup logger (ch)");
Pileup->GetXaxis()->SetTitleOffset(1.2);
Pileup->GetYaxis()->SetTitle("Count");
Pileup->GetYaxis()->SetTitleOffset(1.4);
Pileup->GetYaxis()->SetAxisColor(17);
*/
/*
    TCanvas *c1 = new TCanvas("c1","h1",0,0,600,600);
c1->cd();cccs->Draw("adc[3]>>Pileup(4000,0,4000)",basic);

   TCanvas *c2 = new TCanvas("c2","h2",0,0,800,600);c2->Divide(2,1);
c2->cd(1);cccs->Draw("qdc[3]:xp[0]>>PL0_X(240,-6,6,4000,0,4000)",basic&&pileup,"colz");
c2->cd(2);cccs->Draw("qdc[3]:yp[0]>>PL0_Y(240,-6,6,4000,0,4000)",basic&&pileup,"colz");
   TCanvas *c21 = new TCanvas("c21","h21",0,0,800,600);c21->Divide(2,1);
c21->cd(1);cccs->Draw("qdc[3]:th[0]>>PL0_Th(240,-0.05,0.05,4000,0,4000)",basic&&pileup,"colz");
c21->cd(2);cccs->Draw("qdc[3]:ph[0]>>PL0_Ph(240,-0.05,0.05,4000,0,4000)",basic&&pileup,"colz");


   TCanvas *c3 = new TCanvas("c3","h3",0,0,800,800);c3->Divide(2,2);
c3->cd(1);cccs->Draw("qdc[4]:xp[0]>>PL1L_X(240,-6,6,600,0,600)",basic&&pileup,"colz");
c3->cd(2);cccs->Draw("qdc[6]:xp[0]>>PL1R_X(240,-6,6,600,0,600)",basic&&pileup,"colz");
c3->cd(3);cccs->Draw("qdc[4]:yp[0]>>PL1L_Y(240,-6,6,600,0,600)",basic&&pileup,"colz");
c3->cd(4);cccs->Draw("qdc[6]:yp[0]>>PL1R_Y(240,-6,6,600,0,600)",basic&&pileup,"colz");
   TCanvas *c31 = new TCanvas("c31","h31",0,0,800,800);c31->Divide(2,2);
c31->cd(1);cccs->Draw("qdc[4]:th[0]>>PL1L_Th(240,-0.05,0.05,600,0,600)",basic&&pileup,"colz");
c31->cd(2);cccs->Draw("qdc[6]:th[0]>>PL1R_Th(240,-0.05,0.05,600,0,600)",basic&&pileup,"colz");
c31->cd(3);cccs->Draw("qdc[4]:ph[0]>>PL1L_Ph(240,-0.05,0.05,600,0,600)",basic&&pileup,"colz");
c31->cd(4);cccs->Draw("qdc[6]:ph[0]>>PL1R_Ph(240,-0.05,0.05,600,0,600)",basic&&pileup,"colz");

   TCanvas *c4 = new TCanvas("c4","h4",0,0,800,600);c4->Divide(2,1);
c4->cd(1);cccs->Draw("sqrt(qdc[4]*qdc[6]):xp[0]>>PL1_X(240,-6,6,600,0,600)",basic&&pileup,"colz");
c4->cd(2);cccs->Draw("sqrt(qdc[4]*qdc[6]):yp[0]>>PL1_Y(240,-6,6,600,0,600)",basic&&pileup,"colz");
   TCanvas *c41 = new TCanvas("c41","h41",0,0,800,600);c41->Divide(2,1);
c41->cd(1);cccs->Draw("sqrt(qdc[4]*qdc[6]):th[0]>>PL1_Th(240,-0.05,0.05,600,0,600)",basic&&pileup,"colz");
c41->cd(2);cccs->Draw("sqrt(qdc[4]*qdc[6]):ph[0]>>PL1_Ph(240,-0.05,0.05,600,0,600)",basic&&pileup,"colz");


   TCanvas *c5 = new TCanvas("c5","h5",0,0,800,800);c5->Divide(2,2);
c5->cd(1);cccs->Draw("adc[16]:xp[0]>>MU1_1_X(240,-6,6,3000,0,3000)",basic&&pileup,"colz");
c5->cd(2);cccs->Draw("adc[17]:xp[0]>>MU1_2_X(240,-6,6,3000,0,3000)",basic&&pileup,"colz");
c5->cd(3);cccs->Draw("adc[16]:yp[0]>>MU1_1_Y(240,-6,6,3000,0,3000)",basic&&pileup,"colz");
c5->cd(4);cccs->Draw("adc[17]:yp[0]>>MU1_2_Y(240,-6,6,3000,0,3000)",basic&&pileup,"colz");
   TCanvas *c51 = new TCanvas("c51","h51",0,0,800,800);c51->Divide(2,2);
c51->cd(1);cccs->Draw("adc[16]:th[0]>>MU_1_Th(240,-0.05,0.05,3000,0,3000)",basic&&pileup,"colz");
c51->cd(2);cccs->Draw("adc[17]:th[0]>>MU_2_Th(240,-0.05,0.05,3000,0,3000)",basic&&pileup,"colz");
c51->cd(3);cccs->Draw("adc[16]:ph[0]>>MU_1_Ph(240,-0.05,0.05,3000,0,3000)",basic&&pileup,"colz");
c51->cd(4);cccs->Draw("adc[17]:ph[0]>>MU_2_Ph(240,-0.05,0.05,3000,0,3000)",basic&&pileup,"colz");
*/
/*
   TCanvas *c6 = new TCanvas("c6","h6",0,0,800,600);c6->Divide(2,1);
c6->cd(1);cccs->Draw("adc[18]:xp[0]>>Si0_X(240,-6,6,2500,0,2500)",basic&&pileup,"colz");
c6->cd(2);cccs->Draw("adc[18]:yp[0]>>Si0_Y(240,-6,6,2500,0,2500)",basic&&pileup,"colz");
   TCanvas *c61 = new TCanvas("c61","h61",0,0,800,600);c61->Divide(2,1);
c61->cd(1);cccs->Draw("adc[18]:th[0]>>Si0_Th(240,-0.05,0.05,2500,0,2500)",basic&&pileup,"colz");
c61->cd(2);cccs->Draw("adc[18]:ph[0]>>Si0_Ph(240,-0.05,0.05,2500,0,2500)",basic&&pileup,"colz");

   TCanvas *c7 = new TCanvas("c7","h7",0,0,800,600);c7->Divide(2,1);
c7->cd(1);cccs->Draw("sqrt(qdc[23]*qdc[27]):xp[0]>>Veto_X(240,-6,6,4000,0,4000)",basic&&pileup,"colz");
c7->cd(2);cccs->Draw("sqrt(qdc[23]*qdc[27]):yp[0]>>Veto_Y(240,-6,6,4000,0,4000)",basic&&pileup,"colz");
   TCanvas *c71 = new TCanvas("c71","h71",0,0,800,600);c71->Divide(2,1);
c71->cd(1);cccs->Draw("sqrt(qdc[23]*qdc[27]):th[0]>>Veto_Th(240,-0.05,0.05,4000,0,4000)",basic&&pileup,"colz");
c71->cd(2);cccs->Draw("sqrt(qdc[23]*qdc[27]):ph[0]>>Veto_Ph(240,-0.05,0.05,4000,0,4000)",basic&&pileup,"colz");
*/

//acceptance cut by PL1
TCut acc_pl = "yp[0]>-2.3&&yp[0]<2.&&xp[0]>-2.5&&xp[0]<2.5";
TCut acc_t = "xp[0]*xp[0]+yp[0]*yp[0]<2*2&&th[0]>-0.01&&th[0]<0.01&&ph[0]>-0.01&&ph[0]<0.01";
   
   TCanvas *c7 = new TCanvas("c7","h7",0,0,800,600);c7->Divide(2,1);
c7->cd(1);cccs->Draw("yp[0]:xp[0]>>Pos(240,-6,6,240,-6,6)",basic&&pileup&&acc_pl&&acc_t,"colz");
c7->cd(2);cccs->Draw("ph[0]:th[0]>>Ang(240,-0.05,0.05,240,-0.05,0.05)",basic&&pileup&&acc_pl&&acc_t,"colz");

//puerify cut by tof and pulse height
TCut pure = "(adc[16]+adc[17]+150*yp[0]+sqrt(qdc[4]*qdc[6]))/3<1400&&(adc[16]+adc[17]+150*yp[0]+sqrt(qdc[4]*qdc[6]))/3>300&&tof2>132&&tof2<148";

//cutoff the reacted 14C before target
TCut reac14C = "sqrt(qdc[4]*qdc[6])>218&&sqrt(qdc[4]*qdc[6])<365&&adc[16]>1040&&adc[16]<1600&&adc[17]>1000&&adc[17]<1550&&qdc[3]>1250&&qdc[3]<1750";
TCut reac16N = "sqrt(qdc[4]*qdc[6])>280&&sqrt(qdc[4]*qdc[6])<420&&adc[16]>1400&&adc[16]<2000&&adc[17]>1350&&adc[17]<1950&&qdc[3]>1550&&qdc[3]<2050";

TCut Cout14C = "((adc[22]-1731.21)/88.3/3.5)**2+((adc[23]/2+adc[24]/2-1759.08)/71.04/3.5)**2<1||(adc[22]>1731.21-2*88.3&&adc[23]/2+adc[24]/2>1759.08-2*71.04)";

//   TCanvas *c8 = new TCanvas("c8","h8",0,0,600,600);c8->Divide(2,1);
//c8->cd(1);cccs->Draw("(adc[16]+adc[17]+150*yp[0]+sqrt(qdc[4]*qdc[6]))/3:tof2>>PID(140,132,148,2000,00,2000)",basic&&pileup&&acc&&pure&&reac14C&&"dsca[11]==0&&dsca[10]==1","colz");
//c8->cd(2);cccs->Draw("adc[16]+50*yp[0]:tof2>>PID_MU11(140,132,148,4000,00,4000)",basic&&pileup&&acc&&pure&&reac14C&&"dsca[11]==0&&dsca[10]==1","colz");
TCanvas *c9 = new TCanvas("c9","h9",0,0,600,600);c9->Divide(2,1);
//c9->cd(1);cccs->Draw("adc[22]:(adc[23]+adc[24])/2>>ZID(5500,0,5500,5500,00,5500)",basic&&pileup&&acc&&pure&&"dsca[11]==0&&dsca[10]==1&&14C&&sqrt(qdc[4]*qdc[6])>218&&sqrt(qdc[4]*qdc[6])<365&&adc[16]>1040&&adc[16]<1600&&adc[17]>1000&&adc[16]<1550&&qdc[3]>1250&&qdc[3]<1750&&dsca[4]==1","col");

//c9->cd(2);cccs->Draw("adc[22]:(adc[23]+adc[24])/2>>sZID(5500,0,5500,5500,00,5500)",basic&&pileup&&acc&&pure&&"dsca[11]==0&&dsca[10]==1&&14C&&sqrt(qdc[4]*qdc[6])>218&&sqrt(qdc[4]*qdc[6])<365&&adc[16]>1040&&adc[16]<1600&&adc[17]>1000&&adc[16]<1550&&qdc[3]>1250&&qdc[3]<1750&&dsca[4]==1&&(((adc[22]-1731.21)/88.3/4.5)**2+((adc[23]/2+adc[24]/2-1759.08)/71.04/4.5)**2<1||(adc[22]>1731.21-2*88.3&&adc[23]/2+adc[24]/2>1759.08-2*71.04))","col");

 //  TCanvas *c9 = new TCanvas("c9","h9",0,0,600,600);c9->Divide(2,1);

}

