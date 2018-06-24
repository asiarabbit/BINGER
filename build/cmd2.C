treeTrack->AddFriend(vme);
treeTrack->AddFriend(treeshoot);

treeTrack->Draw("aoz>>(500, 0., 4.)", "aoz>0.&&CUTG")
treeTrack->Draw("tof2:poz>>(200, 1200., 1900., 200, 45., 65.)", "aoz>0.&&CUTG")
treeTrack->Draw("(adc[8]):(adc[9]+adc[10]+adc[11])>>(300, 0., 2500., 300, 0., 2500.)", "t0_1_ok", "col")
treeTrack->Draw("(adc[8]):(adc[9]+adc[10]+adc[11])>>(500, 0., 5000., 500, 0., 5000.)", "mtdc0[3][0]==-9999&&mtdc0[4][0]==-9999&&t0_1_ok", "col")
treeTrack->Draw("(adc[8]):(adc[9]+adc[10]+adc[11])>>(500, 0., 2500., 500, 0., 2500.)", "(mtdc0[3][0]!=-9999||mtdc0[4][0]!=-9999)&&t0_1_ok", "col")
treeTrack->Draw("(adc[12]):(adc[9]+adc[10]+adc[11])>>(500, 0., 5000., 500, 0., 5000.)", "mtdc0[3][0]==-9999&&mtdc0[4][0]==-9999", "col")
