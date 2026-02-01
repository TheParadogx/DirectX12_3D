#include "pch.h"
#include "EnemyAISystem.hpp"

#include "Graphics/DebugRender/DebugRender.hpp"
#include"Application/Components/Tag/TagComponent.hpp"

Engine::System::eEnemyState Engine::System::EnemyAISystem::DetermineNextRequest(EnemyAIComponent& AI, EnemyParameters& Param, float Distance)
{
	//	赤のキャンセル回避
	if (Param.CanCancelEvade == true)
	{
				
	}

	//	待機リクエストがあるかどうか

	//	攻撃判定

	//	回避状態

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
///	状態の変更
/// </summary>
/// <param name="AI"></param>
/// <param name="Param"></param>
/// <param name="fbx"></param>
void Engine::System::EnemyAISystem::ChangeState(EnemyAIComponent& AI, EnemyParameters& Param, FbxComponent& fbx, eEnemyState Next)
{
	//	前状態のリセット
	ExitState(AI, Param, fbx);

	AI.CurrState = Next;

	//	今の状態の初期化
	InitState(AI, Param, fbx);
}

void Engine::System::EnemyAISystem::ExitState(EnemyAIComponent& AI, EnemyParameters& Param, FbxComponent& fbx)
{
}

/// <summary>
/// 状態初期化
/// </summary>
void Engine::System::EnemyAISystem::InitState(EnemyAIComponent& AI, EnemyParameters& Param, FbxComponent& fbx)
{
	switch (AI.CurrState)
	{
	case Engine::System::eEnemyState::Idle:
		break;

	case Engine::System::eEnemyState::Chasing:
		fbx.CurrAnimation = "Jog";
		fbx.AnimationScale = 1.0f;
		break;

	case Engine::System::eEnemyState::Attacking:
		break;

	case Engine::System::eEnemyState::Evade:
		break;

	case Engine::System::eEnemyState::CancelEvade:
		break;
	}
}

void Engine::System::EnemyAISystem::PreUpdate(entt::registry& Reg, double DeltaTime)
{

	
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
			float distance = Dir.SqrLength();

			//	状態の遷移
			auto nextState = DetermineNextRequest(AI, Param, distance);

			//	状態の切り替え
			if (AI.CurrState != nextState)
			{
				ChangeState(AI, Param, fbx, nextState);
			}

			//	その状態での更新
			switch (AI.CurrState)
			{
			case Engine::System::eEnemyState::Idle:
				break;
			case Engine::System::eEnemyState::Chasing:
				UpdateChasing(AI, Param, trans, rigid, Dir, distance);
				break;
			case Engine::System::eEnemyState::Attacking:
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
