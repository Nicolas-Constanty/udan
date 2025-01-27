﻿#pragma once

#include "utilspch.h"

#include "WindowsApi.h"
#include "udan/debug/Logger.h"


namespace udan::utils
{
	class CriticalSectionLock
	{
		CRITICAL_SECTION m_critical_sec;

	public:
		explicit CriticalSectionLock(DWORD dwSpinCount = 4000)
		{
			if (InitializeCriticalSectionAndSpinCount(&m_critical_sec, dwSpinCount) == 0)
			{
				LOG_ERR(GetErrorString());
			}
		}
		bool TryLock()
		{
			return TryEnterCriticalSection(&m_critical_sec);
		}
		void Lock()
		{
			EnterCriticalSection(&m_critical_sec);
		}
		void Unlock()
		{
			LeaveCriticalSection(&m_critical_sec);
		}
		PCRITICAL_SECTION Handle()
		{
			return &m_critical_sec;
		}
	};
}
