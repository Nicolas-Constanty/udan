#pragma once

#include <typeindex>
#include <vector>


#include "EntityManager.h"
#include "System.h"
#include "SystemTask.h"
#include "udan/utils/SparseSet.h"
#include "udan/utils/ThreadPool.h"

namespace udan::ecs
{
	
	/**
	 * \brief Iterate over ...Args https://stackoverflow.com/questions/28263000/correct-usage-of-for-each-arg-too-much-forwarding
	 * \tparam Fs 
	 * \param fs 
	 */
	template<class...Fs>
	void do_in_order(Fs&&...fs) {
		int _[] = { 0,
		  (((void)(std::forward<Fs>(fs)())),0)...
		};
		(void)_; // kills warnings
	}

	inline size_t gTypeMapping = 0;
	template<typename Component>
	struct type_id_mapping
	{
		static size_t index()
		{
			static size_t indextype = gTypeMapping++;
			return indextype;
		}
	};

	template <size_t I = 0, typename... Ts>
	void GetTupleTypes(std::tuple<Ts...> tup, std::array<size_t, sizeof...(Ts)>& result)
	{
		// If we have iterated through all elements 
		if
			constexpr (I == sizeof...(Ts))
		{
			// Last case, if nothing is left to 
			// iterate, then exit the functiopn 
			return;
		}
		else {
			result[I] = type_id_mapping<std::decay_t<decltype(std::get<I>(tup))>>::index();
			// Going for next element. 
			GetTupleTypes<I + 1>(tup, result);
		}
	}

	template <size_t I = 0, typename... Ts>
	std::array<size_t, sizeof...(Ts)> GetTupleTypes(std::tuple<Ts...> tup)
	{
		std::array<size_t, sizeof...(Ts)> result;
		GetTupleTypes(tup, result);
		return result;
	}

	template <typename Entity>
	class World
	{
		template<typename Component>
		using data_storage = utils::DataSet<Entity, Component>;
	public:
		World(size_t capacity) : m_threadPool(2)
		{
			m_entityManager = EntityManager<Entity>::Instance();
			m_entities.reserve(capacity);
		}

		~World()
		{
			m_threadPool.StopWhenQueueEmpty();
		}

		template<typename ...Components>
		void PushComponents(Entity entity, Components &&...components)
		{
			do_in_order(
				[&, entity] {
					auto component = std::forward<Components>(components);
					const auto type_index = type_id_mapping<decltype(component)>::index();
					if (m_dataPool.size() <= type_index)
					{
						m_dataPool.push_back(std::make_unique<utils::DataSet<Entity, decltype(component)>>(m_entities.capacity()));
					}
					GetComponentStorage<decltype(component)>(type_index).PushBack(entity, component);
				}...
			);
		}

		template<typename Component, typename ...Args>
		void EmplaceComponent(Entity entity, Args&& ...args)
		{
			const auto type_index = type_id_mapping<Component>::index();
			if (m_dataPool.size() <= type_index)
			{
				//Create pool if doesn't exist
				m_dataPool.push_back(std::make_unique<utils::DataSet<Entity, Component>>(m_entities.capacity()));
			}
			GetComponentStorage<Component>(type_index).EmplaceBack(entity, std::forward<Args>(args)...);
		}

		template<typename ...Components>
		void AddSystem(void(*func)(Components...))
		{
			auto view = utils::DataSetView<data_storage<std::decay_t<Components>>...>(GetComponentStorage<std::decay_t<Components>>() ...);
			const size_t min = view.GetSize();
			m_systems.push_back(
				[func{ std::move(func) }, view{ std::move(view) }, min]() mutable
				{
					for (auto i = 0; i < min; ++i)
					{
						std::apply(func, view.Get(i));
					}
				}
			);
		}

		template<typename ...Components>
		void AddSystemLambdas(std::function<void(Components...)> func)
		{
			auto view = utils::DataSetView<data_storage<std::decay_t<Components>>...>(GetComponentStorage<std::decay_t<Components>>() ...);
			const size_t min = view.GetSize();
			m_systems.push_back(
				[func { std::move(func) }, view{ std::move(view) }, min]() mutable
				{
					for (auto i = 0; i < min; ++i)
					{
						std::apply(func, view.Get(i));
					}
				}
			);
		}

