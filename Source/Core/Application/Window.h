#pragma once

#include "../../Observer/Subject.h"

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

class Window : public Subject
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
	inline float GetAspectRatio() const { return static_cast<float>(windowInfo.width) / static_cast<float>(windowInfo.height); }

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

