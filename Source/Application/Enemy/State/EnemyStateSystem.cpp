#include "pch.h"
#include "EnemyStateSystem.hpp"

#include "Application/Enemy/State/EnemyStateComponent.hpp"
#include "Application/Components/Tag/TagComponent.hpp"

#include "System/Conponent/Collider/ColliderComponent.hpp"
#include "System/Conponent/Fbx/FbxMeshConponent.hpp"
#include "System/Conponent/Transform/TransformConponent.hpp"

void Engine::System::EnemyStateSystem::PreUpdate(entt::registry& Reg, double DeltaTime)
{
}

void Engine::System::EnemyStateSystem::MainUpdate(entt::registry& Reg, double DeltaTime)
{
	auto view = Reg.view<EnemyStateComponent, FbxComponent>(entt::exclude<DeadTag>);
	bool ret = false;

	//	プレイヤーのタグを取得する
	//auto playerView = Reg.view<Transform3D, PlayerTag>();
	//Math::Vector3 playerPos;
	//for (auto entity : playerView)
	//{
	//	playerPos = playerView.get<Transform3D>(entity).Position;
	//}


	//	状態遷移
	view.each([&](auto entity, EnemyStateComponent& state, FbxComponent& fbx) 
		{



			//	距離に応じて状態を変える


			//	攻撃

			//	追跡

			//	待機状態

		});

}
