#include "pch.h"
#include "EnemyAISystem.hpp"

#include "Graphics/DebugRender/DebugRender.hpp"
#include"Application/Components/Tag/TagComponent.hpp"
#include"System/Conponent/Collider/ColliderComponent.hpp"
#include"Application/Components/WeaponAttack/WeaponAttackComponent.hpp"

//	攻撃状態に遷移するかどうかの判定
bool Engine::System::EnemyAISystem::CanAttackState(EnemyAIComponent& AI, EnemyParameters& param, float DistanceSQ)
{
	//	攻撃範囲外
	if (param.AttackRange * param.AttackRange < DistanceSQ)
	{
		return false;
	}

	//	前の状態が追跡状態か攻撃状態
	bool CanAttack = AI.CurrState == eEnemyState::Chasing || AI.CurrState == eEnemyState::Attacking;
	if (CanAttack == false)
	{
		return false;
	}

	return true;
}

Engine::System::eEnemyState Engine::System::EnemyAISystem::DetermineNextRequest(EnemyAIComponent& AI, EnemyParameters& Param, float DistanceSQ)
{
	//	赤のキャンセル回避
	if (Param.CanCancelEvade == true)
	{
		//	キャンセル回避判定
	}

	//	待機リクエストがあるかどうか
	if (AI.IsIdle == true)
	{
		//	回避判定


		//	回避しないときは待機状態にする

	}

	//	攻撃判定
	if (CanAttackState(AI, Param, DistanceSQ))
	{
		//	攻撃終了フラグが立っていたら
		if (AI.IsActionFinished == true)
		{
			//	攻撃の段数を増やす（連続攻撃用）
			AI.CurrAttackCombo++;
			if (AI.CurrAttackCombo >= Param.AttackComboMax)
			{
				AI.CurrAttackCombo = 0;
			}
		}
		return eEnemyState::Attacking;
	}


	//	デフォルト：追跡
	return eEnemyState::Chasing;
}

/// <summary>
/// 追跡状態での状態更新
/// </summary>
/// <param name="ToPlayer">プレイヤーとの移動ベクトル</param>
void Engine::System::EnemyAISystem::UpdateChasing(EnemyAIComponent& AI, EnemyParameters& Param, Transform3D& Transform, Rigidbody3D& Rigidbody, const Math::Vector3& ToPlayer, float Distance)
{
	//	近すぎたら何もしない
	if (Distance <= 0.001) return;

	Math::Vector3 dir = ToPlayer;
	dir.y = 0;

	//	移動ベクトルに対してのクォータニオンを作成
	Transform.Rotation = Math::Quaternion::LookRotation(dir, Math::Vector3::Forward);

	dir.Normalize();

	//	移動速度
	Rigidbody.Velocity = dir * Param.MoveSpeed;
}

/// <summary>
/// 攻撃状態でもプレイヤーの方を向く
/// </summary>
/// <param name="AI"></param>
/// <param name="Param"></param>
/// <param name="Transform"></param>
/// <param name="ToPlayer"></param>
/// <param name="Distance"></param>
void Engine::System::EnemyAISystem::UpdateAttacking(EnemyAIComponent& AI, EnemyParameters& Param, Transform3D& Transform, const Math::Vector3& ToPlayer, float Distance)
{
	Math::Vector3 dir = ToPlayer;
	dir.y = 0;

	//	移動ベクトルに対してのクォータニオンを作成
	Transform.Rotation = Math::Quaternion::LookRotation(dir, Math::Vector3::Forward);

}

/// <summary>
///	状態の変更
/// </summary>
/// <param name="AI"></param>
/// <param name="Param"></param>
/// <param name="fbx"></param>
void Engine::System::EnemyAISystem::ChangeState(entt::registry& Reg, EnemyAIComponent& AI, EnemyParameters& Param, FbxComponent& fbx, eEnemyState Next)
{

	//	前状態のリセット
	ExitState(Reg,AI, Param, fbx);

	AI.CurrState = Next;

	//	今の状態の初期化
	InitState(Reg,AI, Param, fbx);

	//	状態フラグを一旦戻す
	AI.IsActionFinished = false;

}

void Engine::System::EnemyAISystem::ExitState(entt::registry& Reg,EnemyAIComponent& AI, EnemyParameters& Param, FbxComponent& fbx)
{
	switch (AI.CurrState)
	{
	case Engine::System::eEnemyState::Idle:
		break;

	case Engine::System::eEnemyState::Chasing:


		break;

	case Engine::System::eEnemyState::Attacking:
		if (AI.IsActionFinished == false)
		{
			AI.CurrAttackCombo = 0;
			//	当たり判定の削除
			if (Reg.all_of<ColliderComponent>(Param.Weapon))
			{
				Reg.remove<ColliderComponent>(Param.Weapon);
			}
			if (Reg.all_of<HitHistoryComponent>(Param.Weapon))
			{
				Reg.remove<HitHistoryComponent>(Param.Weapon);
			}
		}

		break;

	case Engine::System::eEnemyState::Evade:
		break;

	case Engine::System::eEnemyState::CancelEvade:
		break;
	}

}

