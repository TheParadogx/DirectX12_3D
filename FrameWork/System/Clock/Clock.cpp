#include "pch.h"
#include "Clock.hpp"

Engine::System::Time Engine::System::Clock::Restart()
{
	const auto now = std::chrono::steady_clock::now();
	const auto delta = now - mLast;
	mLast = now;
	return Time(delta);
}
