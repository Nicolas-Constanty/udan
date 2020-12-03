#pragma once

#include "debugpch.h"

namespace udan::debug
{
	enum class VerbosityLevel : uint8_t
	{
		TRA = 0,
		DEB = 1,
		INF = 2,
		WAR = 3,
		ERR = 4,
		FAT = 5,
		NO = 6
	};
}
