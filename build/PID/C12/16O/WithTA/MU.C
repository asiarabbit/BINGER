treeTrack->Draw("dE0+(PDCPos[1][1]*(0.005*dE0-0.0016)):tof1>>h(500, 133., 159., 500, -0.5, 4.5)", "dsca11==0&&dsca4==1&&PDCPos[1][1]!=-9999.&&C0&&B0;tof1 [ns];dE0-corrected arb.;", "col");
treeTrack->Draw("dE0+(PDCPos[1][1]*(0.005*dE0-0.0016)):beta>>h(500, 0.56, 0.65, 500, -0.5, 4.5)", "dsca11==0&&dsca4==1&&PDCPos[1][1]!=-9999.&&C0&&B0;\\beta;dE0-corrected arb.;", "col");

