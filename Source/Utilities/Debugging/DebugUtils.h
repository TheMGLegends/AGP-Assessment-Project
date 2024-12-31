#pragma once

#include <string>

namespace DebugUtils
{
	void Log(const std::string& message);

	void LogWarning(const std::string& message);

	void LogError(const std::string& message);

	std::string WStringToString(const std::wstring& wstring);
}