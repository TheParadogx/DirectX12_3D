#include "pch.h"
#include "InvincibleSystem.hpp"
#include"InvincibleTag.hpp"
#include"Application/Components/Player/Input/InputRequestComponent.hpp"

void Engine::System::InvincibleSystem::MainUpdate(entt::registry& Reg, double DeltaTime)
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
