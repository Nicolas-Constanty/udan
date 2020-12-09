#pragma once

#include <map>
#include <queue>
#include <set>
#include <thread>
#include <vector>


#include "ConditionVariable.h"
#include "Task.h"

namespace udan::utils
{
	class ThreadPool
	{
	public:
		ThreadPool(size_t capacity);
		/**
		 * \brief This function finish running, then stop threads and join
		 */
		void Stop();
		void StopWhenQueueEmpty();
		void WaitUntilQueueEmpty();
		
		/**
		 * \brief This function may lead to UB since thread are directly killed prefer Stop over Interrupt
		 * (https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-terminatethread)
		 */
		#pragma message ("Cancel job immediatly, prefer Stop over Interrupt if you want to exit the threadpool safely")
		void Interrupt();

		void Schedule(ATask *task);
		void ResetTaskCount();

		//void ThreadPool::Print();
	private:
		void ScheduleCompletedDependency(ATask* task);
		void Run();
		std::vector<std::thread> m_threads;		
		ConditionVariable m_cv;
		ConditionVariable m_queueEmpty;
		CriticalSectionLock m_mtx;
		bool m_shouldRun;

		std::priority_queue<std::unique_ptr<ATask>, std::vector<std::unique_ptr<ATask>>, std::greater<>> m_tasks;
		std::list<DependencyTask *> m_awaintingTasks;
		std::set<uint64_t> m_completedTasks;
	};
}
