#include <iostream>

#include "Core/Application/Window.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance,
				   _In_opt_ HINSTANCE hPrevInstance,
				   _In_ LPSTR lpCmdLine,
				   _In_ int nCmdShow)
{
	// TESTING CODE
	Window window;

	window.Initialise(hInstance, nCmdShow, WindowInfo(L"Window", 800, 600));

	while (true)
	{
		Window::ProcessMessages();
	}

	return 0;
}