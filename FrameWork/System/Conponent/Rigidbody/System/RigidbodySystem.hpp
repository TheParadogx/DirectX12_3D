#pragma once

#include<Plugin/entt/entt.hpp>

namespace Engine::System
{
	class RigidBodySystem
	{
	public:
		/// <summary>
		/// 重力と座標更新
		/// </summary>
		/// <param name="Reg"></param>
		/// <param name="FixedDeltaTime"></param>
		static void MainUpdate(entt::registry& Reg, double FixedDeltaTime);

	};
}