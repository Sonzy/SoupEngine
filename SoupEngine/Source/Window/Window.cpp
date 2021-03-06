#include "Window.h"
#include "resource.h"
#include <sstream>
#include "ImGui/imgui_impl_win32.h"

//Init static var
Window::WindowClass Window::WindowClass::windowClass;

Window::Window(int width, int height, const char * name)
	: width(width), height(height)
{
	//Create rect for window
	RECT windowRegion;
	windowRegion.left = 100;
	windowRegion.right = width + windowRegion.left;
	windowRegion.top = 100;
	windowRegion.bottom = height + windowRegion.top;

	//Adjust for the topbar etc
	if (AdjustWindowRect(&windowRegion, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
		throw WIND_LAST_EXCEPT();
										/* Window Style */						/* Where to spawn the window */
	hWnd = CreateWindowEx(0, WindowClass::GetName(), name, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT,
		windowRegion.right - windowRegion.left, windowRegion.bottom - windowRegion.top, nullptr, nullptr, WindowClass::GetInstance(), this);

	ShowWindow(hWnd, SW_SHOW);
	//Initialise ImGui
	ImGui_ImplWin32_Init(hWnd);
	//Create graphics
	gfx = std::make_unique<Graphics>(hWnd, width, height);


	//Register our raw input devices
	RAWINPUTDEVICE rid;
	rid.usUsagePage = 0x01;
	rid.usUsage = 0x02;
	rid.dwFlags = 0;
	rid.hwndTarget = nullptr;

	if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE) //Tries to get the first raw input device
		throw WIND_LAST_EXCEPT();
}

Window::~Window()
{
	ImGui_ImplWin32_Shutdown();
	DestroyWindow(hWnd);
}

std::optional<int> Window::ProcessMessages()
{
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
			return (int)msg.wParam;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return {};
}

void Window::SetWindowTitle(const std::string newTitle)
{
	if (SetWindowText(hWnd, newTitle.c_str()) == 0)
		throw WIND_LAST_EXCEPT();
}

Graphics & Window::GetGraphics()
{
	return *gfx;
}

int Window::GetWindowWidth()
{
	return width;
}

int Window::GetWindowHeight()
{
	return height;
}

void Window::EnableCursor()
{
	cursorEnabled = true;
	ShowCursor();
	EnableImGuiMouse(true);
	ConfineCursor(false);
}

void Window::DisableCursor()
{
	cursorEnabled = false;
	HideCursor();
	EnableImGuiMouse(false);
	ConfineCursor(true);
}

bool Window::CursorEnabled()
{
	return cursorEnabled;
}

void Window::HideCursor()
{
	while (::ShowCursor(FALSE) >= 0);
}

void Window::ShowCursor()
{
	while (::ShowCursor(TRUE) < 0);
}

void Window::EnableImGuiMouse(bool enable)
{
	if (enable)
	{
		ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
	}
	else
	{
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
	}
}

void Window::ConfineCursor(bool confine)
{
	if (confine)
	{
		RECT rect;
		GetClientRect(hWnd, &rect);
		MapWindowPoints(hWnd, nullptr, reinterpret_cast<POINT*>(&rect), 2);
		ClipCursor(&rect);
	}
	else
		ClipCursor(nullptr);
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
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	const auto guiIO = ImGui::GetIO();

	switch (msg)
	{
	//Window Messages================================
	case WM_CLOSE:
		PostQuitMessage(0); //Sends a quit message with exit code param
		return 0;
	case WM_KILLFOCUS: // When we lose focus, reset the keyboard state
		keyboard.ClearState();
	case WM_ACTIVATE:

		//if (cursorEnabled)
		//	break;
		//
		//if (wParam & WA_ACTIVE || wParam & WA_CLICKACTIVE)
		//{
		//	ConfineCursor(true);
		//	HideCursor();
		//}
		//else
		//{
		//	ConfineCursor(false);
		//	ShowCursor();
		//}

		break;
	//Key Messages===================================
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN: //Not Case Sensitive

		//If the GUI wants to capture the keyboard, let it over the app
		if (guiIO.WantCaptureKeyboard)
			break;

		if(!(lParam & 0x40000000) || keyboard.IsAutorepeatEnabled())
			keyboard.OnKeyPressed(static_cast<unsigned char>(wParam));
		break;
	case WM_SYSKEYUP:
	case WM_KEYUP:

		//If the GUI wants to capture the keyboard, let it over the app
		if (guiIO.WantCaptureKeyboard)
			break;

		keyboard.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR: // Case sensitive

		 //If the GUI wants to capture the keyboard, let it over the app
		if (guiIO.WantCaptureKeyboard)
			break;

		keyboard.OnChar(static_cast<unsigned char>(wParam));
		break;
	//Mouse Messages==================================
	case WM_MOUSEMOVE:
	{
		//If the GUI wants to capture the keyboard, let it over the app
		if (guiIO.WantCaptureMouse)
			break;

		const POINTS points = MAKEPOINTS(lParam);
		//Check if we are in the window
		if (points.x >= 0 && points.x < width && points.y >= 0 && points.y < height)
		{
			mouse.OnMouseMove(points.x, points.y);

			//Check if we have just re-entered the window
			if (!mouse.IsInWindow())
			{
				SetCapture(hWnd);
				mouse.OnMouseEnter();
			}
		}
		//If we arent in the region, keep capturing mouse if mouse key is pressed
		else
		{
			if (mouse.IsLeftPressed() || mouse.IsRightPressed())
				mouse.OnMouseMove(points.x, points.y);
			else
			{
				//release capture on not click
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		}
		break;
	}		
	case WM_LBUTTONDOWN:
	{
		if (!cursorEnabled)
		{
			ConfineCursor(true);
			HideCursor();
		}

		const POINTS points = MAKEPOINTS(lParam);
		mouse.OnLeftPressed(points.x, points.y);
		break;
	}	
	case WM_LBUTTONUP:
	{
		const POINTS points = MAKEPOINTS(lParam);
		mouse.OnLeftReleased(points.x, points.y);
		break;
	}	
	case WM_RBUTTONDOWN:
	{
		const POINTS points = MAKEPOINTS(lParam);
		mouse.OnRightPressed(points.x, points.y);
		break;
	}
	case WM_MBUTTONDOWN:
	{
		const POINTS points = MAKEPOINTS(lParam);
		mouse.OnMiddlePressed(points.x, points.y);
		break;
	}
	case WM_MBUTTONUP:
	{
		const POINTS points = MAKEPOINTS(lParam);
		mouse.OnMiddleReleased(points.x, points.y);
		break;
	}
	case WM_RBUTTONUP:
	{
		const POINTS points = MAKEPOINTS(lParam);
		mouse.OnRightReleased(points.x, points.y);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		const POINTS points = MAKEPOINTS(lParam);
		mouse.OnWheelDelta(points.x, points.y, GET_WHEEL_DELTA_WPARAM(wParam));
		break;
	}
	//= Raw Mouse Messages ==========================

	case WM_INPUT:

		if (!mouse.rawEnabled)
			break;

		UINT size = 0;
		//Get the size of the input data
		if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER)) == -1)
			break;

		rawInputBuffer.resize(size);

		//Read the input data
		if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawInputBuffer.data(), &size, sizeof(RAWINPUTHEADER)) != size)
			break;

		//Process the input data
		auto& ri = reinterpret_cast<const RAWINPUT&>(*rawInputBuffer.data());
		if (ri.header.dwType == RIM_TYPEMOUSE &&
			(ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY != 0)) //Read input from mouse devices that have movement input
		{
			mouse.OnRawDelta( ri.data.mouse.lLastX, ri.data.mouse.lLastY);
		}

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
	windowsClass.hIcon = nullptr; // static_cast<HICON>(LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0)); //Used to set up custom icon
	windowsClass.hIconSm = nullptr; //static_cast<HICON>(LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));; //Same as before but small one
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

const char* Window::NoGfxException::GetType() const noexcept
{
	return "Soup - No Gfx Exception";
}
