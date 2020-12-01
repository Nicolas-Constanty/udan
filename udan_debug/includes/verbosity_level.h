#pragma once

#include "pch.h"

namespace udan {
	namespace debug {
		static enum class VerbosityLevel : uint8_t
		{
			TRACE = 0,
			DEBUG = 1,
			INFO = 2,
			WARN = 3,
			FATAL = 4,
			NO = 5
		};
	}
}