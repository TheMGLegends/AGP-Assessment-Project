#include <Windows.h>
#include <iostream>

#include "Core/Time/Time.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance,
				   _In_opt_ HINSTANCE hPrevInstance,
				   _In_ LPSTR lpCmdLine,
				   _In_ int nCmdShow)
{
	// TESTING CODE
	if (AllocConsole())
	{
		// INFO: Redirect the standard input, output and error streams to our console
		FILE* fp = nullptr;
		freopen_s(&fp, "CONIN$", "r", stdin);
		freopen_s(&fp, "CONOUT$", "w", stdout);
		freopen_s(&fp, "CONOUT$", "w", stderr);
		std::ios::sync_with_stdio(true);

		std::cout << "Console Opened!" << std::endl;
	}

	Time::SetTimeScale(1.0f);

	std::cout << "Time Scale: " << Time::GetTimeScale() << std::endl;

	while (true)
	{
		Time::Tick();
		
		std::cout << "Delta Time: " << Time::GetDeltaTime() << std::endl;
		//std::cout << "Elapsed Time: " << Time::GetElapsedTime() << std::endl;
	}

	return 0;
}