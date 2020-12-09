#pragma once

#include "utilspch.h"


namespace udan::utils
{
	class CriticalSectionLock
	{
	public:
		CriticalSectionLock()
		{
			InitializeCriticalSectionAndSpinCount(&m_critical_sec, 4000);
			EnterCriticalSection(&m_critical_sec);
		}

		PCRITICAL_SECTION Handle()
		{
			return &m_critical_sec;
		}

		~CriticalSectionLock()
		{
			LeaveCriticalSection(&m_critical_sec);
		}
	private:
		CRITICAL_SECTION m_critical_sec;
	};
}