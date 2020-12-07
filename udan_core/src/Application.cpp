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
	Application  *Application::m_instance = nullptr;
	Application::Application(int argc, char* argv[]) : m_entityManager(ENTITY_CAPACITY)
	{
		if (m_instance != nullptr)
		{
			delete this;
		}
		m_instance = this;
		//debug::Logger::init();
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
			int count = 0;
			//const auto entities = m_entityManager.GetEntities();
			//for (const auto &entity : entities)
			//{
			//	//int i = entity * entity;
			//	//LOG_DEBUG("{}", entity);
			//	count++;
			//}
			const float deltaTime = m_clock->Tick();
			time += deltaTime;
			fps++;
			if (time >= 1.f)
			{
				LOG_DEBUG("Entity Count : {}", count);
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

	ecs::EntityManager<Entity> *Application::GetEntityManager()
	{
		return &m_entityManager;
	}
}
