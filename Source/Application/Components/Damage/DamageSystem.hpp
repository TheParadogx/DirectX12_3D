#pragma once
#include"System/Entity/System/Base/ISystem.hpp"

namespace Engine::System
{
	class DamageSystem : public ISystem
	{
	public:
		void MainUpdate(entt::registry& Reg, double DeltaTime) {}

		/// <summary>
		/// ƒ_ƒ[ƒWˆ—
		/// </summary>
		/// <param name="Reg"></param>
		/// <param name="DeltaTime"></param>
		void PostUpdate(entt::registry& Reg, double DeltaTime)override;
	private:


	};
}