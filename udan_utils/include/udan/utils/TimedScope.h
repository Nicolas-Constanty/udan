#pragma once

#include "utilspch.h"

#include <timeapi.h>

#include "udan/debug/Logger.h"

namespace udan::utils
{
	template<typename Timer>
	class TimedScope
	{
	public:
		~TimedScope()
		{
			LOG_DEBUG("TimedScope: {} s", m_timer->GetDeltaTime());
		}

	private:
		Timer m_timer;
	};
}
