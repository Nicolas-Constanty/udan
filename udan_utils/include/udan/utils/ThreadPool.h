#pragma once
#include <condition_variable>
#include <queue>
#include <thread>
#include <vector>

#include "Task.h"

namespace udan::utils
{
	class ThreadPool
	{
	public:
		ThreadPool(size_t capacity);
		void Stop();
		/**
		 * \brief This function may lead to UB since thread are directly killed prefer Stop over Interrupt
		 * (https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-terminatethread)
		 */
		#pragma message ("Cancel job immediatly, prefer Stop over Interrupt if you want to exit the threadpool safely")
		void Interrupt();

		void Schedule(ATask *task);

		//void ThreadPool::Print();
	private:
		void Run();
		std::vector<std::thread> m_threads;		
		std::condition_variable m_cv;
		std::mutex m_mtx;
		bool m_shouldRun;

		std::priority_queue<std::unique_ptr<ATask>, std::vector<std::unique_ptr<ATask>>, std::greater<>> m_tasks;

		static size_t m_taskId;
	};
}
