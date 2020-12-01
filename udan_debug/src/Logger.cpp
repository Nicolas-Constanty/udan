#include <filesystem>
// TODO Update to C++ 2020 when available for https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
// #include <format>
#include "string_format.h"

#include "pch.h"
#include "Logger.h"

namespace udan {
	namespace debug {
		void Logger::set_silent(bool silent)
		{
			m_silent = true;
		}

		void Logger::set_verbosity(VerbosityLevel verbosityLevel)
		{
			m_verbosityLevel = verbosityLevel;
		}

		void Logger::set_log_dir(const std::wstring& log_dir)
		{
			if (m_fileHandle && m_fileHandle.is_open())
				m_fileHandle.close();
			//if (!std::filesystem::exists(log_dir))
			//{
			//	
			//}
		}
	}
}


