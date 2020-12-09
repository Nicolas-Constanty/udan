#include "utilspch.h"

#include "Task.h"

#include "udan/debug/Logger.h"

namespace udan::utils
{
	uint64_t ATask::m_taskId = 1;
	ATask::ATask(TaskPriority priority, size_t task_id) : m_priority(priority), m_id(task_id == 0 ? m_taskId++ : task_id)
	{
	}
	
	ATask::~ATask() = default;

	inline TaskPriority ATask::GetPriority() const
	{
		return m_priority;
	}

	inline uint64_t ATask::GetId() const
	{
		return m_id;
	}

	void ATask::ResetId()
	{
		m_taskId = 0;
	}

	Task::Task(std::function<void()> task_function, TaskPriority priority) :
	ATask(priority),
	m_task(std::move(task_function)),
	m_completed(false)
	{
	}

	Task::~Task()
	{
	}

	void Task::Exec()
	{
		m_task();
		m_completed = true;
	}

	inline bool Task::Completed() const
	{
		return m_completed;
	}

	DependencyTask::DependencyTask(
		std::function<void()> task_function,
		const std::vector<DependencyTask*>& tasks,
		TaskPriority priority) :
	Task(std::move(task_function), priority)
	{
		for (const auto& task : tasks)
		{
			if (task == this)
				continue;
			m_dependencies.push_back(task->GetId());
		}
	}

	DependencyTask::~DependencyTask()
	{
		m_dependencies.clear();
	}
	
	const std::vector<uint64_t> &DependencyTask::Dependencies() const
	{
		return m_dependencies;
	}

	DebugTaskDecorator::DebugTaskDecorator(ATask* task) : ATask(task->GetPriority(), task->GetId()), m_task(std::unique_ptr<ATask>(task))
	{
	}

	DebugTaskDecorator::~DebugTaskDecorator()
	{
	}

	void DebugTaskDecorator::Exec()
	{
		LOG_DEBUG("Starting task {}", m_task->GetId());
		m_task->Exec();
		LOG_DEBUG("Task {} has been executed", m_task->GetId());
	}
}
