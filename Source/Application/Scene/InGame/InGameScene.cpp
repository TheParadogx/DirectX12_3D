#include "pch.h"
#include "InGameScene.hpp"

#include "System/Entity/System/Manager/SystemManager.hpp"
#include"System/Entity/Manager/EntityManager.hpp"
#include"Graphics/FbxMesh/Resource/Manager/FbxResourceManager.hpp"

#include"System/Camera/Camera.hpp"
#include"System/Conponent/Collider/System/ColliderSystem.hpp"

#include"Application/Components/CameraWork/System/CameraControlSystem.hpp"
#include"Application/Components/Socket/SocketComponentSystem.hpp"
#include"Application/Components/HpRender/HpRenderSystem.hpp"
#include"Application/Components/Player/Input/InputRequestSystem.hpp"
#include"Application/Components/Player/PlayerState/PlayerStateSystem.hpp"
#include"Application/Components/Damage/DamageSystem.hpp"
#include"Application/Components/InputMove/MoveComponentSystem.hpp"
#include"Application/Components/GameRule/GameRuleSystem.hpp"
#include"Application/Components/Invincible/InvincibleSystem.hpp"

#include"Application/Components/Enemy/State/EnemyStateSystem.hpp"

#include"Application/Components/Tag/TagComponent.hpp"
#include"Application/Objects/CreateObjects.hpp"

#include"Graphics/SkyBox/Resource/SkyBoxResourceManager.hpp"

#include"Graphics/VFX/VfxSprite.hpp"
#include"Graphics/Texture/Manager/TextureManager.hpp"


namespace Engine::Scene
{
	/// <summary>
	///	初期化
	/// </summary>
	/// <returns></returns>
	bool InGame::Initialize()
	{
		//	カメラ
		Engine::System::Camera* camera = new Engine::System::Camera();
		camera->Create();

		//	システム追加
		System::SystemManager::GetInstance()->AddSystem<System::HpRenderSystem>();
		System::SystemManager::GetInstance()->AddSystem<System::InputRequestSystem>();
		System::SystemManager::GetInstance()->AddSystem<System::PlayerStateSystem>();
		System::SystemManager::GetInstance()->AddSystem<System::EnemyStateSystem>();
		System::SystemManager::GetInstance()->AddSystem<System::MoveComponentSystem>();
		System::SystemManager::GetInstance()->AddSystem<System::DamageSystem>();
		System::SystemManager::GetInstance()->AddSystem<System::SocketComponentSystem>();
		System::SystemManager::GetInstance()->AddSystem<System::CameraControlSystem>();
		System::SystemManager::GetInstance()->AddSystem<System::GameRuleSystem>();
		System::SystemManager::GetInstance()->AddSystem<System::InvincibleSystem>();

		System::ColliderSystem::Initialize();
		System::ColliderSystem::AddCollisionPair<System::PlayerTag, System::EnemyTag>({true,false,false,false});
		System::ColliderSystem::AddCollisionPair<System::PlayerWeaponTag, System::EnemyTag>({ false,false });
		System::ColliderSystem::AddCollisionPair<System::EnemyWeaponTag, System::PlayerTag>({ false,false });

		System::ObjectsFactory::CreateField();
		System::ObjectsFactory::CreatePlayer();
		System::ObjectsFactory::CreateEnemy();

		auto SkyBoxResource = Graphics::SkyBoxResourceManager::GetInstance()->Load("Assets/SkyBox/cubemap.dds");
		mSkyBox = std::make_unique<Graphics::SkyBox>();
		mSkyBox->Create(SkyBoxResource);

		return true;
	}

	/// <summary>
	/// 画面の切り替え判定
	/// </summary>
	/// <param name="FixedDeltaTime"></param>
	void InGame::PostUpdate(double FixedDeltaTime)
	{

	}

	/// <summary>
	/// 描画
	/// </summary>
	void InGame::Render()
	{
		mSkyBox->Render();
	}

	/// <summary>
	/// 終了処理
	/// </summary>
	void InGame::Release()
	{
		System::EntityManager::GetInstance()->ClearLocalEntities();
	}
}