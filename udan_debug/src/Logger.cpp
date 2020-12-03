#include "debugpch.h"


#include "Logger.h"

namespace udan::debug
{
	std::shared_ptr<spdlog::logger> Logger::m_logger;
	void Logger::init()
	{
		m_logger = spdlog::stdout_color_mt("Engine");
	}
}
//#include <spdlog/spdlog.h>
//
//#include <filesystem>
////#include <fmt/core.h>
//
//#include "Logger.h"
//
//namespace udan::debug
//{
//	std::unique_ptr<ILogger> Logger::m_instance;
//	
//	void Logger::trace(const std::string& msg)
//	{
//		m_instance->trace(msg);
//	}
//
//	void Logger::debug(const std::string& msg)
//	{
//		m_instance->debug(msg);
//	}
//
//	void Logger::info(const std::string& msg)
//	{
//		m_instance->info(msg);
//	}
//
//	void Logger::warn(const std::string& msg)
//	{
//		m_instance->warn(msg);
//	}
//
//	void Logger::err(const std::string& msg)
//	{
//		m_instance->err(msg);
//	}
//
//	void Logger::fatal(const std::string& msg)
//	{
//		m_instance->fatal(msg);
//		QUIT();
//	}
//
//	//template <typename ... Args>
//	//void Logger::trace_v(const std::string_view& fmt, Args&&... args)
//	//{
//	//	m_instance->trace(fmt::format(fmt, std::forward<Args>(args)...));
//	//}
//
//	//template <typename ... Args>
//	//void Logger::debug_v(const std::string_view& fmt, Args&&... args)
//	//{
//	//	m_instance->debug(fmt::format(fmt, std::forward<Args>(args)...));
//	//}
//
//	//template <typename ... Args>
//	//void Logger::info_v(const std::string_view& fmt, Args&&... args)
//	//{
//	//	m_instance->info(fmt::format(fmt, std::forward<Args>(args)...));
//	//}
//
//	//template <typename ... Args>
//	//void Logger::warn_v(const std::string_view& fmt, Args&&... args)
//	//{
//	//	m_instance->warn(fmt::format(fmt, std::forward<Args>(args)...));
//	//}
//
//	/*template <typename ... Args>
//	void Logger::err_v(const std::string_view& fmt, Args&&... args)
//	{
//		m_instance->err(fmt::format(fmt, std::forward<Args>(args)...));
//	}*/
//
//	template <typename ... Args>
//	void Logger::fatal_v(const std::string_view& fmt, Args&&... args)
//	{
//		m_instance->fatal(fmt::format(fmt, std::forward<Args>(args)...));
//		QUIT();
//	}
//
//	void Logger::set_silent(bool silent)
//	{
//		m_instance->set_silent(silent);
//	}
//
//	void Logger::set_verbosity(VerbosityLevel verbosityLevel)
//	{
//		m_instance->set_verbosity(verbosityLevel);
//	}
//
//	void Logger::set_log_dir(const std::wstring& log_dir)
//	{
//		m_instance->set_log_dir(log_dir);
//	}
//}
//
//
