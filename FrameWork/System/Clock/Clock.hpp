#pragma once
#include"System/Time/Time.hpp"

namespace Engine::System
{
	class Clock
	{
	public:
		/// <summary>
		/// 前回呼び出しからの経過時間を返し、基準点を更新する
		/// </summary>
		/// <returns></returns>
		[[nodiscard]] Time Restart();
	private:
		std::chrono::steady_clock::time_point mLast;
	};
}