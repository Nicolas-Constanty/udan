#include "corepch.h"

#include <timeapi.h>
#include "UnlimitedClock.h"

namespace udan::core
{
	UnlimitedClock::UnlimitedClock()
	{
		// Set windows timer granularity to 1ms
		timeBeginPeriod(1);
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		m_frequency = static_cast<double>(frequency.QuadPart);
		m_time = GetCurrentTimeSeconds();
	}

	inline float UnlimitedClock::Tick()
	{
		const double currentTime = GetCurrentTimeSeconds();
		const double deltaTime = currentTime - m_time;
		m_time = currentTime;
		return static_cast<float>(deltaTime);
	}

	UnlimitedClock::~UnlimitedClock()
	{
		timeEndPeriod(1);
	}

	inline double UnlimitedClock::GetCurrentTimeSeconds() const
	{
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);
		return  static_cast<double>(time.QuadPart) / m_frequency;
	}
}
