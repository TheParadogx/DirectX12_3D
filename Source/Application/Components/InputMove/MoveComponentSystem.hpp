#pragma once
#include<Plugin/entt/entt.hpp>

#include"System/Entity/System/Base/ISystem.hpp"

namespace Engine::System
{
	class MoveComponentSystem : public ISystem
	{
	public:
		MoveComponentSystem() = default;
		//	ˆÚ“®—Ê‚ÌƒZƒbƒg
		virtual void PreUpdate(entt::registry& Reg, double DeltaTime)override;

		//	“ü—ÍŽæ“¾
		virtual void MainUpdate(entt::registry& Reg, double DeltaTime) override;

	private:

	};
}

