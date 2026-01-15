#pragma once
#include"System/Entity/System/Base/ISystem.hpp"

namespace Engine::System
{
	class CameraControlSystem : public ISystem
	{
	public:
		virtual void MainUpdate(entt::registry& Reg, double DeltaTime) override;
	private:

	};
}