#include "TApplication.h"
#include "MyMainFrame.h"

int main(int argc, char **argv){
	TApplication theApp("App", &argc, argv);
	MyMainFrame *show = new MyMainFrame(gClient->GetRoot(), 800, 800);
	theApp.Run();

	return 0;
}
