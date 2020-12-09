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
		static void Init();
		static std::shared_ptr<spdlog::logger> Instance()
		{
			std::lock_guard<std::mutex> lck(m_mtx);
			if (m_logger == nullptr)
				Init();
			return  m_logger;
		}
#if PRINT_LINE
		template<typename... Args>
		static void trace(const char* functionName, const char* file, int line, Args&&... args)
		{
			Instance()->trace("[{}] {} ({}:{})", functionName, fmt::format(std::forward<Args>(args)...), file, line);
		}
		template<typename... Args>
		static void debug(const char* functionName, const char* file, int line, Args&&... args)
		{
			Instance()->debug("[{}] {} ({}:{})", functionName, fmt::format(std::forward<Args>(args)...), file, line);
		}
		template<typename... Args>
		static void info(const char* functionName, const char* file, int line, Args&&... args)
		{
			Instance()->info("[{}] {} ({}:{})", functionName, fmt::format(std::forward<Args>(args)...), file, line);
		}
		template<typename... Args>
		static void warn(const char* functionName, const char* file, int line, Args&&... args)
		{
			Instance()->warn("[{}] {} ({}:{})", functionName, fmt::format(std::forward<Args>(args)...), file, line);
		}
		template <typename ... Args>
		static void err(const char* functionName, const char* file, int line, Args&&... args)
		{
			Instance()->error("[{}] {} ({}:{})", functionName, fmt::format(std::forward<Args>(args)...), file, line);
		}
#else
		template<typename... Args>
		static void trace(const char *functionName, Args&&... args)
		{
			Instance()->trace("[{}] {}", functionName, fmt::format(std::forward<Args>(args)...));
		}
		template<typename... Args>
		static void debug(const char* functionName, Args&&... args)
		{
			Instance()->debug("[{}] {}", functionName, fmt::format(std::forward<Args>(args)...));
		}
		template<typename... Args>
		static void info(const char* functionName, Args&&... args)
		{
			Instance()->info("[{}] {}", functionName, fmt::format(std::forward<Args>(args)...));
		}
		template<typename... Args>
		static void warn(const char* functionName, Args&&... args)
		{
			Instance()->warn("[{}] {}", functionName, fmt::format(std::forward<Args>(args)...));
		}
		template <typename ... Args>
		static void err(const char* functionName, Args&&... args)
		{
			Instance()->error("[{}] {}", functionName, fmt::format(std::forward<Args>(args)...));
		}
#endif
		template<typename... Args>
		static void fatal(const char* functionName, const char* file, int line, Args&&... args)
		{
			Instance()->critical("[{}] {} ({}:{})", functionName, fmt::format(std::forward<Args>(args)...), file, line);
#if DEBUG
			__debugbreak();
#else
			abort();
#endif
		}
	private:
		static std::shared_ptr<spdlog::logger> m_logger;
		static std::mutex m_mtx;
	};
}

#ifdef DEBUG
#if PRINT_LINE
#define LOG_TRACE(...) udan::debug::Logger::trace(__FUNCTION__, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_DEBUG(...) udan::debug::Logger::debug(__FUNCTION__, __FILE__, __LINE__,  __VA_ARGS__)
#define LOG_INFO(...) udan::debug::Logger::info(__FUNCTION__, __FILE__, __LINE__,  __VA_ARGS__)
#define LOG_WARN(...) udan::debug::Logger::warn(__FUNCTION__, __FILE__, __LINE__,  __VA_ARGS__)
#define LOG_ERR(...) udan::debug::Logger::err(__FUNCTION__, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_FATAL(...) udan::debug::Logger::fatal(__FUNCTION__, __FILE__, __LINE__,  __VA_ARGS__)
#else
#define LOG_TRACE(...) udan::debug::Logger::trace(__FUNCTION__, __VA_ARGS__)
#define LOG_DEBUG(...) udan::debug::Logger::debug(__FUNCTION__, __VA_ARGS__)
#define LOG_INFO(...) udan::debug::Logger::info(__FUNCTION__, __VA_ARGS__)
#define LOG_WARN(...) udan::debug::Logger::warn(__FUNCTION__, __VA_ARGS__)
#define LOG_ERR(...) udan::debug::Logger::err(__FUNCTION__, __VA_ARGS__)
#define LOG_FATAL(...) udan::debug::Logger::fatal(__FUNCTION__, __FILE__, __LINE__, __VA_ARGS__)
#endif
#else
#define LOG_TRACE(...)
#define LOG_DEBUG(...)
#define LOG_INFO(...)
#define LOG_WARN(...)
#define LOG_ERR(...)
#define LOG_FATAL(...) udan::debug::Logger::fatal(__FILE__, __LINE__,  __FUNCTION__, __VA_ARGS__)
#endif

#endif
