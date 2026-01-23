#include "pch.h"
#include "PlayerStateSystem.hpp"
#include "Application/Components/Tag/TagComponent.hpp"
#include"Application/Components/InputMove/MoveComponent.hpp"
#include"Application/Components/WeaponAttack/WeaponAttackComponent.hpp"
#include"Application/Components/Invincible/InvincibleTag.hpp"

#include"System/Conponent/Collider/ColliderComponent.hpp"

/// <summary>
/// 走り状態に移行できるかどうかの判定
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
		curr == ePlayerState::Sprint;

}

/// <summary>
/// 攻撃状態に移行できるかどうかの判定
/// </summary>
/// <returns></returns>
bool Engine::System::PlayerStateSystem::CheckAttackRequest(PlayerStateComponent& state)
{
	const auto& curr = state.State;
	return
		curr == ePlayerState::Idle ||
		curr == ePlayerState::Run ||
		curr == ePlayerState::Sprint ||
		curr == ePlayerState::Attack;
}

/// <summary>
/// スプリント状態に移行できるかどうかの判定
/// </summary>
/// <param name="state"></param>
/// <returns></returns>
bool Engine::System::PlayerStateSystem::CheckSprintRequest(PlayerStateComponent& state, const Math::Vector3& InputVec)
{
	const auto& curr = state.State;

	//	入力量がないとだめ
	if (InputVec.SqrLength() <= 0)
	{
		return false;
	}

	//	クールダウン中はだめ
	if (state.Sprint.IsCoolDown == true)
	{
		return false;
	}

	//	後は条件次第
	return 
		curr == ePlayerState::Idle ||
		curr == ePlayerState::Run ||
		curr == ePlayerState::Attack;
}

/// <summary>
/// 状態の終了
/// </summary>
/// <param name="Reg"></param>
void Engine::System::PlayerStateSystem::ExitState(entt::registry& Reg)
{
	auto view = Reg.view<PlayerStateComponent, InputRequestComponent, FbxComponent>();
	view.each([&](auto entity, PlayerStateComponent& state, InputRequestComponent& req, FbxComponent& fbx)
		{
			//	攻撃
			switch (state.State)
			{
			case ePlayerState::Attack:
				OnAttackFinished(Reg, entity, state, req, fbx);
				break;
			case ePlayerState::Run:
				state.State = ePlayerState::Idle;
				fbx.CurrAnimation = "Idle";
				fbx.IsLoop = true;
				break;
			case ePlayerState::Sprint:
				state.State = ePlayerState::Idle;
				fbx.CurrAnimation = "Idle";
				break;
			default:
				break;
			}


		});

}

/// <summary>
/// 状態の切り替え
/// </summary>
void Engine::System::PlayerStateSystem::ChangeState(entt::registry& Reg, PlayerStateComponent& state, ePlayerState NextState)
{
	ExitState(Reg);
	state.State = NextState;
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
/// 攻撃の終了
/// </summary>
void Engine::System::PlayerStateSystem::OnAttackFinished(entt::registry& Reg,entt::entity entity, PlayerStateComponent& state, InputRequestComponent& req, FbxComponent& fbx)
{
	state.State = ePlayerState::Idle;
	fbx.CurrAnimation = "Idle";
	fbx.IsLoop = true;
	fbx.AnimationScale = 1.0f;

	//	当たり判定の削除
	if (Reg.all_of<ColliderComponent>(state.Weapon))
	{
		Reg.remove<ColliderComponent>(state.Weapon);
	}
	if (Reg.all_of<HitHistoryComponent>(state.Weapon))
	{
		Reg.remove<HitHistoryComponent>(state.Weapon);
	}
}

/// <summary>
/// 終了条件を判断するして前状態を終了する
/// </summary>
void Engine::System::PlayerStateSystem::PreUpdate(entt::registry& Reg, double DeltaTime)
{
	//	攻撃の終了などを判断する
	auto view = Reg.view<PlayerStateComponent, InputRequestComponent, FbxComponent>();
	view.each([&](auto entity, PlayerStateComponent& state, InputRequestComponent& req, FbxComponent& fbx)
		{
			//	攻撃
			if (state.State == ePlayerState::Attack)
			{
				//	攻撃の終了判定
				if (IsFinishAttack(state, req, fbx))
				{
					ExitState(Reg);
				}
			}
			//	ダッシュ
			else if (state.State == ePlayerState::Run)
			{
				if (req.InputVec.SqrLength() <= 0)
				{
					ExitState(Reg);
				}
			}
			else if (state.State == ePlayerState::Sprint)
			{
				//	スプリントのリクエストがない
				if (HasFlag(req.Flags, eActionInputFlags::SprintRequested) == false)
				{
					return false;
				}
				ExitState(Reg);
			}

			//	タイマーのカウントダウン
			if (state.Sprint.RecoveryTimer > 0.0f)
			{
				state.Sprint.RecoveryTimer -= DeltaTime;
			}
			//	クールタイム終了判定
			if (state.Sprint.IsCoolDown && state.Sprint.RecoveryTimer <= 0.0f)
			{
				state.Sprint.IsCoolDown = false;
				state.Sprint.DodgeCount = 0;
				//state.Sprint.RecoveryTimer = 0;
			}
			//	1回以上で最大数以下の状態で連続判定の時間を過ぎたらカウントをリセットする
			else if (!state.Sprint.IsCoolDown && state.Sprint.RecoveryTimer <= 0.0f)
			{
				state.Sprint.DodgeCount = 0;
				//state.Sprint.RecoveryTimer = 0;
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

			//	スプリント
			ret = (HasFlag(req.Flags, eActionInputFlags::SprintRequested) == true)
				&& (this->CheckSprintRequest(state, req.InputVec) == true);
			if (ret == true)
			{
				if (state.State != ePlayerState::Sprint)
				{
					ChangeState(Reg, state, ePlayerState::Sprint);
					fbx.CurrAnimation = "Sprint";
					Reg.emplace_or_replace<InvincibleComponet>(entity);

					//	カウント加算
					state.Sprint.DodgeCount++;
					//	カウントが上限を超えていたら
					if (state.Sprint.DodgeCount >= state.Sprint.DodgeCountMax)
					{
						//	連続使用のペナルティ
						state.Sprint.IsCoolDown = true;
						state.Sprint.RecoveryTimer = state.Sprint.CoolDowmMax;
					}
					else
					{
						state.Sprint.RecoveryTimer = state.Sprint.DodgeInputWindow;
					}
				}
				//	移動量の代入
				move.TargetDir = req.InputVec * 10;
				return;
			}

			//	攻撃
			ret = (HasFlag(req.Flags, eActionInputFlags::AttackRequested) == true) && (this->CheckAttackRequest(state) == true);
			if (ret == true)
			{
				if (state.State != ePlayerState::Attack)
				{
					//	状態変更
					ChangeState(Reg, state, ePlayerState::Attack);
					fbx.CurrAnimation = "Attack";
					fbx.IsLoop = false;
					fbx.AnimationScale = 2.5f;
					//fbx.AnimationScale = 0.2f;

					//	武器に必要なものをアタッチ
					Reg.emplace_or_replace<HitHistoryComponent>(state.Weapon);
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
