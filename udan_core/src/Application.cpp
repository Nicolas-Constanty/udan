#include "corepch.h"

#include "Application.h"

#include <iostream>


#include "Clock.h"
#include "Config.h"
#include "GameClock.h"
#include "UnlimitedClock.h"
#include "udan/debug/Logger.h"

namespace udan::core
{
	Application::Application(int argc, char* argv[])
	{
		debug::Logger::init();
		udan::core::createConfig(argc, argv, &m_config);
		debug::Logger::Instance()->set_level(static_cast<spdlog::level::level_enum>(m_config.app.verbosity));
		SetFramerate(m_config.app.framerate);
	}

	void Application::Run()
	{
		float time = 0.f;
		uint32_t fps = 0;
		while (true)
		{
			const float deltaTime = m_clock->Tick();
			time += deltaTime;
			fps++;
			if (time >= 1.f)
			{
				LOG_TRACE("FPS ({}) - Delta time: {}", fps, deltaTime);
				fps = 0;
				time = 0.f;
			}
		}
	}

	void Application::SetFramerate(uint16_t framerate)
	{
		m_config.app.framerate = framerate;
		if (m_config.app.framerate != 0)
			m_clock = std::make_unique<GameClock>(m_config.app.framerate);
		else
			m_clock = std::make_unique<UnlimitedClock>();
	}
}
