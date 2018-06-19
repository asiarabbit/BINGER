treeTrack->Draw("aoz>>(500, 1., 4.)", "mtdc0[3][0]==-9999&&mtdc0[4][0]==-9999&&(adc[7]+adc[8])>1380&&(adc[7]+adc[8])<1700&&tRef_pos>-0.25&&tRef_pos<0.22&&beta2[0]>0.6&&beta2[0]<0.8&&ntrLs[1][0]==1&&ntrLs[2][0]==1&&fabs(taHitPos[0][0]-taHitPos[1][0])<5.", "col")
treeTrack->Draw("(adc[12]*pow(beta2[0], 1.6)):((adc[9]+adc[10]+adc[11])*pow(beta2[0], 1.6))>>h(500, 0., 1000., 500, 0., 2000.)", "mtdc0[3][0]==-9999&&mtdc0[4][0]==-9999&&(adc[7]+adc[8])>1380&&(adc[7]+adc[8])<1700&&tRef_pos>-0.25&&tRef_pos<0.22&&beta2[0]>0.6&&beta2[0]<0.8&&ntrLs[1][0]==1&&ntrLs[2][0]==1&&fabs(taHitPos[0][0]-taHitPos[1][0])<5.", "col")

treeTrack->Draw("poz[0]:beta2[0]>>(500, 0.5, 0.8, 500, 500., 3000.)", "z6")

treeTrack->Draw("poz[0]:beta2[0]>>(500, 0.5, 0.8, 500, 500., 3000.)", "z6")
