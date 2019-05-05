//#include "WindowsMacroUndefs.h"
#include <Windows.h>
#include "Window.h"
#include <string>


int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	try {
		std::string className = "Main Window Class";
		std::string windowName = "Soup Engine";

		Window window(1024, 768, windowName.c_str());

		// Window loop - Check windows messages
		MSG message;
		BOOL result;
		while ((result = GetMessage(&message, nullptr, 0, 0)) > 0) //nullptr - We want to receive messages from all windows for now
		{
			TranslateMessage(&message); //Checks any specific messages that need to send extra ones e.g WM_KEYDOWN on letters will also post a WM_CHAR
			DispatchMessage(&message);

			if (window.keyboard.IsKeyPressed(VK_SPACE))
				MessageBox(nullptr, "We pressed space", "Space pressed", 0);
		}

		//Return the correct exit code
		if (result == -1)
			return -1;

		return message.wParam;
	}
	catch (const SoupException& e)
	{
		//nullptr can be set to window handle to force interacting with mb first
		MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBox(nullptr, "No details available.", "Unknown exception.", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}