#include "pch.h"
#include "PlayerStateSystem.hpp"
#include "Application/Components/Tag/TagComponent.hpp"
#include"Application/Components/Player/Input/InputRequestComponent.hpp"
#include"Application/Components/Player/PlayerState/PlayerStateComponent.hpp"
#include"System/Conponent/Fbx/FbxMeshConponent.hpp"
#include"Application/Components/InputMove/MoveComponent.hpp"

bool Engine::System::PlayerStateSystem::CheckRunRequest()
{
	return true;
}

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

	auto view = Reg.view<PlayerStateComponent, InputRequestComponent,FbxComponent, MoveComponent>(entt::exclude<DeadTag>);
	bool ret = false;

	view.each([&](auto entity, PlayerStateComponent& state, InputRequestComponent& req, FbxComponent&fbx, MoveComponent& move)
		{
			//	攻撃

			//	走り
			//	走るリクエストがあるときに
			ret = (HasFlag(req.Flags,eActionInputFlags::RunRequested) == true) && (this->CheckRunRequest() == true);
			if (ret == true)
			{
				if (state.State != ePlayerState::Run)
				{
					state.State = ePlayerState::Run;
					//	ここに切り替え時に1回だけ通したい処理などを入れてもいい
					fbx.CurrAnimation = "Run";
				}
				//	移動量の代入
				move.TargetDir = req.InputVec;

				return;
			}

			//	ダッシュ

			//	待機
			//	この地点で何も通いないなら
			if (state.State != ePlayerState::Idle)
			{
				state.State = ePlayerState::Idle;
				fbx.CurrAnimation = "Idle";
				move.TargetDir = Math::Vector3::Zero;
			}


		});
}

/// <summary>
/// 今の状態からアニメーションの設定と更新
/// </summary>
void Engine::System::PlayerStateSystem::PostUpdate(entt::registry& Reg, double DeltaTime)
{

}
