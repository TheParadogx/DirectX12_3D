#include "pch.h"
#include "EnemyStateSystem.hpp"

#include "Application/Components/Enemy/State/EnemyStateComponent.hpp"
#include "Application/Components/Tag/TagComponent.hpp"

#include "System/Conponent/Collider/ColliderComponent.hpp"
#include "System/Conponent/Fbx/FbxMeshConponent.hpp"
#include "System/Conponent/Transform/TransformConponent.hpp"


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
	auto view = Reg.view<EnemyStateComponent, FbxComponent>(entt::exclude<DeadTag>);
	bool ret = false;

	//	プレイヤーの座標取得
	auto Player_view = Reg.view<Transform3D, PlayerTag>(entt::exclude<DeadTag>);
	Math::Vector3 PlayerPos = {};

	//	プレイヤーの座標
	Player_view.each([&](auto entity, Transform3D& trans)
		{
			PlayerPos = trans.Position;
		});

	//	状態遷移
	view.each([&](auto entity, EnemyStateComponent& state, FbxComponent& fbx) 
		{
			/*
			* 処理によって行動を変化させる
			*/

			//	追跡

			//	待機状態
			if (state.State == eEnemyState::Idle)
			{
			}


		});

}
