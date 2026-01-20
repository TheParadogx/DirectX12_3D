#pragma once
#include<Plugin/entt/entt.hpp>

#include"System/Entity/System/Base/ISystem.hpp"

namespace Engine::System
{
	class MoveComponentSystem : public ISystem
	{
	public:
		MoveComponentSystem() = default;

		/// <summary>
		/// ˆÚ“®—Ê‚ÌŒvZˆ—‚ğ‚µ‚ÄRigid‚ÌˆÚ“®—Ê‚É‰ÁZ‚·‚é
		/// </summary>
		virtual void MainUpdate(entt::registry& Reg, double DeltaTime) override;

	private:

	};
}

