#pragma once
#include "Clock.h"

namespace udan::core
{
	class UnlimitedClock final : public IClock
	{
	public:
		UnlimitedClock();
		double GetCurrentTimeSeconds() const;
		float Tick() override;
		~UnlimitedClock() override;
	private:
		double m_time;
		double m_frequency;
	};
}
