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
		explicit ATask(TaskPriority priority = TaskPriority::NORMAL, size_t task_id = 0);
		virtual ~ATask();
		virtual void Exec() = 0;
		[[nodiscard]] TaskPriority GetPriority() const;
		[[nodiscard]] uint64_t GetId() const;
		static void ResetId();
	private:
		TaskPriority m_priority;
		uint64_t m_id;
		static uint64_t m_taskId;
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
		[[nodiscard]] const std::vector<uint64_t> &Dependencies() const;

	private:
		std::vector<uint64_t> m_dependencies;
	};

	class DebugTaskDecorator : public ATask
	{
	public:
		explicit DebugTaskDecorator(ATask* task);
		~DebugTaskDecorator() override;
		void Exec() override;

	private:
		std::unique_ptr<ATask> m_task;
	};
}
