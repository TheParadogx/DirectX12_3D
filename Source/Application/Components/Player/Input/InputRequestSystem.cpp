#include "pch.h"
#include "InputRequestSystem.hpp"
#include"InputRequestComponent.hpp"
#include"System/Input/Manager/InputManager.hpp"

#include"Math/Vector2/Vector2.h"

void Engine::System::InputRequestSystem::PreUpdate(entt::registry& Reg, double DeltaTime)
{
	auto view = Reg.view<InputRequestComponent>();
	Math::Vector2 vel = Input::InputManager::GetInstance()->GetAxis();

	view.each([&](InputRequestComponent& req)
		{
			req.InputVec = {vel.x,0.0f,vel.y};
			//	移動
			if (vel.SqrLength() > 0)
			{
				req.Flags |= eActionInputFlags::RunRequested;
			}
			//	してないからビットを降ろす
			else
			{
				req.Flags &= ~eActionInputFlags::RunRequested;
			}
			//	ダッシュ

			//	攻撃

			//	ジャンプ

		});
}
