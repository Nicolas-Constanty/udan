#pragma once
#include "udan/utils/Task.h"

namespace udan::ecs
{
	template<typename Func, typename View>
	class SystemTask : public utils::ATask
	{
		size_t m_start;
		size_t m_end;
		Func m_func;
		View m_view;
	public:
		explicit SystemTask(size_t start, size_t end, const Func &func, View& view) : ATask(), m_func(std::move(func)), m_view(std::move(view))
		{
			m_start = start;
			m_end = end;
		}

		void Exec() override
		{
			for (auto i = m_start; i < m_end; ++i)
			{
				std::apply(m_func, m_view.Get(i));
			}
			Done();
		}
	};
}
