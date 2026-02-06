#pragma once
#include"entt/entt.hpp"


namespace Engine::System
{
	/// <summary>
	/// スプライトの発光システム
	/// </summary>
	class GlowSystem
	{
	public:
		static void Update(entt::registry& Reg,float DeltaTime);
	};
}

