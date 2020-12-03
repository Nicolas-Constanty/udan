#include "debugpch.h"


#include "Logger.h"

namespace udan::debug
{
	std::shared_ptr<spdlog::logger> Logger::m_logger;
	void Logger::init()
	{
		m_logger = spdlog::stdout_color_mt("Engine");
		m_logger->set_level(spdlog::level::info);
		m_logger->enable_backtrace(1024);
	}
}