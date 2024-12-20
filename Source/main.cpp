#include <ctime>

#include "Core/Application/Application.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance,
				   _In_opt_ HINSTANCE hPrevInstance,
				   _In_ LPSTR lpCmdLine,
				   _In_ int nCmdShow)
{
	// INFO: Seed the random number generator
	srand(static_cast<unsigned int>(time(nullptr)));

	// INFO: Create the application
	Application app(hInstance, nCmdShow, WindowInfo(L"Window", 800, 600));

	// INFO: Run the application
	app.Run();

	return 0;
}