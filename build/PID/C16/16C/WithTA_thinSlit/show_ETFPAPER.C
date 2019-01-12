treeTrack->Draw("1.84+2.41*dE1:-0.0212056+0.975349*aoz[0]>>h(500, 1.8, 3.3, 500, 3., 9)", "dsca11==0&&A0&&B0&&C0&&(dx2[0]>-6&&dx2[0]<2.68)&&dsca[4]==1&&!OFFTA;aoz;dE1 arb.;", "col")
treeTrack->Draw("dE0+(PDCPos[1][1]*(0.005*dE0-0.0016)):tof1>>h(500, 120., 150., 500, 0., 2.2)", "dsca11==0&&PDCPos[1][1]!=-9999.&&B0&&C0&&dsca[4]==1;tof1 [ns];dE0-corrected arb.;", "col")
