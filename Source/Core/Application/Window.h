#pragma once

#include <functional>
#include <Windows.h>

struct WindowInfo
{
public:
	WindowInfo() : windowName(L"Window"), width(800), height(600) {}

	WindowInfo(LPCWSTR _windowName, int _width, int _height)
		: windowName(_windowName), width(_width), height(_height) {}

public:
	LPCWSTR windowName;
	int width;
	int height;
};

class Window
{
public:
	Window();
	~Window();

	HRESULT Initialise(HINSTANCE _hInstance, int nCmdShow, const WindowInfo& _windowInfo);

	static LRESULT CALLBACK HandleMessageSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMessageRepeated(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	/// @retval 0 : If all messages were processed
	/// @retval -1 : If the application should quit
	static int ProcessMessages();

	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	inline HWND GetWindowHandle() const { return hWnd; }
	inline void SetOnQuit(std::function<void()> _OnQuit) { OnQuit = _OnQuit; }

private:
	void AllocateConsole();
	void Quit();

private:
	HINSTANCE hInstance;
	HWND hWnd;

	WindowInfo windowInfo;

	std::function<void()> OnQuit;
};

