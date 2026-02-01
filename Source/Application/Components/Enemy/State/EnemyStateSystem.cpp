#include "pch.h"
#include "EnemyStateSystem.hpp"

#include "Application/Components/Tag/TagComponent.hpp"

#include "System/Conponent/Collider/ColliderComponent.hpp"
#include "System/Conponent/Fbx/FbxMeshConponent.hpp"
#include "System/Conponent/Transform/TransformConponent.hpp"
#include "System/Conponent/Rigidbody/RigidbodyComponent.hpp"

#include "Application/Components/WeaponAttack/WeaponAttackComponent.hpp"

#include "Graphics/DebugRender/DebugRender.hpp"


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
}

/// <summary>
/// デバックの描画
/// </summary>
/// <param name="Reg"></param>
void Engine::System::EnemyStateSystem::Render(entt::registry& Reg)
{
#if _DEBUG
#endif // _DEBUG
}
