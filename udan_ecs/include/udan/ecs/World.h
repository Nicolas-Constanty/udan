#pragma once

#include <vector>


#include "EntityManager.h"
#include "System.h"

namespace udan::ecs
{
	template <typename T>
	class World
	{
	public:
		World(size_t capacity)
		{
			m_entityManager = EntityManager<T>::Instance(); 
			m_entities.reserve(capacity);
		}
		
		void PushEntity(T entity)
		{
			m_entities.push_back(entity);
		}
		
		void AddSystem(System system)
		{
			m_systems.push_back(system);
		}

		T CreateEntity()
		{
			T entity = m_entityManager->CreateEntity();
			m_entities.push_back(entity); 
			return entity;
		}
	private:
		std::list<System> m_systems;
		std::vector<T> m_entities;

		EntityManager<T> *m_entityManager;
	};
}
