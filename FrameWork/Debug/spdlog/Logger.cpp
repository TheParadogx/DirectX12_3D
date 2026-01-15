#include "pch.h"
#include "Logger.hpp"

namespace Engine::Debug
{
	std::shared_ptr<spdlog::logger> Logger::sLogger = nullptr;
	FILE* Logger::sConOutFP = nullptr;
	FILE* Logger::sConInFP = nullptr;
}

/// <summary>
/// コンソールの初期化
/// </summary>
void Engine::Debug::Logger::InitializeConsole()
{
#if _DEBUG
	//	コンソール画面の作成
	AllocConsole();
	//	コンソールとcout
	freopen_s(&sConOutFP, "CONOUT$", "w", stdout);
	freopen_s(&sConInFP, "CONIN$", "r", stdin);
#endif // _DEBUG

}

void Engine::Debug::Logger::InitializeSpdlog()
{
	const std::string logDir = "Log";
	const std::string logFile = logDir + "/engine.log";

	//	ディレクトリの存在判定
	if (std::filesystem::exists(logDir) == false)
	{
		//	なければ作る
		std::filesystem::create_directory(logDir);
	}

#if _DEBUG
	//	存在するときはリセットする
	if (std::filesystem::exists(logFile))
	{
		std::filesystem::remove(logFile);
	}
#endif // _DEBUG

	//	ログファイルのサイズが越えたらローテーションする
	auto fileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
		logFile,	//	ログファイル名
		1024 * 1024 * 5,			//	5MB
		3							//	世代数
	);

	//	コンソールの色付きシンク
	auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

	//	表示する形式のパターン
	std::string pattern =
		"[%Y-%m-%d %H:%M:%S.%e] "
		"[%^%l%$] "
		"[%s:%# %!] "
		"%v";

	// 通常ログ用（時間＋レベル＋メッセージ）
	consoleSink->set_pattern(pattern);
	fileSink->set_pattern(pattern);

	std::vector<spdlog::sink_ptr> sinks{ fileSink, consoleSink };
	sLogger = std::make_shared<spdlog::logger>("Engine", sinks.begin(), sinks.end());

	spdlog::set_default_logger(sLogger);
	spdlog::set_level(spdlog::level::debug);
	spdlog::flush_on(spdlog::level::err);

	Logger::Info("Logger initialized");

}

Engine::Debug::Logger::~Logger()
{
	this->Release();
}

void Engine::Debug::Logger::Initialize()
{
	//	コンソール初期化
	Engine::Debug::Logger::InitializeConsole();

	//	spdlog初期化
	Engine::Debug::Logger::InitializeSpdlog();
}

void Engine::Debug::Logger::Release()
{
#if _DEBUG
	FreeConsole();
#endif // _DEBUG
}
