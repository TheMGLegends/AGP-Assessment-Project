#include "DebugUtils.h"

#include <iostream>
#include <Windows.h>

void DebugUtils::Log(const std::string& message)
{
	std::cout << message << std::endl;
}

void DebugUtils::LogWarning(const std::string& message)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// INFO: Save the current console attributes
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	WORD savedAttributes = consoleInfo.wAttributes;

	// INFO: Set the text colour to yellow
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

	std::cout << "Warning: " << message << std::endl;

	// INFO: Restore the original console attributes
	SetConsoleTextAttribute(hConsole, savedAttributes);
}

void DebugUtils::LogError(const std::string& message)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// INFO: Save the current console attributes
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	WORD savedAttributes = consoleInfo.wAttributes;

	// INFO: Set the text colour to red
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);

	std::cout << "Error: " << message << std::endl;

	// INFO: Restore the original console attributes
	SetConsoleTextAttribute(hConsole, savedAttributes);
}

std::string DebugUtils::WStringToString(const std::wstring& wstring)
{
	std::string string;
	size_t size;
	string.resize(wstring.length());
	wcstombs_s(&size, &string[0], string.size() + 1, wstring.c_str(), wstring.size());
	return string;
}
