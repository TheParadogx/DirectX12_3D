#include "pch.h"
#include "InvincibleSystem.hpp"
#include"InvincibleTag.hpp"

void Engine::System::InvincibleSystem::PostUpdate(entt::registry& Reg, double DeltaTime)
{
	auto view = Reg.view<InvincibleComponet>();
	view.each([&](entt::entity entity, InvincibleComponet& invi)
		{
			invi.InviTimer += DeltaTime;
			if (invi.InviMax <= invi.InviTimer)
			{
				//	–³“G‚Ìíœ
				Reg.remove<InvincibleComponet>(entity);
			}

		});

}
