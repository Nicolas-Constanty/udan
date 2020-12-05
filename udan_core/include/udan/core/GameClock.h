#pragma once
#include "Clock.h"

namespace udan::core
{
	class GameClock final : public IClock
	{
	public:
		GameClock(uint16_t framerate);
		double GetCurrentTimeSeconds() const;
		float Tick() override;
		~GameClock() override;
	private:
		static void Wait(double second);
		double m_time;
		double m_frequency;
		double m_targetDelta;
	};
}
