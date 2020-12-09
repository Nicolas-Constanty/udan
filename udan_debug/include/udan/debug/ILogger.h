#pragma once

#include "verbosity_level.h"

namespace udan
{
	namespace debug
	{
		class ILogger
		{
		public:
			virtual ~ILogger() = default;
			virtual void trace(const std::string& msg) noexcept = 0;
			virtual void debug(const std::string& msg) noexcept = 0;
			virtual void info(const std::string& msg) noexcept = 0;
			virtual void warn(const std::string& msg) noexcept = 0;
			virtual void err(const std::string& msg) noexcept = 0;
			/// <summary>
			/// Fatal will print the error and stop your program
			/// </summary>
			virtual void fatal(const std::string& msg) noexcept(false) = 0;
			
			virtual void trace(const std::string_view& msg) noexcept = 0;
			virtual void debug(const std::string_view& msg) noexcept = 0;
			virtual void info(const std::string_view& msg) noexcept = 0;
			virtual void warn(const std::string_view& msg) noexcept = 0;
			virtual void err(const std::string_view& msg) noexcept = 0;
			/// <summary>
			/// Fatal will print the error and stop your program
			/// </summary>
			virtual void fatal(const std::string_view& msg) noexcept(false) = 0;

			/// <summary>
			/// Enable / Disable logging
			/// </summary>
			/// <param name="silent"></param>
			virtual void set_silent(bool silent) noexcept = 0;
			virtual void set_verbosity(VerbosityLevel verbosity_level) noexcept = 0;
			/// <summary>
			/// log will also be written inside the specified dir
			/// </summary>
			/// <param name="log_dir"></param>
			virtual void set_log_dir(const std::wstring& log_dir) noexcept = 0;
		};
	}
}