// example.C -- gui written for Detector Project
#ifndef __MY_MAIN_FRAME_
#define __MY_MAIN_FRAME_
#include <TGClient.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TGraph2D.h>
#include <TRandom.h>
#include <TGButton.h>
#include <TGFrame.h>
#include <TRootEmbeddedCanvas.h>
#include <RQ_OBJECT.h>
#include <TQObject.h>

class MyMainFrame{
	RQ_OBJECT("MyMainFrame")
public:
	MyMainFrame(const TGWindow *p, int w, int h);
	virtual ~MyMainFrame();
	void DoDraw();
	void DoClose();

private:
	TGMainFrame *fMain;
	TRootEmbeddedCanvas *fEcanvas;
};

#endif
