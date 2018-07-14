#include "TROOT.h"
#include "TH1.h"
#include "TFile.h"
#include "TF1.h"
#include "TList.h"
#include "OnlineFrame.h"

void UserFunc(OnlineFrame *onl)
{
/*
	TF1 *func1 = new TF1("func1","sqrt(216-x*x-6*x)-5" , -18, 12);
	TF1 *func2 = new TF1("func1","-sqrt(216-x*x-6*x)-5", -18, 12);

	if( TH1 *th = (TH1*)(onl->datafile->Get("h292")) )
	{
		TList *thfunclist = th->GetListOfFunctions();
		if(thfunclist)
		{
			thfunclist->Add(func1);
			thfunclist->Add(func2);
		}
	}


	if( TH1 *th = (TH1*)(onl->datafile->Get("h308")) )
	{
		TList *thfunclist = th->GetListOfFunctions();
		if(thfunclist)
		{
			thfunclist->Add(func1);
			thfunclist->Add(func2);
		}
	}


	TF1 *func3 = new TF1("func1","sqrt(625-x*x)" , -25, 25);
	TF1 *func4 = new TF1("func1","-sqrt(625-x*x)", -25, 25);

	if( TH1 *th = (TH1*)(onl->datafile->Get("h313")) )
	{
		TList *thfunclist = th->GetListOfFunctions();
		if(thfunclist)
		{
			thfunclist->Add(func3);
			thfunclist->Add(func4);
		}
	}
*/
}
