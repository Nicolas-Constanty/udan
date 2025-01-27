﻿#pragma once

#include "ecspch.h"

#include <queue>
#include <unordered_set>

namespace udan::ecs
{
	//template<typename T1>
	//class World;
	template<typename T>
	class EntityManager
	{
	public:
		//typedef  std::vector<T>::iterator iterator;
		explicit EntityManager(size_t entityCapacity) :
		m_entityPos(entityCapacity, entityCapacity-1)
		{
			if (m_instance != nullptr)
				LOG_FATAL("Cannot have two instances of EntityManager");
			m_instance = this;
			m_entities.reserve(entityCapacity);
			m_noEntity = entityCapacity - 1;
		}
		T CreateEntity()
		{
			//const T pos = m_entities.size();
			//const unsigned condition = m_entityPos[pos] == m_noEntity;
			//const unsigned mask = 0U - condition;
			//const T ent = (pos & mask) | (GetNextEntityPos() & ~mask);
			//m_entities.push_back(ent);
			//m_entityPos[ent] = pos;
			//return ent;
			T pos = m_entities.size();
			T idx = pos;
			if (m_entityPos[idx] != m_noEntity)
			{
				idx = m_freeEntities.front();
				m_freeEntities.pop();
			}
			m_entities.emplace_back(idx);
			m_entityPos[idx] = pos;
			return idx;
		}
		//   |  0| -1| -1|...
		//   |  0|

		//   |  0|  1| -1|...
		//   |  0|  1|

		//   |  0|  1| -1|...  -- Release 1 swap
		//   |  1|  0|

		//   |  1|  0| -1|...  -- Release 2 swap
		//   |  1|  0|

		//   |  1|  0| -1|...  -- Release pop
		//   |  1|

		//   | -1|  0| -1|...  -- Release put to -1
		//   |  1|

		//   | -1|  0| -1|...  -- Release put to -1
		//   |  1|

		//   | -1|  0| -1|...  -- New
		//   |  1|

		void ReleaseEntity(T entity)
		{
			const auto last = m_entities.back();
			std::swap(m_entities.back(), m_entities[m_entityPos[entity]]);
			std::swap(m_entityPos[last], m_entityPos[entity]);
			m_entities.pop_back();
			m_entityPos[entity] = m_noEntity;
			m_freeEntities.push(entity);
		}

		const std::vector<T> &GetEntities() const
		{
			return m_entities;
		}

		bool EntityExist(T entity)
		{
			return m_entityPos[entity] != m_noEntity;
		}
		
		const T& operator[](std::size_t idx) const
		{
			return m_entities[idx];
		}

		//std::shared_ptr<T> CreateWorld(const std::string &name, size_t capacity)
		//{
		//	auto world = std::make_shared<World<T>>(capacity);
		//	m_worlds.emplace(name, world);
		//	return world;
		//}

		//std::shared_ptr<T> GetWorld(const std::string &name)
		//{
		//	return m_worlds[name];
		//}

		static EntityManager *Instance()
		{
			return m_instance;
		}

	private:
		T GetNextEntityPos()
		{
			T index = 0;
			while (m_entityPos[index] != m_noEntity)
			{
				index++;
			}
			return index;
		}
		
		T m_noEntity;
		std::vector<T> m_entityPos;
		std::vector<T> m_entities;
		std::queue<T> m_freeEntities;

		//std::unordered_map<std::string, std::shared_ptr<World<T>>> m_worlds;
		/*std::vector<World> m_worlds;*/

		inline static EntityManager *m_instance = nullptr;
	};
}
