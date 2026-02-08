#pragma once
#include"entt/entt.hpp"

namespace Engine::System
{
	class TimerComponentSystem
	{
	public:
		/// <summary>
		/// ŽžŠÔŒo‰ß
		/// </summary>
		/// <param name="DeltaTime"></param>
		static void PreUpdate(entt::registry& Reg,double DeltaTime);
	};
}

