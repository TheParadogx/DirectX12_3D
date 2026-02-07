#include "pch.h"
#include "PlayerStateSystem.hpp"
#include "Application/Components/Tag/TagComponent.hpp"
#include"Application/Components/InputMove/MoveComponent.hpp"
#include"Application/Components/WeaponAttack/WeaponAttackComponent.hpp"
#include"Application/Components/Invincible/InvincibleTag.hpp"

#include"System/Conponent/Collider/ColliderComponent.hpp"
#include"System/Conponent/Transform/TransformConponent.hpp"

#include"System/Conponent/Effect/EffectComponent.hpp"
#include"Application/Components/Skill/SkillComponent.hpp"
#include"System/Conponent/Effect/Factory/EffectFactory.hpp"

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
		curr == ePlayerState::Run;

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
		curr == ePlayerState::Dodge ||
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
	if (state.Dodge.IsCoolDown == true)
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
			case ePlayerState::Dodge:
				state.State = ePlayerState::Idle;
				fbx.CurrAnimation = "Idle";
				fbx.AnimationScale = 1.0f;
				fbx.IsLoop = true;
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
bool Engine::System::PlayerStateSystem::IsFinishAttack(PlayerStateComponent& State, InputRequestComponent& Req, FbxComponent& Fbx,float DeltaTime)
{
	//	アニメーション終了判定
	if (Fbx.Mesh->GetAnimationFinish() == false)
	{
		return false;
	}

	//	時間経過
	State.Attack.AttackInputTimer += DeltaTime;

	//	受付時間を終了していたら終了
	if (State.Attack.AttackInputTimer <= State.Attack.AttackInputWindow)
	{
		return false;
	}

	return true;
}

