/////////////////////////////////////////////////////////
// File name: Online.cpp                               //
// Brief introduction:                                 //
//       Online program of RIBLL2-DAQ                  //
//                                                     //
// Version: V1.0                                       //
// Author: Han Jianlong                                //
// Date: Mar. 2012                                     //
// For: RIBLL2                                         //
/////////////////////////////////////////////////////////
#include <time.h>
#include "TApplication.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TGClient.h"
#include "TGWindow.h"
#include "TGButton.h"
#include "TGListView.h"
#include "TVirtualPadEditor.h"
#include "GuiTypes.h"
#include "TGFSContainer.h"
#include "WidgetMessageTypes.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "OnlineFrame.h"
#include "Online.h"

#ifndef WIN32
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/types.h>
#endif //#ifndef WIN32
#include <sys/types.h>

#include <iostream>
using namespace std;
#include  "TH_define.h"

#include <iomanip>
#include <fstream>
//#include <strstream>
#include <cstdio>
#include <cstdlib>

extern int event_num;
extern int event_num1;

extern const int ch_num_limit;
extern const int edge_num_limit;
extern float data_v_leading_f[3500][20];
extern float T_SC0;

extern float P1_X , P1_Y , P2_X , P2_Y , P3_X , P3_Y ; 
extern float Q1_X , Q1_Y , Q2_X , Q2_Y , Q3_X , Q3_Y ; 



extern void Analyze(int* data);
extern void TH_fill();
extern void TH_init();
extern void UserFunc(OnlineFrame *onl);
extern void init_variable();

Online::Online(const TGWindow *p, UInt_t w, UInt_t h):
OnlineFrame(p, w, h)
{
	datafile = new TFile("Ronline.root", "RECREATE");
	TH_init();
	ObjList = datafile->GetList();
	fFileCont->AddFile(datafile->GetName());
	fFileCont->Resize();
	setonline = true;

	MakeTH1MenuList();
	RemoveMenuEntry("Delete", THmlist);
	RemoveMenuEntry("Dump", THmlist);
	RemoveMenuEntry("SetName", THmlist);
	RemoveMenuEntry("SetMaximum", THmlist);
	RemoveMenuEntry("SetMinimum", THmlist);
	RemoveMenuEntry("ShowBackground", THmlist);

	MakeTH2MenuList();
	RemoveMenuEntry("Delete", TH2mlist);
	RemoveMenuEntry("Dump", TH2mlist);
	RemoveMenuEntry("SetName", TH2mlist);
	RemoveMenuEntry("SetMaximum", TH2mlist);
	RemoveMenuEntry("SetMinimum", TH2mlist);
	RemoveMenuEntry("ShowBackground", TH2mlist);
	UserFunc(gOnlineFrame);
		SetOnline();
}


bool Online::ProcessMessage(Long_t msg, Long_t param1, Long_t)
{
	switch (GET_MSG(msg))
	{
	case kC_COMMAND:
		switch(GET_SUBMSG(msg))
		{
		case kCM_BUTTON:
			switch (param1)
			{
			case kB_online:
				SetOnline();
				break;
			case kB_offline:
				SetOffline();
				break;
			case kB_exit:
				CloseWindow();
				break;
			case kB_resetall:
				ImB_ResetAllTH();
				UserFunc(gOnlineFrame);
				break;
			case kB_resetcurr:
				ImB_ResetCurrTH();
				UserFunc(gOnlineFrame);
				break;
			case kB_integral:
				ImB_integral();
				break;
			case kB_privious:
				ImB_previous();
				break;
			case kB_next:
				ImB_next();
				break;
			case kB_update:
				ImB_update();
				break;
			default:
				break;
			}
			break;
		}
		break;

	case kC_CONTAINER:
		switch(GET_SUBMSG(msg))
		{
		case kCT_ITEMDBLCLICK:
			if (param1==kButton1) OnDoubleClick((TGLVEntry *)fFileCont->GetLastActive(), param1);
			break;
		default:
			break;
		}
		break;

	default:
		break;
	}

	return true;
}

Online::~Online()
{
    delete[] onevent;
	setonline = false;
	if (TVirtualPadEditor::GetPadEditor(kFALSE) != 0)
		TVirtualPadEditor::Terminate();

}