/// <summary>
/// 状態初期化
/// </summary>
void Engine::System::EnemyAISystem::InitState(entt::registry& Reg,EnemyAIComponent& AI, EnemyParameters& Param, FbxComponent& fbx)
{
	switch (AI.CurrState)
	{
	case Engine::System::eEnemyState::Idle:
		break;

	case Engine::System::eEnemyState::Chasing:
		fbx.CurrAnimation = "Jog";
		fbx.AnimationScale = 1.0f;
		fbx.IsLoop = true;
		break;

	case Engine::System::eEnemyState::Attacking:
		fbx.CurrAnimation = "Attack_" + std::to_string(AI.CurrAttackCombo);
		fbx.AnimationScale = 0.7f;
		fbx.IsLoop = false;
		{
			if (Param.Weapon != entt::null)
			{
				//	武器に必要なものをアタッチ
				Reg.emplace_or_replace<HitHistoryComponent>(Param.Weapon);
				//	武器に当たり判定をアタッチする
				auto col = ColliderComponent::Create<OBBCollider>();
				auto collider = col.GetPtr<OBBCollider>();
				collider->SetVolume({ 1.0f,4.0f,1.0f });
				col.Offset = { 0.0f, -4.0f, 0.0f };
				Reg.emplace_or_replace<ColliderComponent>(Param.Weapon, std::move(col));
			}
		}
		break;

	case Engine::System::eEnemyState::Evade:
		break;

	case Engine::System::eEnemyState::CancelEvade:
		break;
	}
}

/// <summary>
/// 状態の終了判定
/// </summary>
/// <param name="Reg"></param>
/// <param name="DeltaTime"></param>
void Engine::System::EnemyAISystem::PreUpdate(entt::registry& Reg, double DeltaTime)
{
	auto view = Reg.view<EnemyAIComponent, EnemyParameters,FbxComponent>(entt::exclude<DeadTag>);
	view.each([&](auto entity, EnemyAIComponent& AI, EnemyParameters& Param, FbxComponent& fbx)
		{
			switch (AI.CurrState)
			{
			case Engine::System::eEnemyState::Idle:
				break;

			case Engine::System::eEnemyState::Chasing:


				break;

			case Engine::System::eEnemyState::Attacking:
				if (fbx.Mesh->GetAnimationFinish())
				{
					AI.IsActionFinished = true;
				}
				break;

			case Engine::System::eEnemyState::Evade:
				break;

			case Engine::System::eEnemyState::CancelEvade:
				break;
			}


		});


}

void Engine::System::EnemyAISystem::MainUpdate(entt::registry& Reg, double DeltaTime)
{
	auto playerView = Reg.view<Transform3D, PlayerTag>();
	Math::Vector3 playerPos = Reg.get<Transform3D>(playerView.front()).Position;

	auto view = Reg.view<EnemyAIComponent, EnemyParameters, Transform3D, Rigidbody3D,FbxComponent>(entt::exclude<DeadTag>);
	view.each([&](auto entity, EnemyAIComponent& AI, EnemyParameters& Param, Transform3D& trans,Rigidbody3D& rigid, FbxComponent&fbx)
		{
			//	移動ベクトル
			Math::Vector3 Dir =  playerPos - trans.Position;
			float distanceSQ = Dir.SqrLength();

			//	状態の遷移
			auto nextState = DetermineNextRequest(AI, Param, distanceSQ);

			//	状態の切り替え
			if (AI.CurrState != nextState || AI.IsActionFinished == true)
			{
				ChangeState(Reg,AI, Param, fbx, nextState);
			}

			//	その状態での更新
			switch (AI.CurrState)
			{
			case Engine::System::eEnemyState::Idle:
				break;
			case Engine::System::eEnemyState::Chasing:
				UpdateChasing(AI, Param, trans, rigid, Dir, distanceSQ);
				break;
			case Engine::System::eEnemyState::Attacking:
				UpdateAttacking(AI, Param, trans, Dir, distanceSQ);
				break;
			case Engine::System::eEnemyState::Evade:
				break;
			case Engine::System::eEnemyState::CancelEvade:
				break;
			default:
				break;
			}

		});

}

/// <summary>
/// デバック描画
/// </summary>
/// <param name="Reg"></param>
void Engine::System::EnemyAISystem::Render(entt::registry& Reg)
{
	auto view = Reg.view<EnemyParameters, EnemyAIComponent,Transform3D>();
	view.each([&](EnemyParameters& param, EnemyAIComponent& ai, Transform3D& trans)
		{
			//	攻撃に入る範囲の表示
			Graphics::DebugRender::DrawDebugCircle(trans.Position, param.AttackRange, Graphics::Color::Red());

		});
}
