#include "utilspch.h"
#include "ThreadPool.h"

#include "ScopeLock.h"
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
		{
			ScopeLock<decltype(m_mtx)> lck(m_mtx);
			m_shouldRun = false;
			m_cv.NotifyAll();
		}
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
		ScopeLock<decltype(m_mtx)> lck(m_mtx);
		m_queueEmpty.Wait(m_mtx, [this]() { return m_tasks.empty(); });
	}

	void ThreadPool::Interrupt()
	{
		ScopeLock<decltype(m_mtx)> lck(m_mtx);
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

	void ThreadPool::Schedule(const std::shared_ptr<ATask>& task)
	{
		ScopeLock<decltype(m_mtx)> lck(m_mtx);
		auto dt = std::dynamic_pointer_cast<DependencyTask>(task);
		if (dt != nullptr && !dt->Dependencies().empty())
		{
			for (const auto& dependency : dt->Dependencies())
			{
				if (!dependency->Completed())
				{
					dependency->onCompleted += [this, dt, dependency, task]()
					{
						if (dt->RemoveDependency(dependency))
							ScheduleCompletedDependency(task);
					};
					break;
				}
			}
		}
#if DEBUG
		//LOG_DEBUG("Schedule task {}: ", task->GetId());
		m_tasks.push(std::make_shared<DebugTaskDecorator>(task));
#else
		m_tasks.push(task);
#endif	
		m_cv.NotifyOne();
	}

	void ThreadPool::ResetTaskCount()
	{
		ATask::ResetId();
	}

	size_t ThreadPool::GetThreadCount() const
	{
		return m_threads.size();
	}

	void ThreadPool::ScheduleCompletedDependency(const std::shared_ptr<ATask>& task)
	{
		ScopeLock<decltype(m_mtx)> lck(m_mtx);
#if DEBUG
		//LOG_DEBUG("Schedule task {}: ", task->GetId());
		m_tasks.push(std::make_shared<DebugTaskDecorator>(task));
#else
		m_tasks.push(task);
#endif	
		m_cv.NotifyOne();
	}

	void ThreadPool::Run()
	{
		LOG_INFO("Start thread {}", GetCurrentThreadId());
		while (m_shouldRun)
		{
			ScopeLock<decltype(m_mtx)> lck(m_mtx);
			if (m_tasks.empty())
			{
				m_queueEmpty.NotifyOne();
			}
			m_cv.Wait(m_mtx, [this]()
			{
				return !m_tasks.empty() || !m_shouldRun;
			});
			if (!m_shouldRun)
				break;
			m_tasks.top()->Exec();
			m_tasks.pop();
		}
		LOG_INFO("Exit thread {}", GetCurrentThreadId());
	}
}