void Online::SetOffline()
{
	setonline = false;
	fBonline->SetEnabled(true);
	ShowText("Set Offline.");
}

void Online::SetOnline()
{




     time_t tim=0;
 /////////////////////////// input
    online_shm_interface* p_output_online_shm_interface;    // input interface 
    char on_shm[] ="online_shm";
    if(online_interface_open(on_shm, &p_output_online_shm_interface)!=0) cout<<"open shm error"<<endl;
    int input_online_data[DEFAULT_INTERFACE_ONLINE_LEN]={0};
    int nstored=-1;
/////////////////////////// input 
////////////////////// output
      static online_shm_interface1* p_output_online_shm_interface1;    // input interface 
    char on_shm1[] ="online_shm_PXI1";
    online_interface_destroy1(on_shm1, p_output_online_shm_interface1);
    if(online_interface_create1(on_shm1, &p_output_online_shm_interface1)!=0)
    {
         cout<<"online_shm error 1 "<<endl;
    }
    
    if(online_interface_open1(on_shm1, &p_output_online_shm_interface1)!=0)
    {
         cout<<"online_shm error 2"<<endl;
    }

    int output_online_data1[DEFAULT_INTERFACE_ONLINE_LEN1]={300};

////////////////////// output







    int event_counts_per_sec = 0, event_num_previous = 0;
    int event_counts_per_pulse = 0;

    char strtemp[MAX_TMP_STRING_LENGTH];  
    int time_current_event,time_last_event;

    init_variable();
 
  while(1)
  {  
      gSystem->ProcessEvents();
     // input_online_data[0]=0;
      time_last_event=tim;
      time(&tim);    

      time_current_event=tim;
      strftime(strtemp,MAX_TMP_STRING_LENGTH,"%T",localtime(&tim)); // if(tim%3==0)
      if(time_current_event!=time_last_event)ImB_update();

      sem_getvalue(&p_output_online_shm_interface->nstored,&nstored);


    if(time_current_event!=time_last_event)
    {
      if(nstored==DEFAULT_INTERFACE_ONLINE_NUM)
      {cout<<endl<<"Event num:  "<<event_num1<<"  Stored num is ovfl "<<strtemp<<"  !!!!!!!!!";
      }
      else if(nstored==0)
      {cout<<endl<<"Event num:  "<<event_num1<<"  Stored num is zero "<<strtemp;
      }
      else 
      {cout<<endl<<"Event num:  "<<event_num1<<"  Stored num is norm "<<strtemp;
      }

      event_counts_per_sec = event_num - event_num_previous;
      event_num_previous = event_num;
      cout<<"   Events/sec:  "<<setw(4)<<event_counts_per_sec;
      if(event_counts_per_sec != 0){event_counts_per_pulse+=event_counts_per_sec;cout<<"    "<<setw(4)<<event_counts_per_pulse<<endl;}      
      else {event_counts_per_pulse=0; cout<<endl;}

   }
      //if(nstored==0)                           {cout<<"Stored num is zero    "<<strtemp<<endl; }

      //  if(time_current_event!=time_last_event && time_current_event%10==0) cout<<endl<<"Event num: "<<event_num1<<endl;


     if(nstored != 0)
     {
          if(online_interface_get_task(p_output_online_shm_interface, &input_online_data[0])!=0)
          cout<<"get task error"<<endl;

         if(input_online_data[0]<=DEFAULT_INTERFACE_ONLINE_LEN-1)
        {
           Analyze(input_online_data);
           TH_fill();
           HF1(5,1,1.);

////////////////output
      output_online_data1[0]=4; // Event_length
      output_online_data1[1]=event_num1; // Event_number
      output_online_data1[2]=T_SC0;
      output_online_data1[3]=P2_X;
      output_online_data1[4]=P2_Y;

      int nempty1=-1;
      sem_getvalue(&p_output_online_shm_interface1->nempty,&nempty1);

      if(nempty1!=0)
        if(online_interface_add_task1(&output_online_data1[0],p_output_online_shm_interface1)!=0)
        {
         cout<<"online_shm error 3"<<endl;
        }
////////////////output







        }
         else
        {
          HF1(5,0,1.);
        }
     }
     else
     {	usleep(100000);		
     }


       //gSystem->ProcessEvents();

   }//while(1)






/*


	if ((shmid = shmget(SHMKEY, sizeof(struct SHM_VME), 0600)) == -1)
	{      	ShowText("ANALYZER: Cannot create shared memory"); 
		//exit(2);
		return;
	}
	
	shmp = (struct SHM_VME *)shmat(shmid, 0, 0);
        
	//event_length=shmp->event_length;
      
	// Map the shared memory space for PAW analyze, and create the histograms

	//  calibrate();

	shmp->ana_status=1;
	//onevent = new unsigned int[300];
	onevent = new unsigned int[1000];
	TString texteventsize = "Eventsize = ";
	//TString texteventsize = "Set Online ";
        texteventsize += event_length;
	ShowText(texteventsize.Data());

       
	setonline = true;
	fBonline->SetEnabled(false);
        
        // block[0]: block no. block[1]: number of events in this block. block[2]: number of <int>s
        // event[0]: event length. event[1]: event no=event_num. event[2]: fired VME boards.
                 
//int pqr=0;
L0:

     int proc_0=0;
     if(shmp->status==0){ cout<<"Waiting for daq start."<<endl;}
     while(shmp->status==0)
    { 
     usleep(100);
    
     proc_0++;if(proc_0%1000==0){gSystem->ProcessEvents();}



    }
	if(shmp->status==1)
	{
		analys_block=shmp->block_no;
		if(analys_block>0)
			{block_num = (analys_block-1)%MAX_BLOCK;}
		else
			{block_num = 0; cout<<endl<<analys_block<<endl;}
	}

	pos=3;

	while(setonline)     // 1st while
	{

               
			gSystem->ProcessEvents();
			
			if (pos == 3)
			{    //cout<<endl<<"pos=3"<<endl;
				while (shmp->buffer[block_num].data[1] == 0) usleep(50);
				while (shmp->buffer[block_num].data[1] == 1) usleep(50);
				event_length = shmp->buffer[block_num].data[pos];
				memcpy(onevent, &shmp->buffer[block_num].data[pos], event_length*sizeof(int));
			}
			else
			{    int proc=0;//cout<<endl<<"p3 "<<pqr++<<endl;

				while (event_no == shmp->event_no-1){usleep(100);
                    proc++;if(proc%1000==0){gSystem->ProcessEvents(); }}
				while (event_no == shmp->event_no)  {usleep(100);
                    proc++;if(proc%1000==0){gSystem->ProcessEvents(); }}
				while (event_no == shmp->event_no-1){usleep(100);
                    proc++;if(proc%1000==0){gSystem->ProcessEvents(); }}

				event_length = shmp->buffer[block_num].data[pos];
				if(event_length == 0xFFFFFFFF) goto L0;
				memcpy(onevent, &shmp->buffer[block_num].data[pos], event_length*sizeof(int));
			}
			
			if(shmp->block_no < analys_block){cout<<endl<<"p1"<<endl; goto L0; }    // for "clear"
			if(shmp->block_no >= MAX_BLOCK+analys_block) {cout<<endl<<"p2"<<endl; goto L0; }
			if(pos >= shmp->buffer[block_num].data[2]) {cout<<endl<<pos<<"  "<<shmp->buffer[block_num].data[2]<<endl; goto L0;}
			event_no = shmp->buffer[block_num].data[pos+1]; 
			pos = pos + event_length;

			Analyze(onevent);
			TH_fill();
			
			gSystem->ProcessEvents();
			
			//if (shmp->buffer[block_num].data[pos] == 0xFFFFFFFF) goto L0;


	}//  1st while
 
	delete[] onevent;

*/
}

void ronline()
{
	Online *onl = new Online(gClient->GetRoot(), 100, 200);
}

#if !defined(__CINT__) && !defined(__MAKECINT__)

int main(int argc, char *argv[])
{

	TApplication apponline("Online", &argc, argv);

	Online *onl = new Online(gClient->GetRoot(), 100, 200);

	apponline.Run();

	return 0;
}

#endif// #if !defined(__CINT__) && !defined(__MAKECINT__)
