#include "pch.h"
#include "DefaultScene.hpp"

#include "System/Entity/System/Manager/SystemManager.hpp"
#include"System/Entity/Manager/EntityManager.hpp"
#include"Graphics/FbxMesh/Resource/Manager/FbxResourceManager.hpp"

#include"Application/Components/InputMove/MoveComponentSystem.hpp"
#include"Application/Components/CameraWork/System/CameraControlSystem.hpp"
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

#include"Graphics/Texture/Manager/TextureManager.hpp"
#include"Graphics/UI/UISprite.hpp"

static Engine::System::Camera sCamera;
static entt::entity player;

static Engine::Graphics::Texture sTexture;
static Engine::Graphics::UISprite sUI;

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
    ret = sTexture.Create("Assets/Texture/luffy.dds");


    //  システムの追加
    //System::SystemManager::GetInstance()->AddSystem<System::MoveComponentSystem>();
    System::SystemManager::GetInstance()->AddSystem<System::CameraControlSystem>();
    System::SystemManager::GetInstance()->AddSystem<System::SocketComponentSystem>();
    System::SystemManager::GetInstance()->AddSystem<System::HpRenderSystem>();
    System::SystemManager::GetInstance()->AddSystem<System::InputRequestSystem>();

    //  フィールド
    System::ObjectsFactory::CreateField();

    //  プレイヤーの作成
    player = System::ObjectsFactory::CreatePlayer();

    //  敵の作成
    System::ObjectsFactory::CreateEnemy();
    System::ObjectsFactory::CreateSword(player);

    ret = sUI.Create(luffy);
    sUI.SetPosition({ 100,100 });
    sUI.SetScale({ 0.2,0.2 });
    sUI.SetFillRatio(1.0f);
	return true;
}

void Engine::System::DefaultScene::Update(double DeltaTime)
{
}

void Engine::System::DefaultScene::FixedUpdate(double FixedDeltaTime)
{

}

void Engine::System::DefaultScene::ImGuiUpdate()
{
}

void Engine::System::DefaultScene::Render()
{
    sUI.Render();
}

void Engine::System::DefaultScene::Release()
{
}
