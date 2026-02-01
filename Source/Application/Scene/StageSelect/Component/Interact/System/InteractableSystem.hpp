#pragma once

#include"System/Entity/System/Base/ISystem.hpp"

namespace Engine::System
{
	class InteractableSystem : public ISystem
	{
	public:
		/// <summary>
		/// ‘JˆÚ”»’è
		/// </summary>
		/// <param name="Reg"></param>
		/// <param name="DeltaTime"></param>
		virtual void MainUpdate(entt::registry& Reg, double DeltaTime) override;
		/// <summary>
		/// ”»’è—p‚Ì‰~‚Ì•`‰æ
		/// </summary>
		/// <param name="Reg"></param>
		virtual void Render(entt::registry& Reg) override;

	private:

	};
}

