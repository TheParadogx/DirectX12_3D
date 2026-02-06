#pragma once
#include"System/Entity/System/Base/ISystem.hpp"
namespace Engine::System
{
	class GameRuleSystem : public ISystem
	{
	public:

		/// <summary>
		/// Game‚ÌI—¹ğŒ‚ğŒ©‚é
		/// </summary>
		/// <param name="Reg"></param>
		/// <param name="DeltaTime"></param>
		void PostUpdate(entt::registry& Reg, double DeltaTime)override;

	private:
		bool mIsStateChange = false;
	};
}


