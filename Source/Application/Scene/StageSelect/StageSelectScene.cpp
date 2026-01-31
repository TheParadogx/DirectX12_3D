#include "pch.h"
#include "StageSelectScene.hpp"

#include"System/Camera/Camera.hpp"

//	EngineSystem
#include"System/Conponent/Collider/System/ColliderSystem.hpp"
#include "System/Entity/System/Manager/SystemManager.hpp"
#include"System/Entity/Manager/EntityManager.hpp"
//	Resrouce
#include"Graphics/SkyBox/Resource/SkyBoxResourceManager.hpp"

//	User
#include"Application/Components/Tag/TagComponent.hpp"
#include"Application/Objects/CreateObjects.hpp"
#include"Application/Scene/StageSelect/Object/StageSelectObjFactory.hpp"

//	UseSystem
#include"Application/Components/CameraWork/System/CameraControlSystem.hpp"
#include"Application/Components/Socket/SocketComponentSystem.hpp"
#include"Application/Components/Player/Input/InputRequestSystem.hpp"
#include"Application/Components/Player/PlayerState/PlayerStateSystem.hpp"
#include"Application/Components/InputMove/MoveComponentSystem.hpp"


bool Engine::Scene::StageSelect::Initialize()
{
	////	ƒJƒƒ‰
	Engine::System::Camera* camera = new Engine::System::Camera();
	camera->Create();

	System::SystemManager::GetInstance()->AddSystem<System::InputRequestSystem>();
	System::SystemManager::GetInstance()->AddSystem<System::PlayerStateSystem>();
	System::SystemManager::GetInstance()->AddSystem<System::MoveComponentSystem>();
	System::SystemManager::GetInstance()->AddSystem<System::SocketComponentSystem>();
	System::SystemManager::GetInstance()->AddSystem<System::CameraControlSystem>();

	System::ColliderSystem::Initialize();
	System::ColliderSystem::AddCollisionPair<System::PlayerTag, System::EnemyTag>({ true,false,false,false });

	System::ObjectsFactory::CreateField();
	System::ObjectsFactory::CreatePlayer();
	System::StageSelectObjFactory::CreateEnemy_Basic();
	System::StageSelectObjFactory::CreateEnemy_Advanced();
	System::StageSelectObjFactory::CreateEnemy_Boss();

	//	‹ó
	auto SkyBoxResource = Graphics::SkyBoxResourceManager::GetInstance()->Load("Assets/SkyBox/cubemap.dds");
	mSkyBox = std::make_unique<Graphics::SkyBox>();
	mSkyBox->Create(SkyBoxResource);


	return true;
}

/// <summary>
/// •`‰æ
/// </summary>
void Engine::Scene::StageSelect::Render()
{
	mSkyBox->Render();
}

void Engine::Scene::StageSelect::Release()
{
	System::EntityManager::GetInstance()->ClearLocalEntities();
}
