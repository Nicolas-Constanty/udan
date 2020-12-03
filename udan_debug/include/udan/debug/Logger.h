#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace udan::debug
{
	class Logger
	{
	public:
		static void init();
		static std::shared_ptr<spdlog::logger> Instance() { return  m_logger; }
		template<typename... Args>
		static void trace(const std::string_view& f, Args&&... args)
		{
			m_logger->trace(f, std::forward<Args>(args)...);
		}
		template<typename... Args>
		static void debug(const std::string_view& f, Args&&... args)
		{
			m_logger->debug(f, std::forward<Args>(args)...);
		}
		template<typename... Args>
		static void info(const std::string_view& f, Args&&... args)
		{
			m_logger->info(f, std::forward<Args>(args)...);
		}
		template<typename... Args>
		static void warn(const std::string_view& f, Args&&... args)
		{
			m_logger->warn(f, std::forward<Args>(args)...);
		}
		template <typename ... Args>
		static void err(const std::string_view& f, Args&&... args)
		{
			m_logger->error(f, std::forward<Args>(args)...);
		}
		template<typename... Args>
		static void fatal(const std::string_view& f, Args&&... args)
		{
			m_logger->critical(f, std::forward<Args>(args)...);
#if DEBUG
			__debugbreak();
#else
			abort();
#endif
		}
	private:
		static std::shared_ptr<spdlog::logger> m_logger;
	};
}

#if USE_SPDLOG

#ifdef DEBUG
#define LOG_TRACE(f, ...) udan::debug::Logger::trace(f,  __VA_ARGS__)
#define LOG_DEBUG(f, ...) udan::debug::Logger::debug(f,  __VA_ARGS__)
#define LOG_INFO(f, ...) udan::debug::Logger::info(f,  __VA_ARGS__)
#define LOG_WARN(f, ...) udan::debug::Logger::warn(f,  __VA_ARGS__)
#define LOG_ERR(f, ...) udan::debug::Logger::err(f,  __VA_ARGS__)
#define LOG_FATAL(f, ...) udan::debug::Logger::fatal(f,  __VA_ARGS__)
#else
#define LOG_TRACE(f, ...)
#define LOG_DEBUG(f, ...)
#define LOG_INFO(f, ...)
#define LOG_WARN(f, ...)
#define LOG_ERR(f, ...)
#define LOG_FATAL(f, ...) udan::debug::Logger::fatal(f,  __VA_ARGS__)
#endif

#endif

//namespace udan::debug
//{
//	class Logger
//	{
//	public:
//		static void trace(const std::string& msg);
//		static void debug(const std::string& msg);
//		static void info(const std::string& msg);
//		static void warn(const std::string& msg);
//		static void err(const std::string& msg);
//		static void fatal(const std::string& msg);
//
//		template<typename... Args>
//		static void trace_v(const std::string_view& f, Args&&... args);
//		template<typename... Args>
//		static void debug_v(const std::string_view& f, Args&&... args);
//		template<typename... Args>
//		static void info_v(const std::string_view& f, Args&&... args);
//		template<typename... Args>
//		static void warn_v(const std::string_view& f, Args&&... args);
//		template <typename ... Args>
//		static void err_v(const std::string_view& f, Args&&... args)
//		{
//			//m_instance->err(fmt::format("", std::forward<Args>(args)...));
//		}
//		template<typename... Args>
//		static void fatal_v(const std::string_view& f, Args&&... args);
//		
//		static void set_silent(bool silent);
//		static void set_verbosity(VerbosityLevel verbosityLevel);
//		static void set_log_dir(const std::wstring& log_dir);
//
//		template<class T>
//		static void init(const std::string& name) noexcept
//		{
//			static_assert(std::is_base_of<ILogger, T>::value, "Logger should inherit from ILogger");
//			if (m_instance) return;
//			m_instance = std::unique_ptr<ILogger>(new T(name));
//		}
//
//	private:
//		static std::unique_ptr<ILogger> m_instance;
//	};
//}
//#if USE_SPDLOG
//
//#if DEBUG
//#define LOG_TRACE(f, ...) udan::debug::Logger::trace_v(f,  __VA_ARGS__)
//#define LOG_DEBUG(f, ...) udan::debug::Logger::debug_v(f,  __VA_ARGS__)
//#define LOG_INFO(f, ...) udan::debug::Logger::info_v(f,  __VA_ARGS__)
//#define LOG_WARN(f, ...) udan::debug::Logger::warn_v(f,  __VA_ARGS__)
//#define LOG_ERR(f, ...) udan::debug::Logger::err_v(f,  __VA_ARGS__)
//#define LOG_FATAL(f, ...) udan::debug::Logger::fatal_v(f,  __VA_ARGS__)
//#define QUIT() __debugbreak()
//#else
//#define LOG_TRACE(f, ...)
//#define LOG_DEBUG(f, ...)
//#define LOG_INFO(f, ...)
//#define LOG_WARN(f, ...)
//#define LOG_ERR(f, ...)
//#define LOG_FATAL(f, ...)
//#define QUIT() abort()
//#endif
//
//#endif
