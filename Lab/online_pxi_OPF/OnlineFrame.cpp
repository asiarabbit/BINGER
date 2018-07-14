/////////////////////////////////////////////////////////
// File name: OnlineFrame.cpp                          //
// Brief introduction:                                 //
//       This class create the main frame for          //
//       Online program of RIBLL2-DAQ                  //
//                                                     //
// Version: V1.0                                       //
// Author: Han Jianlong                                //
// Date: Mar. 2012                                     //
// For: RIBLL2                                         //
/////////////////////////////////////////////////////////

#include "TApplication.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TClass.h"
#include "TCanvas.h"
#include "TGClient.h"
#include "TGWindow.h"
#include "GuiTypes.h"
#include "TGSplitter.h"
#include "TGLayout.h"
#include "TVirtualX.h"
#include "TG3DLine.h"
#include "TGText.h"
#include "TGLabel.h"
#include "TGComboBox.h"
#include "TGListView.h"
#include "TGFSContainer.h"
#include "TGButton.h"
#include "TGTextView.h"
#include "TCanvasImp.h"
#include "TRootCanvas.h"
#include "TStyle.h"
#include "TVirtualX.h"
#include "TString.h"
#include "TObject.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TKey.h"
#include "TGFSContainer.h"
#include "TVirtualPad.h"
#include "TPad.h"
#include "TH1.h"
#include "TH2.h"
#include "TIterator.h"
#include "TCollection.h"
#include "TMarker.h"
#include "TAxis.h"
#include "TContextMenu.h"
#include "TClassMenuItem.h"
#include "OnlineFrame.h"

#include <iostream>
#include <ctime>
#include <string.h>
using namespace std;

OnlineFrame *gOnlineFrame =0; 

