#include "BrowserWindow.h"
#include <iostream>

using namespace System;
using namespace System::Windows::Forms;

[STAThread]

int main()
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	MyBrowser::BrowserWindow mainWindow;
	Application::Run(%mainWindow);
}
void MyBrowser::BrowserWindow::MarshalString(String ^ s, std::string & os)
{
	using namespace Runtime::InteropServices;
	const char* chars = (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
	os = chars;
	Marshal::FreeHGlobal(IntPtr((void*)chars));

}