		template<typename ...Components>
		void AddMtSystem(void(*func)(Components...))
		{
			auto view = utils::DataSetView<data_storage<std::decay_t<Components>>...>(GetComponentStorage<std::decay_t<Components>>() ...);
			const size_t min = view.GetSize();
			const size_t thread_count = m_threadPool.GetThreadCount();
			const size_t index_per_thread = min / thread_count;
			std::vector<std::shared_ptr<utils::ATask>> tasks;
			tasks.reserve(thread_count);
			for (auto i = 0; i < thread_count; ++i)
			{
				const size_t start = i * index_per_thread;
				size_t end = start + index_per_thread;
				if (end > min || (i == thread_count - 1 && end < min))
					end = min;
				tasks.push_back(std::make_shared<SystemTask<decltype(func), decltype(view)>>(start, end, func, view));
			}
			m_systems.push_back(
				[this, tasks{ std::move(tasks) }]()
				{
					for (const auto& task : tasks)
					{
						m_threadPool.Schedule(task);
					}
					m_threadPool.WaitUntilQueueEmpty();
				}
			);
		}

		template<typename ...Components>
		void AddMtSystemLambdas(std::function<void(Components...)> func)
		{
			auto view = utils::DataSetView<data_storage<std::decay_t<Components>>...>(GetComponentStorage<std::decay_t<Components>>() ...);
			const size_t min = view.GetSize();
			const size_t thread_count = m_threadPool.GetThreadCount();
			const size_t index_per_thread = min / thread_count;
			std::vector<std::shared_ptr<utils::ATask>> tasks;
			tasks.reserve(thread_count);
			for (auto i = 0; i < thread_count; ++i)
			{
				const size_t start = i * index_per_thread;
				size_t end = start + index_per_thread;
				if (end > min || (i == thread_count - 1 && end < min))
					end = min;
				tasks.push_back(std::make_shared<SystemTask<decltype(func), decltype(view)>>(start, end, func, view));
			}
			m_systems.push_back(
				[this, tasks {std::move(tasks)}]()
				{
					for (const auto& task : tasks)
					{
						m_threadPool.Schedule(task);
					}
					m_threadPool.WaitUntilQueueEmpty();
				}
			);
		}

		template<typename T1, typename T2>
		void RunSystem(const std::function<void(T1, T2)> &func)
		{
			std::vector<std::_Remove_cvref_t<T1>>& comp1 = GetComponentStorage<std::_Remove_cvref_t<T1>>().GetData();
			std::vector<std::_Remove_cvref_t<T2>>& comp2 = GetComponentStorage<std::_Remove_cvref_t<T2>>().GetData();
			size_t s1 = comp1.size();
			size_t s2 = comp2.size();
			size_t min = s1 < s2 ? s1 : s2;
			for (int i = 0; i < min; ++i)
			{
				std::_Remove_cvref_t<T1>& r_comp1 = comp1[i];
				std::_Remove_cvref_t<T2>& r_comp2 = comp2[i];
				func(r_comp1, r_comp2);
			}
		}

		template<typename T1, typename T2>
		void RunSystem1(void(*func)(T1, T2))
		{
			std::vector<std::_Remove_cvref_t<T1>>& comp1 = GetComponentStorage<std::_Remove_cvref_t<T1>>().GetData();
			std::vector<std::_Remove_cvref_t<T2>>& comp2 = GetComponentStorage<std::_Remove_cvref_t<T2>>().GetData();
			size_t s1 = comp1.size();
			size_t s2 = comp2.size();
			size_t min = s1 < s2 ? s1 : s2;
			for (int i = 0; i < min; ++i)
			{
				std::_Remove_cvref_t<T1>& r_comp1 = comp1[i];
				std::_Remove_cvref_t<T2>& r_comp2 = comp2[i];
				func(r_comp1, r_comp2);
			}
		}

		void Update()
		{
			for (auto systemFunc: m_systems)
			{
				systemFunc();
			}
		}

		template<typename Component>
		Component &GetEntityComponent(Entity id)
		{
			return GetComponentStorage<Component>().GetComponent(id);
		}

		Entity CreateEntity()
		{
			Entity entity = m_entityManager->CreateEntity();
			m_entities.push_back(entity); 
			return entity;
		}

		template<typename Component>
		data_storage<Component>& InitDataStorage()
		{
			const auto type_index = type_id_mapping<Component>::index();
			if (m_dataPool.size() <= type_index)
			{
				m_dataPool.push_back(std::make_unique<utils::DataSet<Entity, Component>>(m_entities.capacity()));
			}
			return GetComponentStorage<Component>(type_index);
		}

	private:
		template<typename Component>
		data_storage<Component>& GetComponentStorage()
		{
			return static_cast<data_storage<Component>&>(*m_dataPool[type_id_mapping<Component>::index()]);
		}

		template<typename Component>
		data_storage<Component>& GetComponentStorage(size_t index)
		{
			return static_cast<data_storage<Component>&>(*m_dataPool[index]);
		}
		
		std::list<std::function<void()>> m_systems;
		std::vector<Entity> m_entities;
		std::vector<std::unique_ptr<utils::SparseSet<Entity>>> m_dataPool;
		utils::ThreadPool m_threadPool;
		EntityManager<Entity> *m_entityManager;
	};
}