OnlineFrame::OnlineFrame(const TGWindow *p, UInt_t w, UInt_t h):
TGMainFrame(p, w, h, kHorizontalFrame)
{
	if(gOnlineFrame!=0) 
	{
		cout<<"Only one instance of 'Online' permitted"<<endl;
		return;
	}
	gOnlineFrame = this;

	SetCleanup(kDeepCleanup);
	DontCallClose();
	ObjList = 0;
	objcurr = 0;
	fFFileList = new TGVerticalFrame(this, 210, 350, kChildFrame|kVerticalFrame|kRaisedFrame|kFixedWidth);
	AddFrame(fFFileList, new TGLayoutHints(kLineSolid, 1, 1, 1, 1));

	TGHorizontalFrame *fFdrawopt = new TGHorizontalFrame(fFFileList, 75, 10);
	flabDrawOpt = new TGLabel(fFdrawopt, " Draw Option: ");
	fFdrawopt->AddFrame(flabDrawOpt, new TGLayoutHints(kLHintsLeft|kLHintsCenterY, 10, 1, 1, 1));
	fcomDrawOpt = new TGComboBox(fFdrawopt);
	fcomDrawOpt->Resize(70, 25);
	fcomDrawOpt->AddEntry("",    0);
	fcomDrawOpt->AddEntry("COLZ", 1);
	fcomDrawOpt->AddEntry("LEGO", 2);
	fcomDrawOpt->AddEntry("CONT", 3);
	fcomDrawOpt->AddEntry("SURF", 4);
	fFdrawopt->AddFrame(fcomDrawOpt, new TGLayoutHints(kLHintsRight|kLHintsCenterY, 1, 1, 1,1));
	fFFileList->AddFrame(fFdrawopt, new TGLayoutHints(kLHintsNormal, 1, 1, 1, 1));

	TGHorizontal3DLine *opsep = new TGHorizontal3DLine(fFFileList, 210, 3);
	fFFileList->AddFrame(opsep, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

	TGHorizontalFrame *fFDivPad = new TGHorizontalFrame(fFFileList, 210, 15);
	flabDivPad = new TGLabel(fFDivPad, " Divide Pad: ");
	fFDivPad->AddFrame(flabDivPad, new TGLayoutHints(kLHintsLeft|kLHintsCenterY, 19, 1, 1, 1));

	fcomDivPad = new TGComboBox(fFDivPad);
	fcomDivPad->Resize(70, 25);
	fcomDivPad->AddEntry("1 X 1", 0);
	fcomDivPad->AddEntry("2 X 2", 1);
	fcomDivPad->AddEntry("2 X 3", 2);
	fcomDivPad->AddEntry("3 X 3", 3);
	fcomDivPad->AddEntry("4 X 4", 4);
	fcomDivPad->Select(0, false);
	fFDivPad->AddFrame(fcomDivPad, new TGLayoutHints(kLHintsRight|kLHintsCenterY, 1, 1, 1,1));
	fFFileList->AddFrame(fFDivPad, new TGLayoutHints(kLHintsNormal, 1, 1, 1, 1));

	TGHorizontal3DLine *pfvsep = new TGHorizontal3DLine(fFFileList, 210, 3);
	fFFileList->AddFrame(pfvsep, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

	flvFile = new TGListView(fFFileList, 200, 330);
	fFFileList->AddFrame(flvFile, new TGLayoutHints(kLHintsNormal, 1, 1, 1, 1));

	Pixel_t white;
	gClient->GetColorByName("white", white);
	fFileCont = new TGFileContainer(flvFile, kSunkenFrame, white);
	fFileCont->Associate(this);

	TGHorizontalFrame *fFvsp = new TGHorizontalFrame(this, 5, 400, kChildFrame|kRaisedFrame);
	TGVSplitter *vsp = new TGVSplitter(fFvsp, 4, 4);
	vsp->SetFrame(fFFileList, true);
	fFvsp->AddFrame(vsp, new TGLayoutHints(kLHintsExpandY));
	AddFrame(fFvsp, new TGLayoutHints(kLHintsNormal|kLHintsExpandY, 1, 1, 0, 0));

	TGVerticalFrame *fFRight = new TGVerticalFrame(this, 300, 400, kChildFrame|kFixedHeight|kFixedWidth);
	fFButton = new TGVerticalFrame(fFRight, 300, 150, kChildFrame|kRaisedFrame|kFixedWidth|kFixedHeight);
	TGHorizontalFrame *butt1 = new TGHorizontalFrame(fFButton, 300, 50, kFitWidth);
	TGLayoutHints *flayoutButt = new TGLayoutHints(kLHintsNormal|kLHintsCenterX|kLHintsCenterY, 5, 5, 7, 7);

	fBonline  =  new TGTextButton(butt1,  "  Online   ", kB_online);
	fBoffline =  new TGTextButton(butt1,  "  Offline  ", kB_offline);
	fBexit    =  new TGTextButton(butt1,  "   Exit    ", kB_exit);
	butt1->AddFrame(fBonline, flayoutButt);
	butt1->AddFrame(fBoffline, flayoutButt);
	butt1->AddFrame(fBexit, flayoutButt);

	TGHorizontalFrame *butt2 = new TGHorizontalFrame(fFButton, 300, 50, kFitWidth);
	fBresetall =  new TGTextButton(butt2, " Reset All ", kB_resetall);
	fBresetcurr = new TGTextButton(butt2, " Reset Cur ", kB_resetcurr);
	fBintegral =  new TGTextButton(butt2, "  Integral ", kB_integral);
	butt2->AddFrame(fBresetcurr, flayoutButt);
	butt2->AddFrame(fBresetall,  flayoutButt);
	butt2->AddFrame(fBintegral,  flayoutButt);

	TGHorizontalFrame *butt3 = new TGHorizontalFrame(fFButton, 300, 50, kFitWidth);
	fBprevious = new TGTextButton(butt3, " Previous ", kB_privious);
	fBnext     = new TGTextButton(butt3, "   Next   ", kB_next);
	fBupdate   = new TGTextButton(butt3, "  Update  ", kB_update);
	butt3->AddFrame(fBprevious, flayoutButt);
	butt3->AddFrame(fBnext, flayoutButt);
	butt3->AddFrame(fBupdate, flayoutButt);


	fBonline->Associate(this);
	fBoffline->Associate(this);
	fBexit->Associate(this);
	fBresetall->Associate(this);
	fBresetcurr->Associate(this);
	fBintegral->Associate(this);
	fBprevious->Associate(this);
	fBnext->Associate(this);
	fBupdate->Associate(this);

	fBonline->SetToolTipText("Analyze online");
	fBoffline->SetToolTipText("Stop analysis");
	fBexit->SetToolTipText("Exit this program");
	fBresetall->SetToolTipText("Reset all histograms");
	fBresetcurr->SetToolTipText("Reset current histogram");
	fBintegral->SetToolTipText("Integral given bins");
	fBprevious->SetToolTipText("Draw previous histogram");
	fBnext->SetToolTipText("Draw next histogram");
	fBupdate->SetToolTipText("Update current histogram");

	fFButton->AddFrame(butt1, new TGLayoutHints(kLHintsCenterX, 2, 2, 5, 5));
	fFButton->AddFrame(butt2, new TGLayoutHints(kLHintsCenterX, 2, 2, 5, 5));
	fFButton->AddFrame(butt3, new TGLayoutHints(kLHintsCenterX, 2, 2, 5, 5));

	fFRight->AddFrame(fFButton, new TGLayoutHints(kLHintsCenterX, 2, 2, 8, 8));

	TGVerticalFrame *fFvsp2 = new TGVerticalFrame(fFRight, 300, 15, kChildFrame|kRaisedFrame);
	TGHSplitter *vsp2 = new TGHSplitter(fFvsp2, 5, 5);
	vsp2->SetFrame(fFButton, true);
	fFvsp2->AddFrame(vsp2, new TGLayoutHints(kLHintsExpandY|kLHintsExpandX));

	fFRight->AddFrame(fFvsp2, new TGLayoutHints(kLHintsNormal|kLHintsExpandX, 1, 1, 0, 0));

	TGHorizontalFrame *fFvtext = new TGHorizontalFrame(fFRight, 300, 290, kChildFrame|kRaisedFrame|kFixedHeight|kFixedWidth);
	fviewText = new TGTextView(fFvtext, 300, 190);
	fFvtext->AddFrame(fviewText, new TGLayoutHints(kLHintsExpandY|kLHintsExpandX, 2, 2, 6, 2));
	fFRight->AddFrame(fFvtext, new TGLayoutHints(kLHintsExpandY|kLHintsExpandX, 2, 2, 6, 2));

	AddFrame(fFRight, new TGLayoutHints(kLHintsNormal, 1, 1, 0, 0));

	SetWMSizeHints(GetDefaultWidth(), GetDefaultHeight(), GetDefaultWidth(), GetDefaultHeight(), 0, 0);

	SetWindowName("Online-RIBLL2-DAQ");
	MapSubwindows();
	MapWindow();

	fFileCont->SetDefaultHeaders();
	//	fFileCont->DisplayDirectory();
	//	fFileCont->AddFile("..");        // up level directory
	//	fFileCont->Resize();
	fFileCont->StopRefreshTimer();   // stop refreshing
	fFileCont->SetPageDimension(0, 0);
	fFileCont->SetViewMode(kLVDetails);
	fFileCont->SetHeaders(2);
	fFileCont->SetColHeaders("    name    ", "                             title                                ");

	Resize();
	CreateCanvas();

}

OnlineFrame::~OnlineFrame()
{
	if(datafile) delete datafile;
	if(candaq)  delete candaq;
}

void OnlineFrame::CloseWindow()
{
	DeleteWindow();
	gApplication->Terminate(0);
}

bool OnlineFrame::ProcessMessage(Long_t msg, Long_t param1, Long_t)
{
	switch(GET_MSG(msg))
	{
	case kC_COMMAND:
		switch(GET_SUBMSG(msg))
		{
		case kCM_BUTTON:
			switch(param1)
			{
			case kB_exit:
				CloseWindow();
				break;
			}
		}
		break;

	default:
		break;
	}

	//	ShowText(GetDrawOpt());
	//cout << ShowText(GetDrawOpt()) << endl;
	return kTRUE;
}

const char* OnlineFrame::GetDrawOpt()
{
	int num =  fcomDrawOpt->GetSelected();
	switch (num)
	{
	case 0:
		return "";
		break;
	case 1:
		return "COLZ";
		break;
	case 2:
		return "LEGO";
		break;
	case 3:
		return "CONT";
		break;
	case 4:
		return "SURF";
		break;
	default:
		return "";
		break;
	}
}

void OnlineFrame::ShowText(TGText *text)
{
	ClearTextView();
	fviewText->AddText(text);
	fviewText->Update();
	fviewText->ShowBottom();
}

void OnlineFrame::ShowText(const char *text)
{
	ClearTextView();
	fviewText->AddLineFast(text);
	fviewText->Update();
	fviewText->ShowBottom();
}

void OnlineFrame::CreateCanvas()
{
	gStyle->SetTitleFillColor(kWhite);
	gStyle->SetFrameFillStyle(0);
	gStyle->SetFrameFillStyle(1001);
	gStyle->SetFrameFillColor(0);
	gStyle->SetFuncColor(kRed);
	gStyle->SetStatColor(kWhite);
	gStyle->SetStatBorderSize(1);
	gStyle->SetPadBorderMode(0);
	gStyle->SetPadColor(0);
	gStyle->SetTitleBorderSize(1);
	gStyle->SetLegendBorderSize(1);
	gStyle->SetFillColor(1);
	gStyle->SetPalette(1);
	gStyle->SetOptFit(0111);
//	gStyle->SetHistLineWidth(0.2);
        gStyle->SetOptStat("nemruoi");
	if(!(gROOT->GetListOfCanvases()->FindObject("can_daq")))
	{
		candaq = new TCanvas("can_daq", "RIBLL2_ONLINE");
		//if(!candaq->GetShowToolBar())   candaq->ToggleToolBar();
		if(!candaq->GetShowEditor())   candaq->ToggleEditor();
		if(!candaq->GetShowEventStatus()) candaq->ToggleEventStatus();
		candaq->SetBorderMode(0);
		candaq->SetFillColor(0);
		candaq->SetFillStyle(0);
		candaq->SetFrameBorderMode(0);
		((TRootCanvas *)candaq->GetCanvasImp())->DontCallClose();
		//remove some contextmenu items of TCanvas
		MakeTcMenuList();
		RemoveMenuEntry("SetEditable", TCmlist);
		RemoveMenuEntry("DrawClonePad", TCmlist);
		RemoveMenuEntry("DrawClone", TCmlist);
		RemoveMenuEntry("Dump", TCmlist);
		RemoveMenuEntry("SetName", TCmlist);
	}
}

void OnlineFrame::ClearTextView()
{
	if(fviewText->ReturnLineCount()>=50)
	{
		fviewText->GetText()->DelLine(1);
	}
}

void OnlineFrame::OnDoubleClick(TGLVEntry *f, Int_t btn)
{
	if (btn!=kButton1) return;
	gVirtualX->SetCursor(fFileCont->GetId(),gVirtualX->CreateCursor(kWatch));

	TString name(f->GetTitle());
	const char* fname = (const char*)f->GetUserData();

	if (fname) {
		DisplayObject(fname,name);
	} else if (name.EndsWith(".root")) {
		DisplayFile(name);
	} else {
		DisplayDirectory(name);
	}
	gVirtualX->SetCursor(fFileCont->GetId(),gVirtualX->CreateCursor(kPointer));
}

void OnlineFrame::DisplayFile(const TString &fname)
{
	// display content of ROOT file
	if(!datafile) return;
	TFile *file = datafile;

	fFileCont->RemoveAll();
	//	fFileCont->AddFile(gSystem->WorkingDirectory());
	fFileCont->SetPagePosition(0,0);
	//	fFileCont->SetHeaders(2);
	//	fFileCont->SetColHeaders("name", "title");

	//	TIter next(file.GetListOfKeys());
	TIter next(file->GetList());
	TKey *key;

	while ((key=(TKey*)next())) {
		if(!(key->IsA())->InheritsFrom("TH1")) continue;
		TString cname = key->GetTitle();
		TString name = key->GetName();
		TGLVEntry *entry = new TGLVEntry(fFileCont,name,cname);
		entry->SetSubnames(key->GetTitle());
		fFileCont->AddItem(entry);

		// user data is a filename
		entry->SetUserData((void*)StrDup(fname));
	}

	Resize();
}

void OnlineFrame::DisplayObject(const TString& fname,const TString& name)
{
	// browse object located in file

	if(!datafile) return;
	TDirectory *sav = gDirectory;

	static TFile *file = 0;
	file = (TFile *) gROOT->GetListOfFiles()->FindObject(fname.Data());
	if(!file) return;

	TObject* obj = file->Get(name);
	if (obj) {
		if (!obj->IsFolder()) {
			candaq->cd(0);
			DrawObj(obj);
		} else obj->Print();
	}
	gDirectory = sav;
}

void OnlineFrame::DisplayDirectory(const TString &fname)
{
	// display content of directory

	fFileCont->SetDefaultHeaders();
	gSystem->ChangeDirectory(fname);
	fFileCont->ChangeDirectory(fname);
	fFileCont->DisplayDirectory();
	fFileCont->AddFile("..");  // up level directory
	Resize();
}

void OnlineFrame::ImB_update()
{
	if(!candaq) return;
	if(!ObjListOK()) return;
//	int nx=1, ny=1;
//	GetDivPad(nx, ny);
	if(candaq->GetPad(1))
	{
		for(int i=1; i<30; i++)
		{
			if(candaq->GetPad(i))
			{
				candaq->cd(i);
				gPad->Modified();
				gPad->Update();
			}
		}
	}
	else
	{
		gPad->Modified();
		gPad->Update();
	}
}

void OnlineFrame::ImB_next()
{
	DrawObj(kB_next);
	//if(!ObjListOK()) return;
	//do
	//{
	//	DrawObj(ObjList->After(objcurr));
	//}while(!(objcurr->IsA()->InheritsFrom("TH1")));
}

void OnlineFrame::ImB_previous()
{
	DrawObj(kB_privious);
	//if(!ObjListOK()) return;
	//do
	//{
	//	DrawObj(ObjList->Before(objcurr));
	//}while(!(objcurr->IsA()->InheritsFrom("TH1")));
}

bool OnlineFrame::ObjListOK()
{
	if( objcurr && ObjList )
	{
		return true;
	}
	else
	{
		return false;
	}

}

void OnlineFrame::DrawObj(TObject *obj)
{
	if(!obj) return;
	if(!candaq) return;
	obj->Draw(GetDrawOpt());
	candaq->Update();
	objcurr = obj;
}

void OnlineFrame::DrawObj(CMDIdentifiers id)
{
	if(!ObjListOK()) return;
	candaq->Clear();
	int nx=1, ny=1;
	GetDivPad(nx, ny);	
	if(nx*ny>2)
	{
		candaq->Divide(nx, ny);
		//candaq->SetLogy();
	}

	if(id == kB_privious)
	{
		for(int np=nx*ny; np>=1; np--)
		{
			candaq->cd(np);
			//if(nx*ny>2) gPad->SetLogy();
			do
			{
				DrawObj(ObjList->Before(objcurr));
			}while(!(objcurr->IsA()->InheritsFrom("TH1")));

		}

	} else if(id == kB_next)
	{
		for(int np=1; np<=nx*ny; np++)
		{
			candaq->cd(np);
			//if(nx*ny>2) gPad->SetLogy();
			do
			{
				DrawObj(ObjList->After(objcurr));
			}while(!(objcurr->IsA()->InheritsFrom("TH1")));

		}

	}

}

void OnlineFrame::ImB_ResetAllTH()
{
	if(!ObjListOK()) return;
	TIter next(ObjList);
	while(TObject *obj = next())
	{
		ResetTH(obj);
	}
	ImB_update();
}

void OnlineFrame::ImB_ResetCurrTH()
{
	if(!ObjListOK()) return;
	ResetTH(objcurr);
	ImB_update();
}

void OnlineFrame::ResetTH(TObject *obj)
{
	if (obj && (obj->IsA()->InheritsFrom("TH1"))) 
	{
		((TH1 *)obj)->Reset();
	}
}

void OnlineFrame::ImB_integral()
{
	if(!objcurr) return;
	if(!candaq) return;
	if(candaq->GetPad(1)) return;
	TString cname = objcurr->IsA()->GetName();
	if(!cname.Contains("TH1F", TString::kIgnoreCase)) return;
	ShowText("Select the left bin with mouse:");
	TMarker *p1 = (TMarker *)gPad->WaitPrimitive("TMarker");
	float left = 0.;
	if (p1)
	{
		p1->SetMarkerStyle(3);
		p1->SetMarkerColor(2);
		p1->SetMarkerSize(2.5);
		p1->Draw();
		left = p1->GetX();
	}

	TString text_xl = "X1= ";
	text_xl += left;
	ShowText(text_xl.Data());
	delete p1;
	ShowText("Select the right bin with mouse:");
	TMarker *p2 = (TMarker *)gPad->WaitPrimitive("TMarker");
	float right = 0.;
	if (p2)
	{
		p2->SetMarkerStyle(3);
		p2->SetMarkerColor(2);
		p2->SetMarkerSize(2.5);
		p2->Draw();	
		right = p2->GetX();
	}	
	
	TString text_xr = "X2= ";
	text_xr += right;
	ShowText(text_xr.Data());
	p2->Delete();
	TAxis *xaxis = ((TH1*)objcurr)->GetXaxis();
	if(left>right) 
	{
		float xxt = left;
		left = right;
		right = xxt;
	}
	int xlbin = xaxis->FindBin(left);
	int xrbin = xaxis->FindBin(right);

	double numb = ((TH1*)objcurr)->Integral(xlbin, xrbin);
	float per=0.0;
	if(((TH1*)objcurr)->GetEntries()>0)
	{
		per = numb*100./(((TH1*)objcurr)->GetEntries());
	}
	TString text_inte = "Integral: ";
	text_inte += numb;
	ShowText(text_inte);
	TString text_per = "Percent: ";
	//TString data_per = "p";
	char s[32];
	sprintf(s, "%.5f", per);
	TString data_per = s;
	TString data_per1 = data_per(0, 4);
	text_per += data_per1;
	text_per += "%";
	ShowText(text_per.Data());

	TString stime = "Integral time: ";
	time_t nowtime;
	time(&nowtime);
	struct tm * timeinfo;
	timeinfo = localtime(&nowtime);
	TString ltime = asctime(timeinfo);
	stime += ltime(11, 8);
	ShowText(stime.Data());	
	ShowText("   ");
	

//	p1->Delete();
//	p2->Delete();
}

void OnlineFrame::GetDivPad(int &nx, int &ny)
{
	nx = 1;
	ny = 1;
	int n = fcomDivPad->GetSelected();
	switch(n)
	{
	case 0:
		nx = 1;
		ny = 1;
		break;
	case 1:
		nx = 2;
		ny = 2;
		break;
	case 2:
		nx = 2;
		ny = 3;
		break;
	case 3:
		nx = 3;
		ny = 3;
		break;
	case 4:
		nx = 4;
		ny = 4;
		break;
	default:
		break;		
	}
}

void OnlineFrame::RemoveMenuEntry(const char *menuTitle, TList *mlist)
{
	TClassMenuItem *menuItem = 0;
	TString itemTitle;

	if(!mlist) return;
	TIter next(mlist);
	while( menuItem = (TClassMenuItem *)next() )
	{
		itemTitle = menuItem->GetTitle();
		if( itemTitle == menuTitle)
		{
			delete menuItem;
			break;
		}
	}
}

void OnlineFrame::MakeTcMenuList()
{
	TClass *c_TCanvas = gROOT->GetClass("TCanvas");
	if(!c_TCanvas) return;
	
	c_TCanvas->MakeCustomMenuList();
	TCmlist = c_TCanvas->GetMenuList();
}

void OnlineFrame::MakeTH1MenuList()
{
	TClass *c_TH1 = gROOT->GetClass("TH1F");
	if(!c_TH1) return;
	
	c_TH1->MakeCustomMenuList();
	THmlist = c_TH1->GetMenuList();
}

void OnlineFrame::MakeTH2MenuList()
{
	TClass *c_TH2 = gROOT->GetClass("TH2F");
	if(!c_TH2) return;
	
	c_TH2->MakeCustomMenuList();
	TH2mlist = c_TH2->GetMenuList();
}

void HBOOK1(int id, const char *title, int nxbin, float xlow, float xup, float vmx)
{
	if(!(gOnlineFrame->datafile)) return;
	TString hname = "h";
	hname += id;
	new TH1F(hname.Data(), title, nxbin, xlow, xup);
}

void HBOOK2(int id, const char *title, int nxbin, float xlow, float xup, int nybin, float ylow, float yup, float vmx)
{
	if(!(gOnlineFrame->datafile)) return;
	TString hname = "h";
	hname += id;
	new TH2F(hname.Data(), title, nxbin, xlow, xup, nybin, ylow, yup);
}

void HF1(int id, float x, float weight)
{
	if(!(gOnlineFrame->datafile)) return;
	TString name = "h";
	name += id;
	if(gOnlineFrame->datafile->Get(name.Data()))
	((TH1F *)(gOnlineFrame->datafile->Get(name.Data())))->Fill(x, weight);
	
}

void HF2(int id, float x, float y, float weight)
{
	if(!(gOnlineFrame->datafile)) return;
	TString name = "h";
	name += id;
	if(gOnlineFrame->datafile->Get(name.Data()))
	((TH2F *)(gOnlineFrame->datafile->Get(name.Data())))->Fill(x, y, weight);
}

bool HEXIST(int ihist)
{
	if(!(gOnlineFrame->datafile)) return false;
	TString name = "h";
	name += ihist;
	if(gOnlineFrame->datafile->Get(name.Data())) return true;
	return false;
}
