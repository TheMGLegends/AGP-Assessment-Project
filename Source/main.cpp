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
		// INFO: Break if the application should quit
		if (Window::ProcessMessages() == -1)
			break;
	}

	return 0;
}