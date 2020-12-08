﻿#include "utilspch.h"
#include "ThreadPool.h"

#include "WindowsApi.h"
#include "udan/debug/Logger.h"

namespace udan::utils
{
	ThreadPool::ThreadPool(size_t capacity)
	{
		m_shouldRun = true;
		m_threads.reserve(capacity);
		for (size_t i = 0; i < capacity; ++i)
		{
			m_threads.emplace_back([this] { Run(); });
		}
		LOG_INFO("Threadpool launching {} threads...", capacity);
	}

	void ThreadPool::Stop()
	{
		std::unique_lock<std::mutex> lck(m_mtx);
		m_shouldRun = false;
		m_cv.notify_all();
		lck.unlock();
		for (auto& thread : m_threads)
		{
			thread.join();
		}
	}

	void ThreadPool::StopWhenQueueEmpty()
	{
		WaitUntilQueueEmpty();
		Stop();
	}

	void ThreadPool::WaitUntilQueueEmpty()
	{
		std::unique_lock<std::mutex> lck(m_mtx);
		m_queueEmpty.wait(lck, [this]() { return m_tasks.empty(); });
	}

	void ThreadPool::Interrupt()
	{
		std::unique_lock<std::mutex> lck(m_mtx);
		m_shouldRun = false;
		for (auto& thread : m_threads)
		{
			if (TerminateThread(thread.native_handle(), 0) != 0)
			{
				LOG_ERR(GetErrorString());
			}
		}
	}

	/*void ThreadPool::Print()
	{
		while (!m_tasks.empty())
		{
			LOG_DEBUG("{}", m_tasks.top()->GetPriority());
			m_tasks.pop();
		}
	}*/

	void ThreadPool::Schedule(ATask* task)
	{
		std::unique_lock<std::mutex> lck(m_mtx);
		DependencyTask* dt = dynamic_cast<DependencyTask*>(task);
		if (dt != nullptr && !dt->Dependencies().empty())
		{
			bool completed = true;
			for (const auto& depId : dt->Dependencies())
			{
				if (m_completedTasks.find(depId) == m_completedTasks.end())
				{
					completed = false;
				}
			}
			if (!completed)
			{
				m_awaintingTasks.push_back(dt);
				return;
			}
		}
#if DEBUG
		//LOG_DEBUG("Schedule task {}: ", task->GetId());
		m_tasks.push(std::make_unique<DebugTaskDecorator>(task));
#else
		m_tasks.push(std::unique_ptr<ATask>(task));
#endif	
		m_cv.notify_one();
	}

	void ThreadPool::ResetTaskCount()
	{
		m_awaintingTasks.clear();
		m_completedTasks.clear();
		ATask::ResetId();
	}

	void ThreadPool::ScheduleCompletedDependency(ATask* task)
	{
#if DEBUG
		//LOG_DEBUG("Schedule task {}: ", task->GetId());
		m_tasks.push(std::make_unique<DebugTaskDecorator>(task));
#else
		m_tasks.push(std::unique_ptr<ATask>(task));
#endif	
		m_cv.notify_one();
	}

	void ThreadPool::Run()
	{
		LOG_INFO("Start thread {}", GetCurrentThreadId());
		while (m_shouldRun)
		{
			std::unique_lock<std::mutex> lck(m_mtx);
			if (m_tasks.empty())
			{
				m_queueEmpty.notify_one();
			}
			m_cv.wait(lck, [this]()
			{
				return !m_tasks.empty() || !m_shouldRun;
			});
			if (!m_shouldRun)
				break;
			uint64_t taskId = 0;
			{
				const auto& task = m_tasks.top();
				task->Exec();
				taskId = task->GetId();
			}
			m_tasks.pop();
			m_completedTasks.insert(taskId);
			m_awaintingTasks.remove_if([this](DependencyTask* task)
				{
					bool completed = true;
					for (const auto& depId : task->Dependencies())
					{
						if (m_completedTasks.find(depId) == m_completedTasks.end())
						{
							completed = false;
							break;
						}
					}
					if (completed)
					{
						ScheduleCompletedDependency(task);
					}
					return completed;
				});
		}
		LOG_INFO("Exit thread {}", GetCurrentThreadId());
	}
}
