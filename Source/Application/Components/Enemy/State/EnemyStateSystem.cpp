#include "pch.h"
#include "EnemyStateSystem.hpp"

#include "Application/Components/Enemy/State/EnemyStateComponent.hpp"
#include "Application/Components/Tag/TagComponent.hpp"

#include "System/Conponent/Collider/ColliderComponent.hpp"
#include "System/Conponent/Fbx/FbxMeshConponent.hpp"
#include "System/Conponent/Transform/TransformConponent.hpp"
#include "System/Conponent/Rigidbody/RigidbodyComponent.hpp"

#include "Application/Components/WeaponAttack/WeaponAttackComponent.hpp"

/// <summary>
/// 状態の終了の判定
/// </summary>
/// <param name="Reg"></param>
/// <param name="DeltaTime"></param>
void Engine::System::EnemyStateSystem::PreUpdate(entt::registry& Reg, double DeltaTime)
{

}

/// <summary>
/// 状態の遷移
/// </summary>
/// <param name="Reg"></param>
/// <param name="DeltaTime"></param>
void Engine::System::EnemyStateSystem::MainUpdate(entt::registry& Reg, double DeltaTime)
{
	//	プレイヤーの座標取得
	auto Player_view = Reg.view<Transform3D, PlayerTag>(entt::exclude<DeadTag>);
	Math::Vector3 PlayerPos = {};

	//	プレイヤーの座標
	Player_view.each([&](auto entity, Transform3D& trans)
		{
			PlayerPos = trans.Position;
		});

	auto view = Reg.view<EnemyStateComponent, FbxComponent,Transform3D>(entt::exclude<DeadTag>);
	bool ret = false;

	//	状態遷移
	view.each([&](auto entity, EnemyStateComponent& state, FbxComponent& fbx, Transform3D& trans)
		{
			/*
			* 処理によって行動を変化させる
			*/

			//	距離
			float distance = Math::Vector3::Distance(PlayerPos, trans.Position);

			// 攻撃
			if (distance < state.Chase.AttackRange)
			{
				//	攻撃に移動する処理
				if (state.State != eEnemyState::Attack)
				{
					Math::Vector3 toPlayer = PlayerPos - trans.Position;

					//	方向ベクトルの作成
					Math::Quaternion targetRot = Math::Quaternion::LookRotation(toPlayer);

					//	回転の適応
					trans.Rotation = Math::Quaternion::Slerp(trans.Rotation, targetRot, state.Chase.RotationSpeed * DeltaTime);

					//	状態のリセット関数を呼び出す。
					state.State = eEnemyState::Attack;
					fbx.CurrAnimation = "Attack_D";
					fbx.AnimationScale = 1.0f;

					//	攻撃の当たり判定をつける。(素材が今は一緒なので同じ処理で大丈夫)
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
			//	追跡
			else if (distance < state.Chase.DetectionRange)
			{
				//	移動ベクトル
				Math::Vector3 toPlayer = PlayerPos - trans.Position;

				//	方向ベクトルの作成
				Math::Quaternion targetRot = Math::Quaternion::LookRotation(toPlayer);

				//	回転の適応
				trans.Rotation = Math::Quaternion::Slerp(trans.Rotation, targetRot, state.Chase.RotationSpeed * DeltaTime);
				
				//	移動の取得
				if (Rigidbody3D* rigid = Reg.try_get<Rigidbody3D>(entity))
				{
					//	向きだけを取り出す
					Math::Vector3 moveDir = toPlayer;
					moveDir.Normalize();

					//	移動量セット
					rigid->Velocity = moveDir * state.Chase.MoveSpeed;
				}

				if (state.State != eEnemyState::Chase)
				{
					state.State = eEnemyState::Chase;
					fbx.CurrAnimation = "Jog";
				}
				return;
			}

			//	待機状態
			state.State = eEnemyState::Idle;
			fbx.CurrAnimation = "Idle";

		});

}
