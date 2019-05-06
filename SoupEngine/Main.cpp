//#include "WindowsMacroUndefs.h"
#include <Windows.h>
#include "Window.h"
#include <string>
#include <sstream>


int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	try {
		//Create our window
		Window window(1024, 768, "Soup Engine");

		// Window loop - Check windows messages
		MSG message; BOOL result;
		while ((result = GetMessage(&message, nullptr, 0, 0)) > 0) //nullptr - We want to receive messages from all windows for now
		{
			TranslateMessage(&message); //Checks any specific messages that need to send extra ones e.g WM_KEYDOWN on letters will also post a WM_CHAR
			DispatchMessage(&message);
		}

		//Throw exception if exiting with failed error code
		if (result == -1)
			throw WIND_LAST_EXCEPT();

		return message.wParam;
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