#include "debugpch.h"


#include "Logger.h"

namespace udan::debug
{
	std::shared_ptr<spdlog::logger> Logger::m_logger = nullptr;
	std::mutex Logger::m_mtx;

	void Logger::Init()
	{
		m_logger = spdlog::stdout_color_mt("Engine");
		m_logger->set_level(spdlog::level::trace);
		m_logger->enable_backtrace(1024);
	}
}