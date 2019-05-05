#pragma once
#include "WindowsMacroUndefs.h"
#include "SoupException.h"
#include "Keyboard.h"

// Macro to quickly throw exception
#define WIND_EXCEPT(hr) Window::Exception(__LINE__, __FILE__, hr)
#define WIND_LAST_EXCEPT() Window::Exception(__LINE__, __FILE__, GetLastError())

/* Custom window class. Creates a standard window with standard formatting */
class Window
{

public:
	class Exception : public SoupException
	{
	public:
		Exception(int line, const char* file, HRESULT hr);
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept override;
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
	private:
		HRESULT hr;
	};

private:
	class WindowClass
	{
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete; //Disallow initialising another one with a reference to a previous
		WindowClass& operator=(const WindowClass&) = delete; //Disallow comparing
		static constexpr const char* className = "Soup Window";

		// Static reference to the window class
		static WindowClass windowClass;

		// Handle to the instance
		HINSTANCE hInstance;
	};

public:
	Window(int width, int height, const char* name);
	Window(const Window& other) = delete;
	Window& operator=(const Window& other) = delete;
	~Window();

	Keyboard keyboard;

protected:
	WNDCLASSEX windowsClass;
	HWND windowHandle;

	int width;
	int height;
	HWND hWnd;

private:
	//I nitially handles the messages to store a pointer to the window
	static LRESULT WINAPI HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	// Passes messages to HandleMessages()
	static LRESULT WINAPI HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	// Handles all of the windows procedure messages
	LRESULT HandleMessages(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};