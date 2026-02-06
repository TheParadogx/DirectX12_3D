#pragma once
#include"entt/entt.hpp"

namespace Engine::System
{
	class EffectSystem
	{
	public:
		/// <summary>
		/// トランスとの座標、回転同期
		/// </summary>
		static void PostUpdate(entt::registry& Reg);

	};
}


