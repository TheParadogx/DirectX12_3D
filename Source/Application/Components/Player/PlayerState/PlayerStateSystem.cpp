#include "pch.h"
#include "PlayerStateSystem.hpp"
#include "Application/Components/Tag/TagComponent.hpp"
#include"Application/Components/InputMove/MoveComponent.hpp"
#include"Application/Components/WeaponAttack/WeaponAttackComponent.hpp"

#include"System/Conponent/Collider/ColliderComponent.hpp"

/// <summary>
/// 走り状態に移行できるかどうかの判定フラグ
/// </summary>
/// <returns></returns>
bool Engine::System::PlayerStateSystem::CheckRunRequest(PlayerStateComponent& state)
{
	//	歩き状態かスプリント状態かダッシュ状態ならtrue
	//	any_ofより処理速度が速いので可読性そんなに変わらないからこれで。
	const auto& curr = state.State;
	return  
		curr == ePlayerState::Idle	||
		curr == ePlayerState::Run	||
		curr == ePlayerState::Dash;

}

/// <summary>
/// 攻撃状態に移行できるかどうかの判定フラグ
/// </summary>
/// <returns></returns>
bool Engine::System::PlayerStateSystem::CheckAttackRequest(PlayerStateComponent& state)
{
	const auto& curr = state.State;
	return
		curr == ePlayerState::Idle ||
		curr == ePlayerState::Run ||
		curr == ePlayerState::Dash ||
		curr == ePlayerState::Attack;
}

/// <summary>
/// 攻撃の終了条件の判定
/// </summary>
/// <returns>true:終了</returns>
bool Engine::System::PlayerStateSystem::IsFinishAttack(PlayerStateComponent& State, InputRequestComponent& Req, FbxComponent& Fbx)
{
	//	アニメーション終了判定
	if (Fbx.Mesh->GetAnimationFinish() == false)
	{
		return false;
	}

	//	リクエストが次も攻撃かどうか
	//if (HasFlag(Req.Flags, eActionInputFlags::AttackRequested) == true)
	//{
	//	return false;
	//}

	//	スタミナを追加したらここでスタミナの残量判定をする

	//	あとはコンボ数の上限とか

	return true;
}

/// <summary>
/// 終了条件を判断するして前状態を終了する
/// </summary>
void Engine::System::PlayerStateSystem::PreUpdate(entt::registry& Reg, double DeltaTime)
{
	//	攻撃の終了などを判断する
	auto view = Reg.view<PlayerStateComponent, InputRequestComponent,FbxComponent>();
	view.each([&](auto entity, PlayerStateComponent& state, InputRequestComponent& req, FbxComponent& fbx)
	{
			//	攻撃
			if (state.State == ePlayerState::Attack)
			{
				//	攻撃の終了判定
				if (IsFinishAttack(state, req, fbx))
				{
					state.State = ePlayerState::Idle;
					fbx.CurrAnimation = "Idle";
					fbx.IsLoop = true;
					fbx.AnimationScale = 1.0f;
					//	当たり判定の削除
					Reg.remove<ColliderComponent>(state.Weapon);
				}
			}

			//	ダッシュ
			if (state.State == ePlayerState::Run)
			{
				if (req.InputVec.SqrLength() <= 0)
				{
					state.State = ePlayerState::Idle;
					fbx.CurrAnimation = "Idle";
					fbx.IsLoop = true;
				}
			}

	});
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
			ret = (HasFlag(req.Flags, eActionInputFlags::AttackRequested) == true) && (this->CheckAttackRequest(state) == true);
			if (ret == true)
			{
				if (state.State != ePlayerState::Attack)
				{
					//	状態変更
					state.State = ePlayerState::Attack;
					fbx.CurrAnimation = "Attack";
					fbx.IsLoop = false;
					//fbx.AnimationScale = 2.5f;
					fbx.AnimationScale = 0.2f;

					//	武器に必要なものをアタッチ
					Reg.emplace_or_replace<WeaponAttackComponent>(state.Weapon);
					//	ここで当たり判定をアタッチする。
					auto col = ColliderComponent::Create<OBBCollider>();
					auto collider = col.GetPtr<OBBCollider>();
					collider->SetVolume({ 1.0f,4.0f,1.0f });
					col.Offset = { 0.0f, -4.0f, 0.0f };
					Reg.emplace_or_replace<ColliderComponent>(state.Weapon, std::move(col));

				}
				return;
			}

			//	走り
			//	走るリクエストがあるときに
			ret = (HasFlag(req.Flags,eActionInputFlags::RunRequested) == true) && (this->CheckRunRequest(state) == true);
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
			if (HasNoFlag(req.Flags) && state.State == ePlayerState::Idle)
			{
				//	この地点で何も通いないなら
				if (state.State != ePlayerState::Idle)
				{
					state.State = ePlayerState::Idle;
					fbx.CurrAnimation = "Idle";
					move.TargetDir = Math::Vector3::Zero;
				}
			}


		});
}

/// <summary>
/// 今の状態からアニメーションの設定と更新
/// </summary>
void Engine::System::PlayerStateSystem::PostUpdate(entt::registry& Reg, double DeltaTime)
{

}
