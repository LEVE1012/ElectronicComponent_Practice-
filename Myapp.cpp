
#include "MyApp.h"
#include "Main.h"

wxIMPLEMENT_APP(MyApp);

MyApp::MyApp() {

}

MyApp::~MyApp() {


}

bool MyApp::OnInit()
{
	Main* frame = new Main();
	
	frame->Show(true);
	return true;
}
