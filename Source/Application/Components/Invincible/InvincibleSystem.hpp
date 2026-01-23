#pragma once
#include"System/Entity/System/Base/ISystem.hpp"
namespace Engine::System
{
	class InvincibleSystem : public ISystem
	{
	public:
		/// <summary>
		/// –³“GŠÔ‚ÌŒ¸­
		/// </summary>
		/// <param name="Reg"></param>
		/// <param name="DeltaTime"></param>
		void MainUpdate(entt::registry& Reg, double DeltaTime)override;
	private:

	};
}

