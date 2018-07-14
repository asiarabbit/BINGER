
#ifndef TH_HBOOK
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include "cfortran.h"
#include "hbook.h"
#else
#include  "TH_define.h"
#endif

#define _t_raw
#define _tof
#define gasdet_single_spectrum
#define gasdet_calibrated


void hbook_init()
{


// SSD & MUSIC & T0 & T1 

    HBOOK1(1, "event_monitor",1000,0.5,1000000.5,0);

    HBOOK1(2, "distr_fired_num_per_ch",3701,-100.5,3600.5,0);

    HBOOK1(3, "event_length_total (word)",1051,-50.5,1000.5,0);

    HBOOK1(4, "edgenumtotal",3701,-100.5,3600.5,0);
    HBOOK1(5, "Events_processed",2,-0.5,1.5,0);

    HBOOK2(7,"Edge_number_per_ch",3701,-100.5,3600.5,61,-10.5,50.5,0);

    HBOOK2(8,"Edge_number_Ld-Tr",3701,-100.5,3600.5,61,-10.5,50.5,0);
    HBOOK2(9,"Amplitude",3701,-100.5,3600.5,61,-100.5,1000.5,0);

    HBOOK2(10,"TOT_h_per_ch",3701,-100.5,3600.5,200,-10.5,1200.5,0);
    HBOOK2(11,"TOT_v_per_ch",3701,-100.5,3600.5,800,-50.5,300.5,0);

    HBOOK2(12,"TimeToTrig_h_per_ch",3701,-100.5,3600.5,200,-100.5,5000.5,0);
    HBOOK2(13,"TimeToTrig_v_per_ch",3701,-100.5,3600.5,200,-100.5,5000.5,0);


    HBOOK1(20, "multi_all_ch",111,-10.5,100.5,0);
    HBOOK1(21, "event_length_data (word)",1051,-50.5,1000.5,0);

  //  HBOOK2(22,"distr_PPAC1_YvsX",36,-9,9,36,-9,9,0);
   // HBOOK2(23,"distr_PPAC2_YvsX",36,-9,9,36,-9,9,0);
   // HBOOK2(24,"distr_PPAC3_YvsX",36,-9,9,36,-9,9,0);

  //  HBOOK2(25,"distr_QQAC1_YvsX",18,-9,9,18,-9,9,0);
   // HBOOK2(26,"distr_QQAC2_YvsX",18,-9,9,18,-9,9,0);
  //  HBOOK2(27,"distr_QQAC3_YvsX",18,-9,9,18,-9,9,0);

    HBOOK1(28,"SC0_dn_minus_up",1001,-10.05,10.05,0);
    HBOOK1(29,"SC_dn_minus_up",1001,-10.05,10.05,0);

    HBOOK1(30,"T_SC0_to_SC",2401,30.05,230.05,0);

    HBOOK1(101,"distr_fired_num_TOFWup",41,-5.5,35.5,0);
    HBOOK1(102,"distr_fired_num_TOFWdn",41,-5.5,35.5,0);
    HBOOK1(103,"distr_fired_num_MWDC1X1",91,-5.5,85.5,0);
    HBOOK1(104,"distr_fired_num_MWDC1X2",91,-5.5,85.5,0);
    HBOOK1(105,"distr_fired_num_MWDC2X1",91,-5.5,85.5,0);
    HBOOK1(106,"distr_fired_num_MWDC2X2",91,-5.5,85.5,0);
    HBOOK1(107,"distr_fired_num_MWDC3X1",91,-5.5,85.5,0);
    HBOOK1(108,"distr_fired_num_MWDC3X2",91,-5.5,85.5,0);

   // HBOOK1(113,"distr_fired_num_PPAC1X1",18,-0.5,17.5,0);
   // HBOOK1(114,"distr_fired_num_PPAC1X2",18,-0.5,17.5,0);
   // HBOOK1(115,"distr_fired_num_PPAC2X1",18,-0.5,17.5,0);
   // HBOOK1(116,"distr_fired_num_PPAC2X2",18,-0.5,17.5,0);
  //  HBOOK1(117,"distr_fired_num_PPAC3X1",18,-0.5,17.5,0);
   // HBOOK1(118,"distr_fired_num_PPAC3X2",18,-0.5,17.5,0);

 //   HBOOK1(123,"distr_fired_num_PPAC1Y1",18,-0.5,17.5,0);
 //   HBOOK1(124,"distr_fired_num_PPAC1Y2",18,-0.5,17.5,0);
   // HBOOK1(125,"distr_fired_num_PPAC2Y1",18,-0.5,17.5,0);
   // HBOOK1(126,"distr_fired_num_PPAC2Y2",18,-0.5,17.5,0);
  //  HBOOK1(127,"distr_fired_num_PPAC3Y1",18,-0.5,17.5,0);
 //   HBOOK1(128,"distr_fired_num_PPAC3Y2",18,-0.5,17.5,0);

 //   HBOOK1(133,"distr_fired_num_QQAC1X1",18,-0.5,17.5,0);
 //   HBOOK1(134,"distr_fired_num_QQAC1X2",18,-0.5,17.5,0);
   // HBOOK1(135,"distr_fired_num_QQAC2X1",18,-0.5,17.5,0);
  //  HBOOK1(136,"distr_fired_num_QQAC2X2",18,-0.5,17.5,0);
  //  HBOOK1(137,"distr_fired_num_QQAC3X1",18,-0.5,17.5,0);
  //  HBOOK1(138,"distr_fired_num_QQAC3X2",18,-0.5,17.5,0);

 //   HBOOK1(143,"distr_fired_num_QQAC1Y1",18,-0.5,17.5,0);
 //   HBOOK1(144,"distr_fired_num_QQAC1Y2",18,-0.5,17.5,0);
   // HBOOK1(145,"distr_fired_num_QQAC2Y1",18,-0.5,17.5,0);
  //  HBOOK1(146,"distr_fired_num_QQAC2Y2",18,-0.5,17.5,0);
  //  HBOOK1(147,"distr_fired_num_QQAC3Y1",18,-0.5,17.5,0);
  //  HBOOK1(148,"distr_fired_num_QQAC3Y2",18,-0.5,17.5,0);


    HBOOK1(201,"multi_TOFWup",32,-1.5,30.5,0);
    HBOOK1(202,"multi_TOFWdn",32,-1.5,30.5,0);
    HBOOK1(203,"multi_MWDC1X1",12,-1.5,10.5,0);
    HBOOK1(204,"multi_MWDC1X2",12,-1.5,10.5,0);
    HBOOK1(205,"multi_MWDC2X1",12,-1.5,10.5,0);
    HBOOK1(206,"multi_MWDC2X2",12,-1.5,10.5,0);
    HBOOK1(207,"multi_MWDC3X1",12,-1.5,10.5,0);
    HBOOK1(208,"multi_MWDC3X2",12,-1.5,10.5,0);

  //  HBOOK1(213,"multi_PPAC1X1",12,-1.5,10.5,0);
  //  HBOOK1(214,"multi_PPAC1X2",82,-1.5,80.5,0);
   // HBOOK1(215,"multi_PPAC2X1",82,-1.5,80.5,0);
   // HBOOK1(216,"multi_PPAC2X2",82,-1.5,80.5,0);
  //  HBOOK1(217,"multi_PPAC3X1",82,-1.5,80.5,0);
  //  HBOOK1(218,"multi_PPAC3X2",82,-1.5,80.5,0);

  //  HBOOK1(223,"multi_PPAC1Y1",82,-1.5,80.5,0);
 //   HBOOK1(224,"multi_PPAC1Y2",82,-1.5,80.5,0);
  //  HBOOK1(225,"multi_PPAC2Y1",82,-1.5,80.5,0);
  //  HBOOK1(226,"multi_PPAC2Y2",82,-1.5,80.5,0);
 //   HBOOK1(227,"multi_PPAC3Y1",82,-1.5,80.5,0);
  //  HBOOK1(228,"multi_PPAC3Y2",82,-1.5,80.5,0);

  //  HBOOK1(233,"multi_QQAC1X1",82,-1.5,80.5,0);
 //   HBOOK1(234,"multi_QQAC1X2",82,-1.5,80.5,0);
   // HBOOK1(235,"multi_QQAC2X1",82,-1.5,80.5,0);
  //  HBOOK1(236,"multi_QQAC2X2",82,-1.5,80.5,0);
  //  HBOOK1(237,"multi_QQAC3X1",82,-1.5,80.5,0);
  //  HBOOK1(238,"multi_QQAC3X2",82,-1.5,80.5,0);

  //  HBOOK1(243,"multi_QQAC1Y1",82,-1.5,80.5,0);
  //  HBOOK1(244,"multi_QQAC1Y2",82,-1.5,80.5,0);
   // HBOOK1(245,"multi_QQAC2Y1",82,-1.5,80.5,0);
  //  HBOOK1(246,"multi_QQAC2Y2",82,-1.5,80.5,0);
 //   HBOOK1(247,"multi_QQAC3Y1",82,-1.5,80.5,0);
 //   HBOOK1(248,"multi_QQAC3Y2",82,-1.5,80.5,0);

    HBOOK1(601,"distr_fired_num_OPFup",51,-5.5,45.5,0);
    HBOOK1(602,"distr_fired_num_OPFdn",51,-5.5,45.5,0);
    HBOOK1(501,"multi_OPFup",42,-1.5,40.5,0);
    HBOOK1(502,"multi_OPFdn",42,-1.5,40.5,0);
    HBOOK2(701,"T_SC_to_OPF;stripId;tof1 [ns]",51,-5.5,45.5,2001,-200.05,0.05,0);
    HBOOK2(702,"OPFup - OPFdn;stripId;dt [ns]",51,-5.5,45.5,2001,-100.05,100.05,0);


 }















