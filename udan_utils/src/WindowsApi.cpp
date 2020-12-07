#include "utilspch.h"

#include "udan/debug/Logger.h"
#include "WindowsApi.h"

namespace udan::utils
{
	std::string GetErrorString()
	{
		const DWORD errCode = GetLastError();
		char* err;
		if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			nullptr,
			errCode,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // default language
			reinterpret_cast<LPTSTR>(&err),
			0,
			nullptr))
		{
			LOG_FATAL("Format message failed.");
		}
		return std::string(err);
	}
}
