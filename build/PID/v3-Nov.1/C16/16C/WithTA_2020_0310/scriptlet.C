treeTrack->Draw("((kTa[0][0]*sqrt((1.+k[0]*k[0])/(1.+kTa[0][0]*kTa[0][0])) + k[0] - (kTa[0][0]*k[0]+sqrt((1.+kTa[0][0]*kTa[0][0])*(1.+k[0]*k[0]))-1.)/(kTa[0][0]+k[0]))/(kTa[0][0]+k[0]))>>h1(500, -0.1, 0.1)", "A0&&B0&&C0&&dsca11==0&&dE1>1.5&&CUTG1", "");
(kTa[0][0]*sqrt((1.+k[0]*k[0])/(1.+kTa[0][0]*kTa[0][0])) + k[0] - (kTa[0][0]*k[0]+sqrt((1.+kTa[0][0]*kTa[0][0])*(1.+k[0]*k[0]))-1.)/(kTa[0][0]+k[0]))/(kTa[0][0]+k[0])


treeTrack->Draw("taHitPos[1][0]:dx2[0]>>(500, -100., 150., 500, -100., 100.)", "A0&&B0&&C0&&dsca11==0&&dE1>1.50", "col")

 treeTrack->Draw("taHitPos[0][0]:dx2[0]>>(500, -100., 150., 500, -100., 100.)", "A0&&B0&&C0&&dsca11==0&&dE1>1.50&&CUTG1", "col")


