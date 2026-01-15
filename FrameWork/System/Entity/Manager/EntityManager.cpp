#include "pch.h"
#include "EntityManager.hpp"
#include"System/Entity/Traits/EnTTTraits.hpp"

#include"System/Conponent/Transform/TransformConponent.hpp"

/// <summary>
/// 初期化
/// </summary>
void Engine::System::EntityManager::OnCreate()
{
    //  座標は全てのオブジェクトが持っているのでソート基準にします。
    mRegistry.group<Transform3D>();
}

/// <summary>
/// registryへの直接アクセス（Viewを作成できるようにするため）
/// </summary>
/// <returns></returns>
entt::registry& Engine::System::EntityManager::GetRegistry()
{
    return mRegistry;
}

/// <summary>
/// エンティティの生成
/// </summary>
/// <returns></returns>
entt::entity Engine::System::EntityManager::CreateEntity()
{
    return mRegistry.create();
}

/// <summary>
///	永続エンティティの作成 
/// </summary>
/// <returns></returns>
entt::entity Engine::System::EntityManager::CreatePersistentEntity()
{
    auto entity = mRegistry.create();
    mRegistry.emplace<Persistent>(entity);
    //mRegistry.emplace<Transform3D>(entity);   //// 座標系を絶対に持たせるかどうか考えます
    return entity;
}

/// <summary>
/// ローカルオブジェクト（Scene単位）のクリア
/// </summary>
void Engine::System::EntityManager::ClearLocalEntities()
{
    auto view = mRegistry.view<entt::entity>(entt::exclude<Persistent>);

    //  一括削除
    mRegistry.destroy(view.begin(), view.end());
}

/// <summary>
/// すべて削除
/// </summary>
void Engine::System::EntityManager::AllClear()
{
    mRegistry.clear();
}
