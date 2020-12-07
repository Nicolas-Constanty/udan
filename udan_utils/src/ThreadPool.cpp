#include "utilspch.h"
#include "ThreadPool.h"

#include "WindowsApi.h"
#include "udan/debug/Logger.h"

namespace udan::utils
{
	size_t ThreadPool::m_taskId = 1;
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
#if DEBUG
		m_tasks.push(std::make_unique<DebugTaskDecorator>(task, m_taskId++));
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
			m_cv.wait(lck, [this]()
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
