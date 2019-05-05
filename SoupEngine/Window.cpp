#include "Window.h"
#include "resource.h"
#include <sstream>

//Init static var
Window::WindowClass Window::WindowClass::windowClass;

Window::Window(int width, int height, const char * name)
{
	//Create rect for window
	RECT windowRegion;
	windowRegion.left = 100;
	windowRegion.right = width + windowRegion.left;
	windowRegion.top = 100;
	windowRegion.bottom = height + windowRegion.top;

	//Adjust for the topbar etc
	AdjustWindowRect(&windowRegion, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
	
	//Create the window										/* Window Style */						/* Where to spawn the window */
	hWnd = CreateWindowEx(0, WindowClass::GetName(), name, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT,
		width, height, nullptr, nullptr, WindowClass::GetInstance(), this);

	//Show the window
	ShowWindow(hWnd, SW_SHOW);

	//Exception example
	//throw WIND_EXCEPT(ERROR_ARENA_TRASHED);
}

Window::~Window()
{
	DestroyWindow(hWnd);
}

LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//Get the pointer to the instance
	if (msg == WM_NCCREATE)
	{
		//Get window pointer from creation data
		const CREATESTRUCTW* const creationStruct = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const window = static_cast<Window*>(creationStruct->lpCreateParams);

		//Store pointer to window class in the WinApi user data
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
		//Reset the windows message handling to the standard mode
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));

		return window->HandleMessages(hWnd, msg, wParam, lParam);
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//Get the pointer to the window class
	 Window* const window = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	//Pass the message to the handler inside the class
	return window->HandleMessages(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMessages(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0); //Sends a quit message with exit code param
		return 0;
	case WM_KEYDOWN: //Not Case Sensitive
		keyboard.OnKeyPressed(static_cast<unsigned char>(wParam));
		break;
	case WM_KEYUP:
		keyboard.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR: // Case sensitive
		keyboard.OnChar(static_cast<unsigned char>(wParam));
		break;
	case WM_LBUTTONDOWN:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//============================================================================
// Window class stuff
//============================================================================

const char * Window::WindowClass::GetName() noexcept
{
	return className;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return windowClass.hInstance;
}

Window::WindowClass::WindowClass() noexcept
	: hInstance(GetModuleHandle(nullptr))
{
	//Register the window class (Documentation: https://docs.microsoft.com/en-us/windows/desktop/api/winuser/ns-winuser-tagwndclassexa)
	WNDCLASSEX windowsClass = { 0 };
	windowsClass.cbSize = sizeof(windowsClass); //Assign the size of this windows class to the struct
	windowsClass.style = CS_OWNDC; //Tell window to not draw the background, will be done by me
	windowsClass.lpfnWndProc = HandleMsgSetup; //Link to the function that will be handling windows messages
	windowsClass.cbClsExtra = 0; //Dont allocate extra bytes for stoof
	windowsClass.cbWndExtra = 0; //Dont allocate extra bytes for stoof
	windowsClass.hInstance = GetInstance(); //Pass the hinstance
	windowsClass.hIcon = static_cast<HICON>(LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0)); //Used to set up custom icon
	windowsClass.hIconSm = static_cast<HICON>(LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));; //Same as before but small one
	windowsClass.lpszClassName = className; //Set class name

	//RegisterClass() - Is old version, use RegisterClassEx()
	RegisterClassEx(&windowsClass);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(className, GetInstance());
}

//============================================================================
// Window Exception
//============================================================================

Window::Exception::Exception(int line, const char * file, HRESULT hr)
	: SoupException(line, file), hr(hr)
{
}

const char* Window::Exception::what() const noexcept
{
	std::ostringstream stringStream;

	stringStream << GetType() << std::endl
		<< "Error Code: " << GetErrorCode() << std::endl
		<< "Description: " << GetErrorString() << std::endl
		<< GetOriginString();

	whatBuffer = stringStream.str();
	return whatBuffer.c_str();
}

const char * Window::Exception::GetType() const noexcept
{
	return "Soup Window Exception";
}

std::string Window::Exception::TranslateErrorCode(HRESULT hr) noexcept
{
	//Create a new memory buffer and put the error code data into it?
	char* messageBuffer = nullptr;

	//Format the error message
	DWORD messageLength = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&messageBuffer), 0, nullptr );

	if (messageLength == 0)
		return "Unknown error code.";

	//Free the memory buffer
	std::string errorString = messageBuffer;
	LocalFree(messageBuffer);

	return errorString;
}

HRESULT Window::Exception::GetErrorCode() const noexcept
{
	return hr;
}

std::string Window::Exception::GetErrorString() const noexcept
{
	return TranslateErrorCode(hr);
}
