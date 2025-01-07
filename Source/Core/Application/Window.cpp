#include "Window.h"

#include <iostream>

#include "../Input/InputHandler.h"
#include "../../Utilities/Debugging/DebugUtils.h"
#include "../../../resource.h"

using namespace DebugUtils;
using namespace DirectX;

Window::Window() : hInstance(nullptr), hWnd(nullptr), windowInfo(), OnQuit(nullptr)
{
	AllocateConsole();
}

Window::~Window()
{
	DestroyWindow(hWnd);
}

HRESULT Window::Initialise(HINSTANCE _hInstance, int nCmdShow, const WindowInfo& _windowInfo)
{
	hInstance = _hInstance;
	windowInfo = _windowInfo;

	// INFO: Create the window class
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = HandleMessageSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = static_cast<HICON>(LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 256, 256, 0));
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = L"WindowClass";
	wc.hIconSm = static_cast<HICON>(LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));

	// INFO: Register the window class
	if (!RegisterClassEx(&wc))
	{
		LogError("Window::Initialise(): Failed to register window class!");
		return E_FAIL;
	}

	// INFO: Adjust the window size
	RECT rc = { 0, 0, windowInfo.width, windowInfo.height };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	// INFO: Get the screen width and height
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// INFO: Calculate the position to center the window on the screen
	int windowWidth = rc.right - rc.left;
	int windowHeight = rc.bottom - rc.top;
	int xPos = std::max<int>(0, (screenWidth - windowWidth) / 2); // NOTE: Prevent negative window position
	int yPos = std::max<int>(0, (screenHeight - windowHeight) / 2); // NOTE: Prevent negative window position

	// INFO: Create the window
	hWnd = CreateWindowEx(
		NULL,
		L"WindowClass",
		windowInfo.windowName,
		WS_OVERLAPPEDWINDOW,
		xPos, yPos,
		windowWidth, windowHeight,
		NULL,
		NULL,
		hInstance,
		this
	);

	if (!hWnd)
	{
		LogError("Window::Initialise(): Failed to create window!");
		return E_FAIL;
	}

	// INFO: Initialise input handler
	if (!InputHandler::Initialise(hWnd))
	{
		LogError("Window::Initialise(): Failed to initialise input handler!");
		return E_FAIL;
	}

	// INFO: Show the window
	ShowWindow(hWnd, nCmdShow);

	return S_OK;
}

LRESULT Window::HandleMessageSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// INFO: Upon initial window creation, associate the window with this class instance
	if (uMsg == WM_NCCREATE)
	{
		// INFO: Extract the pointer to the class instance from the window creation data
		CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		Window* pWindow = reinterpret_cast<Window*>(pCreate->lpCreateParams);

		// INFO: Store the pointer in the window instance
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));

		// INFO: Set the message procedure to handle message repeated
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMessageRepeated));

		// INFO: Forward the message to the user-defined message handler
		return pWindow->HandleMessage(hWnd, uMsg, wParam, lParam);
	}

	// INFO: Default Message Handling for the Window
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT Window::HandleMessageRepeated(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// INFO: Retrieve the pointer to the class instance from the window instance
	Window* pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	// INFO: Forward the message to the user-defined message handler
	return pWindow->HandleMessage(hWnd, uMsg, wParam, lParam);
}

int Window::ProcessMessages()
{
	MSG msg = { 0 };

	// INFO: Process messages until there are no more
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
			return -1;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT Window::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_ACTIVATEAPP:
	{
		Keyboard::ProcessMessage(uMsg, wParam, lParam);
		Mouse::ProcessMessage(uMsg, wParam, lParam);
		break;
	}
	case WM_DESTROY:
	{
		Quit();
		break;
	}
	case WM_KEYDOWN:
	{
		Keyboard::ProcessMessage(uMsg, wParam, lParam);
		switch (wParam)
		{
		case VK_ESCAPE:
			Quit();
			break;
		case VK_LWIN:
		case VK_RWIN:
			InputHandler::SetMouseMode(Mouse::Mode::MODE_ABSOLUTE);
			break;
		}
		break;
	}
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	{
		Keyboard::ProcessMessage(uMsg, wParam, lParam);
		break;
	}
	case WM_ACTIVATE:
	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
	{
		Mouse::ProcessMessage(uMsg, wParam, lParam);
		break;
	}
	case WM_MOUSEACTIVATE:
	{
		return MA_ACTIVATEANDEAT;
	}
	case WM_SIZE:
	{
		windowInfo.width = LOWORD(lParam);
		windowInfo.height = HIWORD(lParam);

		NotifyWindowSizeChanged(windowInfo.width, windowInfo.height);
		break;
	}
	default:
	{
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	}

	return 0;
}

void Window::AllocateConsole()
{
#if defined(_DEBUG)
	if (AllocConsole())
	{
		// INFO: Redirect the standard input, output and error streams to our console
		FILE* fp = nullptr;
		freopen_s(&fp, "CONIN$", "r", stdin);
		freopen_s(&fp, "CONOUT$", "w", stdout);
		freopen_s(&fp, "CONOUT$", "w", stderr);
		std::ios::sync_with_stdio(true);

		Log("Window::AllocateConsole(): Console Opened!");
	}
#endif
}

void Window::Quit()
{
	if (OnQuit)
	{
		// INFO: Quits the application using the user-defined callback if it exists
		OnQuit();
	}
	else
	{
		// INFO: Quits the application using the default callback
		PostQuitMessage(0);
	}
}