/// <summary>
/// 攻撃の終了
/// </summary>
void Engine::System::PlayerStateSystem::OnAttackFinished(entt::registry& Reg,entt::entity entity, PlayerStateComponent& state, InputRequestComponent& req, FbxComponent& fbx)
{
	state.State = ePlayerState::Idle;
	state.Attack.AttackQueued = false;
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
				if (IsFinishAttack(state, req, fbx, DeltaTime))
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
			//	ここだけど状態の自動終了と無敵は分離して考えて、アニメーションの終了とかにするほうがいいかもしれない
			else if (state.State == ePlayerState::Dodge)
			{
				if (fbx.Mesh->GetAnimationFinish() == true)
				{
					ExitState(Reg);
				}
			}

			//	タイマーのカウントダウン
			if (state.Dodge.RecoveryTimer > 0.0f)
			{
				state.Dodge.RecoveryTimer -= DeltaTime;
			}
			//	クールタイム終了判定
			if (state.Dodge.IsCoolDown && state.Dodge.RecoveryTimer <= 0.0f)
			{
				state.Dodge.IsCoolDown = false;
				state.Dodge.DodgeCount = 0;
			}
			//	1回以上で最大数以下の状態で連続判定の時間を過ぎたらカウントをリセットする
			else if (!state.Dodge.IsCoolDown && state.Dodge.RecoveryTimer <= 0.0f)
			{
				state.Dodge.DodgeCount = 0;
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

	auto view = Reg.view<Transform3D,PlayerStateComponent, InputRequestComponent,FbxComponent, MoveComponent>(entt::exclude<DeadTag>);
	bool ret = false;

	view.each([&](auto entity, Transform3D& trans,PlayerStateComponent& state, InputRequestComponent& req, FbxComponent&fbx, MoveComponent& move)
		{

			//	スプリント
			ret = (HasFlag(req.Flags, eActionInputFlags::DodgeRequested) == true)
				&& (this->CheckSprintRequest(state, req.InputVec) == true);
			if (ret == true)
			{
				if (state.State != ePlayerState::Dodge)
				{
					ChangeState(Reg, state, ePlayerState::Dodge);
					fbx.CurrAnimation = "Dodge";
					fbx.AnimationScale = 4.0f;
					fbx.IsLoop = false;
					Reg.emplace_or_replace<InvincibleComponet>(entity);

					//	カウント加算
					state.Dodge.DodgeCount++;
					//	カウントが上限を超えていたら
					if (state.Dodge.DodgeCount >= state.Dodge.DodgeCountMax)
					{
						//	連続使用のペナルティ
						state.Dodge.IsCoolDown = true;
						state.Dodge.RecoveryTimer = state.Dodge.CoolDowmMax;
					}
					else
					{
						state.Dodge.RecoveryTimer = state.Dodge.DodgeInputWindow;
					}
				}
				move.ForceVelocity = trans.GetForward();
				return;

			}

			//	スキル（今は１つだけ）
			bool skillRequested = HasFlag(req.Flags, eActionInputFlags::Skill1Requested);
			if (skillRequested == true && state.State != ePlayerState::Skill)
			{
				//	プレイヤーが持っているスキルのエンティティの取得
				SkillSlotComponent* slots = Reg.try_get<SkillSlotComponent>(entity);
				if (slots != nullptr)
				{
					//	今はスキル番号０を指定します
					entt::entity skillEntity = slots->SkillSlots[0];
					if (Reg.valid(skillEntity) && Reg.all_of<SkillComponent>(skillEntity))
					{
						SkillComponent& skill = Reg.get<SkillComponent>(skillEntity);
						Transform3D& skillTrans = Reg.get<Transform3D>(skillEntity);
						//	クールダウンチェック
						if (skill.CooldownTimer <= 0.0f)
						{
							//	アニメーションあるなら変更する

							//	アクティブのアタッチ
							Reg.emplace_or_replace<ActiveSkillTag>(skillEntity);

							//	当たり判定とヒット履歴のアタッチ
							Reg.emplace_or_replace<HitHistoryComponent>(skillEntity);
							//	ここで当たり判定をアタッチする。
							auto col = ColliderComponent::Create<OBBCollider>();
							auto collider = col.GetPtr<OBBCollider>();
							collider->SetVolume({ 20.0f,0.4f,20.0f });
							Reg.emplace_or_replace<ColliderComponent>(skillEntity, std::move(col));
							skillTrans.Position = trans.Position;

							float scale = 10.0f;
							//	エフェクト生成
							for (auto& asset : skill.Effects)
							{
								auto EffEnt = EffectFactory::CreateAtLocation(asset, skillTrans.Position, { scale,scale,scale }, false);
							}
							//	クールタイムを設定
							skill.CooldownTimer = skill.MaxCooldown;

						}

					}
					else
					{
						//	何もセットされてないときときとかの処理があるなら入れてもいい
					}

				}
				return;

			}

			bool skill2Requested = HasFlag(req.Flags, eActionInputFlags::Skill2Requested);
			if (skill2Requested == true && state.State != ePlayerState::Skill)
			{
				//	プレイヤーが持っているスキルのエンティティの取得
				SkillSlotComponent* slots = Reg.try_get<SkillSlotComponent>(entity);
				if (slots != nullptr)
				{
					//	今はスキル番号１を指定します
					entt::entity skillEntity = slots->SkillSlots[1];
					if (Reg.valid(skillEntity) && Reg.all_of<SkillComponent>(skillEntity))
					{
						SkillComponent& skill = Reg.get<SkillComponent>(skillEntity);
						Transform3D& skillTrans = Reg.get<Transform3D>(skillEntity);
						//	クールダウンチェック
						if (skill.CooldownTimer <= 0.0f)
						{
							//	アニメーションあるなら変更する

							//	アクティブのアタッチ
							Reg.emplace_or_replace<ActiveSkillTag>(skillEntity);

							//	当たり判定とヒット履歴のアタッチ
							Reg.emplace_or_replace<HitHistoryComponent>(skillEntity);
							//	ここで当たり判定をアタッチする。
							auto col = ColliderComponent::Create<OBBCollider>();
							auto collider = col.GetPtr<OBBCollider>();
							collider->SetVolume({ 20.0f,0.4f,20.0f });
							Reg.emplace_or_replace<ColliderComponent>(skillEntity, std::move(col));
							skillTrans.Position = trans.Position;

							float scale = 10.0f;
							//	エフェクト生成
							for (auto& asset : skill.Effects)
							{
								auto EffEnt = EffectFactory::CreateAtLocation(asset, skillTrans.Position, { scale,scale,scale }, false);
							}
							//	クールタイムを設定
							skill.CooldownTimer = skill.MaxCooldown;

						}

					}
					else
					{
						//	何もセットされてないときときとかの処理があるなら入れてもいい
					}

				}
				return;

			}


			// 攻撃ボタンが押されている
			bool isRequested = HasFlag(req.Flags, eActionInputFlags::AttackRequested);

			// 先行入力
			bool canCombo = (state.State == ePlayerState::Attack && state.Attack.AttackQueued);

			//	攻撃
			ret = (isRequested || canCombo);
			if (ret == true)
			{
				// すでに攻撃中の場合はコンボ加算、そうでなければ初期化
				if (state.State == ePlayerState::Attack) 
				{
					//	攻撃中なら次の攻撃にまだ遷移しない
					if (fbx.Mesh->GetAnimationFinish() == false)
					{
						state.Attack.AttackInputTimer = 0.0f;
						state.Attack.AttackQueued = true;
						return;
					}

					//	リセット
					state.Attack.AttackCount++;
					state.Attack.AttackQueued = false;
					//	攻撃数が最大を越したらリセット
					if (state.Attack.AttackCount >= state.Attack.AttackCountMax)
					{
						state.Attack.AttackCount = 0;
					}
				}
				else 
				{
					ChangeState(Reg, state, ePlayerState::Attack);
					state.Attack.AttackCount = 0;
					fbx.IsLoop = false;
					fbx.AnimationScale = 2.2f;
				}

				//	武器に必要なものをアタッチ
				Reg.emplace_or_replace<HitHistoryComponent>(state.Weapon);
				//	ここで当たり判定をアタッチする。
				auto col = ColliderComponent::Create<OBBCollider>();
				auto collider = col.GetPtr<OBBCollider>();
				collider->SetVolume({ 2.0f,5.0f,2.0f });
				col.Offset = { 0.0f, -4.0f, 0.0f };
				Reg.emplace_or_replace<ColliderComponent>(state.Weapon, std::move(col));

				fbx.CurrAnimation = "Attack_" + std::to_string(state.Attack.AttackCount);


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
					fbx.CurrAnimation = "Jog";
				}
				//	移動量の代入
				move.InputDir = req.InputVec;

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
					move.InputDir = Math::Vector3::Zero;
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
