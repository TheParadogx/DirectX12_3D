#pragma once
#include<Plugin/spdlog/spdlog.h>
#include<Plugin/spdlog/sinks/rotating_file_sink.h>
#include<Plugin/spdlog/sinks/stdout_color_sinks.h>

#include<string_view>
#include<memory>
#include<stdio.h>
#include <stdint.h>

namespace Engine::Debug
{
	/// <summary>
	/// spdlogをラップしたクラス
	/// </summary>
	class Logger
	{
		/// <summary>
		/// コンソールの初期化
		/// </summary>
		static void InitializeConsole();

		/// <summary>
		/// spdlogの初期化
		/// </summary>
		static void InitializeSpdlog();
	public:

		virtual ~Logger();

		/// <summary>
		/// 初期化
		/// </summary>
		static void Initialize();

		/// <summary>
		/// 解放
		/// </summary>
		static void Release();

		template<typename... Args>
		static void Info(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
			sLogger->info(fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		static void Warn(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
			sLogger->warn(fmt, std::forward<Args>(args)...);
		}

		template<typename... Args>
		static void Debug(spdlog::format_string_t<Args...> fmt, Args&&... args)
		{
#if _DEBUG
			sLogger->debug(fmt, std::forward<Args>(args)...);
#endif
		}


		template<typename... Args>
		static void Error(
			const spdlog::source_loc& loc,
			std::string_view fmt,
			Args&&... args)
		{
			sLogger->log(loc, spdlog::level::err, fmt, std::forward<Args>(args)...);
		}


		template<typename... Args>
		static void Critical(
			const spdlog::source_loc& loc,
			std::string_view fmt,
			Args&&... args)
		{
			sLogger->log(loc, spdlog::level::critical, fmt, std::forward<Args>(args)...);
			DebugBreak();
		}

	private:
		static std::shared_ptr<spdlog::logger> sLogger;
		static FILE* sConOutFP;
		static FILE* sConInFP;
	};
}


#define LOG_INFO(...)     Engine::Debug::Logger::Info(__VA_ARGS__)
#define LOG_WARN(...)     Engine::Debug::Logger::Warn(__VA_ARGS__)
#define LOG_DEBUG(...)    Engine::Debug::Logger::Debug(__VA_ARGS__)

#define LOG_ERROR(...) \
    Engine::Debug::Logger::Error({ __FILE__, __LINE__, SPDLOG_FUNCTION }, __VA_ARGS__)

#define LOG_CRITICAL(...) \
    Engine::Debug::Logger::Critical({ __FILE__, __LINE__, SPDLOG_FUNCTION }, __VA_ARGS__)
