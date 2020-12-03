#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#if PRINT_LINE
#include <spdlog/fmt/fmt.h>
#endif

namespace udan::debug
{
#if USE_SPDLOG
	class Logger
	{
	public:
		static void init();
		static std::shared_ptr<spdlog::logger> Instance() { return  m_logger; }
#if PRINT_LINE
		template<typename... Args>
		static void trace(const char* file, int line, Args&&... args)
		{
			m_logger->trace("{} ({}:{})", fmt::format(std::forward<Args>(args)...), file, line);
		}
		template<typename... Args>
		static void debug(const char* file, int line, Args&&... args)
		{
			m_logger->debug("{} ({}:{})", fmt::format(std::forward<Args>(args)...), file, line);
		}
		template<typename... Args>
		static void info(const char* file, int line, Args&&... args)
		{
			m_logger->info("{} ({}:{})", fmt::format(std::forward<Args>(args)...), file, line);
		}
		template<typename... Args>
		static void warn(const char* file, int line, Args&&... args)
		{
			m_logger->warn("{} ({}:{})", fmt::format(std::forward<Args>(args)...), file, line);
		}
		template <typename ... Args>
		static void err(const char* file, int line, Args&&... args)
		{
			m_logger->error("{} ({}:{})", fmt::format(std::forward<Args>(args)...), file, line);
		}
#else
		template<typename... Args>
		static void trace(Args&&... args)
		{
			m_logger->trace(std::forward<Args>(args)...);
		}
		template<typename... Args>
		static void debug(Args&&... args)
		{
			m_logger->debug(std::forward<Args>(args)...);
		}
		template<typename... Args>
		static void info(Args&&... args)
		{
			m_logger->info(std::forward<Args>(args)...);
		}
		template<typename... Args>
		static void warn(Args&&... args)
		{
			m_logger->warn(std::forward<Args>(args)...);
		}
		template <typename ... Args>
		static void err(Args&&... args)
		{
			m_logger->error(std::forward<Args>(args)...);
		}
#endif
		template<typename... Args>
		static void fatal(const char* file, int line, Args&&... args)
		{
			m_logger->critical("{} ({}:{})", fmt::format(std::forward<Args>(args)...), file, line);
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

#ifdef DEBUG
#if PRINT_LINE
#define LOG_TRACE(...) udan::debug::Logger::trace(__FILE__, __LINE__, __VA_ARGS__)
#define LOG_DEBUG(...) udan::debug::Logger::debug(__FILE__, __LINE__,  __VA_ARGS__)
#define LOG_INFO(...) udan::debug::Logger::info(__FILE__, __LINE__,  __VA_ARGS__)
#define LOG_WARN(...) udan::debug::Logger::warn(__FILE__, __LINE__,  __VA_ARGS__)
#define LOG_ERR(...) udan::debug::Logger::err(__FILE__, __LINE__,  __VA_ARGS__)
#define LOG_FATAL(...) udan::debug::Logger::fatal(__FILE__, __LINE__,  __VA_ARGS__)
#else
#define LOG_TRACE(...) udan::debug::Logger::trace(__VA_ARGS__)
#define LOG_DEBUG(...) udan::debug::Logger::debug(__VA_ARGS__)
#define LOG_INFO(...) udan::debug::Logger::info(__VA_ARGS__)
#define LOG_WARN(...) udan::debug::Logger::warn(__VA_ARGS__)
#define LOG_ERR(...) udan::debug::Logger::err(__VA_ARGS__)
#define LOG_FATAL(...) udan::debug::Logger::fatal(__FILE__, __LINE__,  __VA_ARGS__)
#endif
#else
#define LOG_TRACE(...)
#define LOG_DEBUG(...)
#define LOG_INFO(...)
#define LOG_WARN(...)
#define LOG_ERR(...)
#define LOG_FATAL(...) udan::debug::Logger::fatal(__FILE__, __LINE__,  __VA_ARGS__)
#endif

#endif
