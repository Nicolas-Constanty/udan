#pragma once

#include <memory>

#include "Clock.h"
#include "Config.h"
#include "udan/ecs/EntityManager.h"

namespace udan::core
{
#if ENTITY_CAPACITY > 4294967295
	typedef uint64_t Entity;
#elif ENTITY_CAPACITY > 65535
	typedef uint32_t Entity;
#elif ENTITY_CAPACITY > 254
	typedef uint16_t Entity;
#else
	typedef uint8_t Entity;
#endif
	
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
