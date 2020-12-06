#include "corepch.h"

#include <cstdint>
#include <timeapi.h>
#include "GameClock.h"

#include "udan/debug/Logger.h"

namespace udan::core
{
	GameClock::GameClock(uint16_t framerate)
	{
		// Set windows timer granularity to 1ms
		timeBeginPeriod(1);
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		m_frequency = static_cast<double>(frequency.QuadPart);
		m_time = GetCurrentTimeSeconds();
		m_targetDelta = 1.0 / framerate;
		LOG_TRACE("Target framerate: {}", framerate);
		LOG_TRACE("Target delta: {}", m_targetDelta);
	}

	inline float GameClock::Tick()
	{
		double currentTime = GetCurrentTimeSeconds();
		double deltaTime = currentTime - m_time;
		//if (deltaTime > 1)
		//	deltaTime = m_targetDelta;
		double waitTime = m_targetDelta - deltaTime;
		const double targetTime = currentTime + waitTime;
		// Adjust to fixed framerate
		if (waitTime > 0)
		{
			while (currentTime < targetTime)
			{
				// Subtract 1ms to be more precise since windows is >= 1ms (prefer wait less than loss time)
				Wait(waitTime - 0.001);
				currentTime = GetCurrentTimeSeconds();
				waitTime = targetTime - currentTime;
			}
			deltaTime = currentTime - m_time;
		}
		m_time = GetCurrentTimeSeconds();
		return static_cast<float>(deltaTime);
	}

	GameClock::~GameClock()
	{
		timeEndPeriod(1);
	}

	inline double GameClock::GetCurrentTimeSeconds() const
	{
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);
		return  static_cast<double>(time.QuadPart) / m_frequency;
	}

	inline void GameClock::Wait(double seconds)
	{
		const uint32_t ms = static_cast<uint32_t>(seconds * 1000.0);
		if (ms == 0)
		{
			SwitchToThread();
		}
		else
		{
			Sleep(ms);
		}
	}


}
