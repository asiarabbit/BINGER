#include "MyMainFrame.h"
void graph3();
MyMainFrame::MyMainFrame(const TGWindow *p, int w, int h){
	// Create a main frame
	// Object fMain would be the only communication channel
	// to the system screen. Anything to be shown on screen
	// has to be added by fMain. This is the concept.
	// Here p stands for parent, which is what fMain belongs to.
	fMain = new TGMainFrame(p, w, h);
	fMain->Connect("CloseWindow()", "MyMainFrame", this, "DoClose()");

	// Create canvas widget.
	// The unit of the size of the window is pixel.
	fEcanvas  = new TRootEmbeddedCanvas("Ecanvas", fMain, 800, 600);
	TGLayoutHints *canvasLayout = new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 10, 10, 10, 1);
	fMain->AddFrame(fEcanvas, canvasLayout);

	// Create a horizontal frame widget with buttons
	TGHorizontalFrame *hframe = new TGHorizontalFrame(fMain, 800, 40);
	TGTextButton *draw = new TGTextButton(hframe, "&Draw");
	draw->Connect("Clicked()", "MyMainFrame", this, "DoDraw()");
	TGLayoutHints *drawLayout = new TGLayoutHints(kLHintsCenterX, 5, 5, 3, 4);
	hframe->AddFrame(draw, drawLayout);

	TGTextButton *exit = new TGTextButton(hframe, "&Exit", "gApplication->Terminate(0)");
	TGLayoutHints *exitLayout = new TGLayoutHints(kLHintsCenterX, 5, 5, 3, 4);
	hframe->AddFrame(exit, exitLayout);

	
	TGLayoutHints *hframeLayout = new TGLayoutHints(kLHintsCenterX, 2, 2, 2, 2);
	fMain->AddFrame(hframe, hframeLayout);

	// Set a name to the main frame
	fMain->SetWindowName("Simple Example");


	// *** The next three lines make the widgets visible *** //
	// Map all subwindows of main frame
	fMain->MapSubwindows(); // make all the children of fMain appear on itself.
	// Initialize the layout algorithm
	fMain->Resize(fMain->GetDefaultSize()); // to execute all layout specifications for the widgets before the top level window itself is shown on the screen.
	// Map main frame
	fMain->MapWindow(); // make the main frame itself appear on the screen.

} // end of the constructor.

void MyMainFrame::DoDraw(){
	// Draws function graphics in randomly chosen interval
//	TF1 *f1 = new TF1("f1", "sin(x)/x", 0, gRandom->Rndm() * 10);
//	f1->SetLineWidth(3);
//	f1->Draw();
	graph3();
	TCanvas *fCanvas = fEcanvas->GetCanvas();
	fCanvas->cd();
	fCanvas->Update();
} // end member function draw
void MyMainFrame::DoClose(){
	exit(1);
} // end of member function DoClose()
MyMainFrame::~MyMainFrame(){
	fMain->Cleanup();
	delete fMain;
} // end of the destructor


void graph3(){
	TGraph2D *g = new TGraph2D();
	g->SetNameTitle("g", "A 3-D Line");
	g->SetFillColor(29);
	g->SetMarkerSize(0.8);
	g->SetMarkerStyle(20);
	g->SetMarkerColor(kRed);
	g->SetLineColor(kBlue-3);
	g->SetLineWidth(5);
	g->SetLineStyle(1);
//	gPad->SetLogy(1);
	
	TGraph2D *g2 = (TGraph2D*)g->Clone();
	g2->SetLineColor(kCyan);
	
	for(int i = 0; i < 10; i++){
		g->SetPoint(i, i, i, i);
		g2->SetPoint(i, i, 2 * i, 3 * i);		
	} // end for over i

	g2->Draw("P0LINE");
	g->Draw("P0LINEsame");
}

