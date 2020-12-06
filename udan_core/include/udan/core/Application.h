#pragma once

#include <memory>

#include "Clock.h"
#include "Config.h"
#include "udan/ecs/EntityManager.h"

namespace udan::core
{
	typedef uint32_t Entity;
	extern const Entity ENTITY_CAPACITY;
	class Application
	{
	public:
		virtual ~Application() = default;
		Application(int argc, char* argv[]);
		virtual void Init(int argc, char* argv[]) = 0;
		void Run();
		void SetFramerate(uint16_t framerate);
		ecs::EntityManager<Entity> *GetEntityManager();
	private:
		std::unique_ptr<IClock> m_clock;
		Config m_config;
		udan::ecs::EntityManager<Entity> m_entityManager;
	};
}
