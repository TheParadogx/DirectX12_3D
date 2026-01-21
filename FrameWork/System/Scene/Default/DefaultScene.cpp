#include "pch.h"
#include "DefaultScene.hpp"

#include "System/Entity/System/Manager/SystemManager.hpp"
#include"System/Entity/Manager/EntityManager.hpp"
#include"Graphics/FbxMesh/Resource/Manager/FbxResourceManager.hpp"

#include"Application/Components/InputMove/MoveComponentSystem.hpp"
#include"Application/Components/CameraWork/System/CameraControlSystem.hpp"

#include"Application/Components/Tag/TagComponent.hpp"
#include"System/Conponent/Collider/System/ColliderSystem.hpp"

/*
* テスト
*/

#include"Application/Objects/CreateObjects.hpp"

#include"Graphics/Texture/Texture.hpp"
#include"Graphics/Sprite/Sprite.hpp"
#include"Graphics/FbxMesh/Resource/FbxResource.hpp"
#include"Graphics/FbxMesh/FbxMesh.hpp"
#include"System/Camera/Camera.hpp"
#include"Math/Math.h"
#include"System/Collider/AABB/AABBCollider.hpp"

#include"System/Conponent/Rigidbody/RigidbodyComponent.hpp"
#include"System/Conponent/Transform/TransformConponent.hpp"
#include"System/Conponent/Fbx/FbxMeshConponent.hpp"
#include"Application/Components/Socket/SocketComponentSystem.hpp"
#include"Application/Components/HpRender/HpRenderSystem.hpp"
#include"Application/Components/Player/Input/InputRequestSystem.hpp"
#include"Application/Components/Player/PlayerState/PlayerStateSystem.hpp"
#include"Application/Components/Damage/DamageSystem.hpp"

#include"Graphics/Texture/Manager/TextureManager.hpp"
#include"Graphics/UI/UISprite.hpp"

static Engine::System::Camera sCamera;

bool Engine::System::DefaultScene::Initialize()
{
    bool ret = false;
    ret = sCamera.Create();
    if (ret == false)
    {
        return false;
    }

    auto baseRes = Graphics::TextureManager::GetInstance()->Load("Assets/Texture/HPBar/test.png");
    auto barRes = Graphics::TextureManager::GetInstance()->Load("Assets/Texture/HPBar/Bar.png");
    auto luffy = Graphics::TextureManager::GetInstance()->Load("Assets/Texture/luffy.dds");

    //  システムの追加
    //System::SystemManager::GetInstance()->AddSystem<System::MoveComponentSystem>();
    System::SystemManager::GetInstance()->AddSystem<System::CameraControlSystem>();
    System::SystemManager::GetInstance()->AddSystem<System::SocketComponentSystem>();
    System::SystemManager::GetInstance()->AddSystem<System::HpRenderSystem>();
    System::SystemManager::GetInstance()->AddSystem<System::InputRequestSystem>();
    System::SystemManager::GetInstance()->AddSystem<System::PlayerStateSystem>();
    System::SystemManager::GetInstance()->AddSystem<System::MoveComponentSystem>();
    System::SystemManager::GetInstance()->AddSystem<System::DamageSystem>();

    //  当たり判定のシステム
    System::ColliderSystem::Initialize();
    //System::ColliderSystem::AddCollisionPair<PlayerTag, EnemyTag>({ true,false });
    System::ColliderSystem::AddCollisionPair<WeaponTag, EnemyTag>({ false,false });

    //  フィールド
    System::ObjectsFactory::CreateField();

    //  プレイヤーの作成
    System::ObjectsFactory::CreatePlayer();

    //  敵の作成
    System::ObjectsFactory::CreateEnemy();
    //System::ObjectsFactory::CreateSword(player);

	return true;
}


void Engine::System::DefaultScene::ImGuiUpdate()
{
}


void Engine::System::DefaultScene::Release()
{
}
