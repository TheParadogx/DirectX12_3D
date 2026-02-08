#include "pch.h"
#include "StageSelectScene.hpp"

#include"System/Camera/Camera.hpp"

//	EngineSystem
#include"System/Conponent/Collider/System/ColliderSystem.hpp"
#include "System/Entity/System/Manager/SystemManager.hpp"
#include"System/Entity/Manager/EntityManager.hpp"
#include"System/Input/Manager/InputManager.hpp"
#include"System/Scene/Manager/SceneManager.hpp"
#include"System/Window/Window.hpp"

#include"System/Conponent/Sprite/SpriteComponent.hpp"
#include"System/Conponent/Transform/TransformConponent.hpp"

//	Resrouce
#include"Graphics/SkyBox/Resource/SkyBoxResourceManager.hpp"
#include"Graphics/Texture/Manager/TextureManager.hpp"

//	User
#include"Application/Components/Tag/TagComponent.hpp"
#include"Application/Objects/CreateObjects.hpp"
#include"Application/Scene/StageSelect/Object/StageSelectObjFactory.hpp"
#include"Application/Objects/EnemyRank.hpp"

//	UseSystem
#include"Application/Components/CameraWork/System/CameraControlSystem.hpp"
#include"Application/Components/Socket/SocketComponentSystem.hpp"
#include"Application/Components/Player/Input/InputRequestSystem.hpp"
#include"Application/Components/Player/PlayerState/PlayerStateSystem.hpp"
#include"Application/Components/InputMove/MoveComponentSystem.hpp"
#include"Application/Scene/StageSelect/Component/Interact/System/InteractableSystem.hpp"
#include"Application/Components/Skill/System/SkillSystem.hpp"

#include"Audio/Manager/AudioManager.hpp"

#include"Application/Scene/Title/TitleScene.hpp"
#include"System/Time/Time.hpp"

void Engine::Scene::StageSelect::OpenConfirmWindow()
{
	System::Time::Scale = 0.0f;
	mState = eState::Confirming;
	using namespace System;
	auto manager = EntityManager::GetInstance();
	auto& registry = manager->GetRegistry();

	mConfirmWindowEntity = manager->CreateEntity();
	auto window = Window::GetInstance();

	//	座標
	auto& trans = registry.emplace<Transform2D>(mConfirmWindowEntity);
	trans.Position = { window->GetWidth() / 2.0f ,(float)window->GetHeight() / 2 };
	float scale = 0.5f;
	trans.Scale = { scale,scale };

	//	sprite
	auto res = Graphics::TextureManager::GetInstance()->Load("Assets/StageSelect/Texture/OpitionMenu.png");
	auto& sprite = registry.emplace<SpriteComponent>(mConfirmWindowEntity, res);
	sprite.Sprite.SetColor({ 1,1,1,1 });
	sprite.Sprite.SetPivot({ 0.4,0.5 });
	sprite.Sprite.SetPosition(trans.Position);
	sprite.Sprite.SetScale(trans.Scale);
}

void Engine::Scene::StageSelect::CloseConfirmWindow()
{
	System::Time::Scale = 1.0f;
	mState = eState::Normal;
	using namespace System;
	auto manager = EntityManager::GetInstance();
	auto& registry = manager->GetRegistry();
	registry.destroy(mConfirmWindowEntity);
	mConfirmWindowEntity = entt::null;

}

bool Engine::Scene::StageSelect::Initialize()
{
	////	カメラ
	Engine::System::Camera* camera = new Engine::System::Camera();
	camera->Create();

	System::SystemManager::GetInstance()->AddSystem<System::InputRequestSystem>();
	System::SystemManager::GetInstance()->AddSystem<System::PlayerStateSystem>();
	System::SystemManager::GetInstance()->AddSystem<System::MoveComponentSystem>();
	System::SystemManager::GetInstance()->AddSystem<System::SocketComponentSystem>();
	System::SystemManager::GetInstance()->AddSystem<System::InteractableSystem>();
	System::SystemManager::GetInstance()->AddSystem<System::CameraControlSystem>();
	System::SystemManager::GetInstance()->AddSystem<System::SkillSystem>();

	System::ColliderSystem::Initialize();
	System::ColliderSystem::AddCollisionPair<System::PlayerTag, System::EnemyTag>({ true,false,false,false });

	System::ObjectsFactory::CreateField();
	System::ObjectsFactory::CreatePlayer();
	System::StageSelectObjFactory::CreateEnemy_Basic();
	System::StageSelectObjFactory::CreateEnemy_Advanced();
	System::StageSelectObjFactory::CreateEnemy_Boss();
#ifdef _DEBUG
	//System::StageSelectObjFactory::TestEffect();
#endif // _DENUG

	//	空
	auto SkyBoxResource = Graphics::SkyBoxResourceManager::GetInstance()->Load("Assets/SkyBox/skybox.dds");
	mSkyBox = std::make_unique<Graphics::SkyBox>();
	mSkyBox->Create(SkyBoxResource);

	//	タイトルの音
	Audio::AudioManager::GetInstance()->PlaySE("Assets/StageSelect/Sound/BGM.aud", true, 0.05f,false);


	return true;
}

void Engine::Scene::StageSelect::Update(double DeltaTime)
{
	auto input = Input::InputManager::GetInstance();

	//	optionで状態判定
	if (input->IsActionPressed("Option"))
	{
		if (mState == eState::Normal)
		{
			//	ウィンドウを出す
			OpenConfirmWindow();
		}
		else
		{
			//	ウィンドウを閉じる
			CloseConfirmWindow();
		}
	}

	//	状態遷移判定
	if (mState == eState::Confirming)
	{
		if (input->IsActionPressed("Select"))
		{
			System::Time::Scale = 1.0f;
			//	状態遷移を呼ぶ
			System::SceneManager::GetInstance()->ChangeSceneFade<Scene::Title>();
			//	SE
			Audio::AudioManager::GetInstance()->PlaySE("Assets/Sound/Select.aud", false, 1.0f);
		}
		else if (input->IsActionPressed("Cancel"))
		{
			CloseConfirmWindow();
		}
		return;
	}

}

/// <summary>
/// 描画
/// </summary>
void Engine::Scene::StageSelect::Render()
{
	mSkyBox->Render();
}

void Engine::Scene::StageSelect::Release()
{
	System::Time::Scale = 1.0f;
	System::EntityManager::GetInstance()->ClearLocalEntities();
}
