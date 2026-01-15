#include "pch.h"
#include "PlayerStateSystem.hpp"
#include "Application/Components/Tag/TagComponent.hpp"
#include"Application/Components/Player/Input/InputRequestComponent.hpp"

void Engine::System::PlayerStateSystem::PreUpdate(entt::registry& Reg, double DeltaTime)
{

}

/// <summary>
/// 状態遷移の判定
/// </summary>
void Engine::System::PlayerStateSystem::MainUpdate(entt::registry& Reg, double DeltaTime)
{
	//	優先度順に並べておきます。
	//	後から実装増やします

	auto view = Reg.view<PlayerStateSystem, InputRequestComponent>(entt::exclude<DeadTag>);

	view.each([&](auto entity, PlayerStateSystem& state, InputRequestComponent& req)
		{
			//	攻撃

			//	走り

			//	ダッシュ

			//	待機
		});
}

/// <summary>
/// 今の状態からアニメーションの設定と更新
/// </summary>
void Engine::System::PlayerStateSystem::PostUpdate(entt::registry& Reg, double DeltaTime)
{

}
