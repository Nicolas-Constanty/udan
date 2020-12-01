#pragma once

#include <iostream>
#include <fstream>

#include "pch.h"
#include "ainsi_color.h"
#include "verbosity_level.h"

namespace udan {
	namespace debug {
		class Logger
		{
		public:
			void set_silent(bool silent);
			void set_verbosity(VerbosityLevel verbosityLevel);
			void set_log_dir(const std::wstring& log_dir);

		private:
			std::string m_date_format;
			std::wstring m_name;
			VerbosityLevel m_verbosityLevel;
			std::ofstream m_fileHandle;
			bool m_silent;
		};
	}
}


