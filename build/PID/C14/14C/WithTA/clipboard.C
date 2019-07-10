treeTrack->Draw("adc[23]/1000:adc[22]/1000.", "dsca11==0&&dsca[4]==1&&A0&&B0&&C0&&B2&&qdc[4]>30&&qdc[6]>30", "colbox");
treeTrack->Draw("dE1:aoz[0]>>h(500, 1.4, 3.2, 500, 0., 4.5)", "dsca11==0&&A0&&B0&&C0&&(dx2[0]>-9&&dx2[0]<4);aoz;dE1 arb.;", "colbox");
p2->SetLineColor(2); p2->SetLineWidth(2);
// DCArr[L-R][DC0-1-2][XUV][X1-2]
treeTrack->Draw("multi_DC[1][0][0][0]>>(11, -0.5, 10.5)", "dsca11==0&&dsca[4]==1&&A0&&B0&&C0&&B2&&qdc[4]>30&&qdc[6]>30");

treeTrack->GetEntries("dsca11==0&&A0&&B0&&C0&&dsca[4]==1&&B2&&qdc[4]>30&&qdc[6]>30&&CUTG");
treeTrack->GetEntries("dsca11==0&&A0&&B0&&C0&&!(dsca[4]==1)&&B2&&qdc[4]>30&&qdc[6]>30&&CUTG");
p0->Draw(); p1->Draw(); p10->Draw(); p2->Draw(); p20->Draw(); p21->Draw(); p3->Draw();


// PDC Abnormity
treeTrack->Draw("PDCPos[0][1]:PDCPos[0][0]>>h(500, -60., 60., 500, -60., 60.)", "dsca11==0&&dsca[4]==1&&A0&&B0&&C0&&B2&&qdc[4]>30&&qdc[6]>30", "colbox");
treeTrack->Draw("taHitPos[0][1]:taHitPos[0][0]>>h(500, -60., 60., 500, -60., 60.)", "dsca11==0&&dsca[4]==1&&A0&&B0&&C0&&B2&&qdc[4]>30&&qdc[6]>30", "colbox");

treeTrack->Draw("PDCPos[2][1]:PDCPos[2][0]>>h(500, -100., 100., 500, -100., 100.)", "dsca11==0&&dsca[4]==1&&A0&&B0&&C0&&B2&&qdc[4]>30&&qdc[6]>30", "colbox");

treeTrack->Draw("PDCPos[3][1]:PDCPos[3][0]>>h(500, -100., 100., 500, -100., 100.)", "dsca11==0&&dsca[4]==1&&A0&&B0&&C0&&B2&&qdc[4]>30&&qdc[6]>30&&nuTa[1][0][0]>=0&&nuTa[1][0][1]>=0&&nuTa[1][0][2]>=0&&nuTa[1][0][3]>=0", "colbox")

treeTrack->Draw("PDCPos[2][1]:PDCPos[2][0]>>h(500, -100., 100., 500, -100., 100.)", "dsca11==0&&dsca[4]==1&&A0&&B0&&C0&&B2&&qdc[4]>30&&qdc[6]>30   &&nuTa[1][0][0]>=0&&nuTa[1][0][1]>=0&&nuTa[1][0][2]>=0&&nuTa[1][0][3]>=0&& chiTa[1][0][0]>-1.8&&chiTa[1][0][0]<0.1&&chiTa[1][0][1]>-1.8&&chiTa[1][0][1]<0.1&&chiTa[1][0][2]>-1.8&&chiTa[1][0][2]<0.1&&chiTa[1][0][3]>-1.8&&chiTa[1][0][3]<0.1      &&nuTa[1][1][0]>=0&&nuTa[1][1][1]>=0&&nuTa[1][1][2]>=0&&nuTa[1][1][3]>=0&& chiTa[1][1][0]>-1.8&&chiTa[1][1][0]<-0.1&&chiTa[1][1][1]>-1.8&&chiTa[1][1][1]<-0.1&&chiTa[1][1][2]>-1.8&&chiTa[1][1][2]<-0.1&&chiTa[1][1][3]>-1.8&&chiTa[1][1][3]<-0.1", "colbox")

treeTrack->Draw("PDCPos[1][1]:PDCPos[1][0]>>h(500, -100., 100., 500, -100., 100.)", "dsca11==0&&dsca[4]==1&&A0&&B0&&C0&&B2&&qdc[4]>30&&qdc[6]>30   &&nuTa[0][0][0]>=0&&nuTa[0][0][1]>=0&&nuTa[0][0][2]>=0&&nuTa[0][0][3]>=0&& chiTa[0][0][0]>-1.8&&chiTa[0][0][0]<0.1&&chiTa[0][0][1]>-1.8&&chiTa[0][0][1]<0.1&&chiTa[0][0][2]>-1.8&&chiTa[0][0][2]<0.1&&chiTa[0][0][3]>-1.8&&chiTa[0][0][3]<0.1      &&nuTa[0][1][0]>=0&&nuTa[0][1][1]>=0&&nuTa[0][1][2]>=0&&nuTa[0][1][3]>=0&& chiTa[0][1][0]>-1.8&&chiTa[0][1][0]<-0.1&&chiTa[0][1][1]>-1.8&&chiTa[0][1][1]<-0.1&&chiTa[0][1][2]>-1.8&&chiTa[0][1][2]<-0.1&&chiTa[0][1][3]>-1.8&&chiTa[0][1][3]<-0.1", "colbox")

