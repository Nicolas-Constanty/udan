#pragma once

#include <atomic>
#include "utilspch.h"

namespace udan::utils
{
	class SpinLock
	{
	public:
		SpinLock()
		{}

		bool TryLock()
		{
			return !m_flag.test_and_set(std::memory_order_acquire);
		}

		void Lock()
		{
			while (!TryLock())
			{
				YieldProcessor();
			}
		}

		void Unlock()
		{
			m_flag.clear(std::memory_order_release);
		}

	private:
		std::atomic_flag m_flag;
	};
}
