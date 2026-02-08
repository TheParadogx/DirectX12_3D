#pragma once
#include"entt/entt.hpp"

namespace Engine::System
{
	class TimerComponentSystem
	{
	public:
		/// <summary>
		/// 時間経過
		/// </summary>
		/// <param name="DeltaTime"></param>
		static void PreUpdate(entt::registry& Reg,double DeltaTime);

		/// <summary>
		/// タイマーとなるエンティティの作成
		/// </summary>
		/// <returns></returns>
		static entt::entity CreateTimerObject();
	};
}

