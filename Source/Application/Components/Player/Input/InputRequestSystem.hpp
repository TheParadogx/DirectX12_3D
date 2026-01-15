#pragma once
#include"System/Entity/System/Base/ISystem.hpp"

namespace Engine::System
{
	class InputRequestSystem : public ISystem
	{
	public:
		/// <summary>
		/// 入力からリクエストの発生
		/// </summary>
		/// <param name="Reg"></param>
		/// <param name="DeltaTime"></param>
		virtual void PreUpdate(entt::registry& Reg, double DeltaTime)override;
	private:
	};
}

