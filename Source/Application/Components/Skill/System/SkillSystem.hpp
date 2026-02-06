#pragma once
#include"entt/entt.hpp"
#include"System/Entity/System/Base/ISystem.hpp"

namespace Engine::System
{
	class SkillSystem : public ISystem
	{
	public:
		/// <summary>
		/// クールタイムや終了判定
		/// </summary>
		/// <param name="Reg"></param>
		/// <param name="DeltaTime"></param>
		void MainUpdate(entt::registry& Reg, double DeltaTime)override;

	};
}

