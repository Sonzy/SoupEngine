//#include "WindowsMacroUndefs.h"
#include "Application.h"


int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	try
	{
		//Creates an application and initialises it. (Runs loop inside the app)
		return Application{}.InitGameLoop();
	}
	//Try catch soup exception
	catch (const SoupException& e)
	{
		//nullptr can be set to window handle to force interacting with mb first
		MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	//Otherwise throw generic error
	catch (...)
	{
		MessageBox(nullptr, "No details available.", "Unknown exception.", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}