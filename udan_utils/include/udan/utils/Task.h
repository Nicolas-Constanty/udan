#pragma once
#include <condition_variable>
#include <functional>
#include <vector>

namespace udan::utils
{
	enum class TaskPriority : uint16_t
	{
		LOW = 0,
		NORMAL = 1,
		HIGH = 2,
		CRITICAL = 3
	};
	
	class ATask
	{
	public:
		explicit ATask(TaskPriority priority = TaskPriority::NORMAL);
		virtual ~ATask();
		virtual void Exec() = 0;
		[[nodiscard]] TaskPriority GetPriority() const;

	private:
		TaskPriority m_priority;


	};

	inline bool operator<(const std::unique_ptr<ATask>& lhs, const std::unique_ptr <ATask>& rhs)
	{
		return lhs->GetPriority() > rhs->GetPriority();
	}

	inline bool operator>(const std::unique_ptr <ATask>& lhs, const std::unique_ptr < ATask>& rhs)
	{
		return lhs->GetPriority() < rhs->GetPriority();
	}

	class Task : public ATask
	{
	public:
		explicit Task(std::function<void()> task_function, TaskPriority priority = TaskPriority::NORMAL);
		~Task() override;
		void Exec() override;
		[[nodiscard]] bool Completed() const;

	private:
		std::function<void()> m_task;
		bool m_completed;

	};

	/// <summary>
	/// This task can specify some other tasks dependencies
	/// </summary>
	class DependencyTask final : public Task
	{
	public:
		explicit DependencyTask(
			std::function<void()> task_function,
			const std::vector<DependencyTask*>& tasks = {},
			TaskPriority priority = TaskPriority::NORMAL);
		~DependencyTask()  override;
		void Exec() override;

	private:
		void WaitForDependencies();

		std::vector<DependencyTask*> m_dependencies;
		std::condition_variable m_cv;
		std::mutex m_mtx;
	};

	class DebugTaskDecorator : public ATask
	{
	public:
		explicit DebugTaskDecorator(ATask* task, size_t id);
		~DebugTaskDecorator() override;
		void Exec() override;

	private:
		std::unique_ptr<ATask> m_task;
		size_t m_id;
	};
}
