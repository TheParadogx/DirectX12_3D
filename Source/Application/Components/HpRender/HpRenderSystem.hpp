#pragma once
#include"System/Entity/System/Base/ISystem.hpp"
#include"Application/Components/HpRender/HpRenderComponent.hpp"


namespace Engine::System
{
	class HpRenderSystem : public ISystem
	{
	public:
		virtual void PostUpdate(entt::registry& Reg, double DeltaTime)override;
		virtual void Render(entt::registry& Reg) override;
	private:
		/// <summary>
		/// ‘Ì—Í‚Ìc—Ê‚ÅF‚ğ•ÏX‚·‚é
		/// </summary>
		void UpdateColor(HpRenderComponent& Component,float Ratio);

	};
}


