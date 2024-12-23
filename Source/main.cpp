#include <ctime>

#include "Core/Application/Application.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance,
				   _In_opt_ HINSTANCE hPrevInstance,
				   _In_ LPSTR lpCmdLine,
				   _In_ int nCmdShow)
{
	// INFO: Memory Leak Detection Flags
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// INFO: Create the application
	Application app(hInstance, nCmdShow, WindowInfo(L"AGP Assessment Project", 800, 600));

	// INFO: Run the application
	app.Run();

	return 0;
}