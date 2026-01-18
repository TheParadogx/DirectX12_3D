#include "pch.h"
#include "CreateObjects.hpp"

#include"System/Entity/Manager/EntityManager.hpp"
#include"Graphics/FbxMesh/Resource/Manager/FbxResourceManager.hpp"
#include"Graphics/Texture/Manager/TextureManager.hpp"

#include"System/Conponent/Rigidbody/RigidbodyComponent.hpp"
#include"System/Conponent/Transform/TransformConponent.hpp"
#include"System/Conponent/Fbx/FbxMeshConponent.hpp"
#include"System/Conponent/Collider/ColliderComponent.hpp"

#include"Application/Components/InputMove/MoveComponent.hpp"
#include"Application/Components/CameraWork/CameraControl.hpp"
#include"Application/Components/Socket/SocketComponent.hpp"
#include"Application/Components/HpRender/HpRenderComponent.hpp"
#include"Application/Components/Status/StatusComponent.hpp"

#include"Application/Components/Player/PlayerState/PlayerStateComponent.hpp"
#include"Application/Components/Player/Input/InputRequestComponent.hpp"

#include"Application/Components/Tag/TagComponent.hpp"


entt::entity Engine::System::ObjectsFactory::CreatePlayer()
{
	auto manager = EntityManager::GetInstance();
	auto& registry = EntityManager::GetInstance()->GetRegistry();

	const float Scale = 0.05f;

	//	オブジェクト
	auto player = manager->CreateEntity();

	//	座標
	auto& transform = registry.emplace<Transform3D>(player);
	transform.Position = { 0.0f,0.0f,0.0f };
	transform.Scale = { Scale ,Scale ,Scale };
	transform.Rotation = Math::Quaternion::Identity;

	//	fbxのリソース
	auto res = Graphics::FbxResourceManager::GetInstance()->Load("Assets/Mannequin/SKM_Manny_Simple.FBX.bin");
	res->LoadAnimation("Idle", "Assets/Mannequin/Animation/MM_Idle.FBX.anm");
	res->LoadAnimation("Run", "Assets/Mannequin/Animation/MM_Run_Fwd.FBX.anm");
	res->LoadAnimation("Jump", "Assets/Mannequin/Animation/MM_Jump.FBX.anm");
	res->LoadAnimation("Attack", "Assets/Mannequin/Animation/SwordSlash.fbx.anm");

	//	fbxのモデル
	auto& fbx = registry.emplace<FbxComponent>(player, res);
	fbx.CurrAnimation = "Idle";

	//	当たり判定
	//auto& collider = registry.emplace<AABBColliderComponent>(player);
	//collider.Collider.SetVolume({ 2.0f,8.0f,2.0f });
	//collider.Offset = { 0.0f, collider.Collider.GetVolume().y * 0.5f, 0.0f };

	//	当たり判定
	auto col = ColliderComponent::Create<AABBCollider>();
	auto collider = col.GetPtr<AABBCollider>();
	collider->SetVolume({ 2.0f,8.0f,2.0f });
	col.Offset = { 0.0f, collider->GetVolume().y * 0.5f, 0.0f };
	registry.emplace<ColliderComponent>(player, std::move(col));

	//	移動量（物理）
	auto& rigidbody = registry.emplace<Rigidbody3D>(player);
	rigidbody.Flags |= RigidbodyFlags::UseCameraBasis;

	//	カメラ追従
	auto& camera = registry.emplace<System::CameraControlComponent>(player);
	camera.TargetEntity = player;
	camera.offset = { 0,8,0 };

	//	ステータス
	auto& status = registry.emplace<System::StatusComponet>(player);
	status.MaxHp.Base = 1000;
	status.Hp = 1000;

	auto baseRes = Graphics::TextureManager::GetInstance()->Load("Assets/Texture/HPBar/BarBase.png");
	auto barRes = Graphics::TextureManager::GetInstance()->Load("Assets/Texture/HPBar/Bar.png");

	//	体力バー
	auto& hpBar = registry.emplace<System::HpRenderComponent>(player, baseRes, barRes, Math::Vector2(0.0,0.0));
	hpBar.SetPosition({100,10});

	//	入力
	registry.emplace<System::InputRequestComponent>(player);
	//	状態
	auto& state = registry.emplace<System::PlayerStateComponent>(player);

	//	移動
	auto& move = registry.emplace<System::MoveComponent>(player);
	move.MoveSpeed = 20.0f;

	//	武器
	auto sword = CreateSword(player);
	state.Weapon = sword;

	//	タグ
	registry.emplace<PlayerTag>(player);

	return player;
}

void Engine::System::ObjectsFactory::CreateEnemy()
{
	auto manager = EntityManager::GetInstance();
	auto& registry = EntityManager::GetInstance()->GetRegistry();

	const float Scale = 0.05f;

	auto enemy = manager->CreateEntity();

	//	座標
	auto& transform = registry.emplace<Transform3D>(enemy);
	transform.Position = { 20.0f,0.0f,20.0f };
	transform.Scale = { Scale ,Scale ,Scale };
	transform.Rotation = Math::Quaternion::Identity;

	//	fbxのリソース
	auto res = Graphics::FbxResourceManager::GetInstance()->Load("Assets/Mannequin/SKM_Manny_Simple.FBX.bin");
	res->LoadAnimation("Idle", "Assets/Mannequin/Animation/MM_Idle.FBX.anm");
	res->LoadAnimation("Jump", "Assets/Mannequin/Animation/MM_Jump.FBX.anm");
	res->LoadAnimation("Attack", "Assets/Mannequin/Animation/SwordSlash.fbx.anm");
	
	//	fbxのモデル
	auto& fbx = registry.emplace<FbxComponent>(enemy, res);
	fbx.CurrAnimation = "Idle";
	fbx.Mesh->SetColor(Graphics::Color::Red());

	//	当たり判定
	//auto& collider = registry.emplace<AABBColliderComponent>(enemy);
	//collider.Collider.SetVolume({ 2.0f,8.0f,2.0f });
	//collider.Offset = { 0.0f, collider.Collider.GetVolume().y * 0.5f, 0.0f };

	auto col = ColliderComponent::Create<AABBCollider>();
	auto collider = col.GetPtr<AABBCollider>();
	collider->SetVolume({ 2.0f,8.0f,2.0f });
	col.Offset = { 0.0f, collider->GetVolume().y * 0.5f, 0.0f };
	registry.emplace<ColliderComponent>(enemy, std::move(col));

	//	タグ
	registry.emplace<EnemyTag>(enemy);

}

void Engine::System::ObjectsFactory::CreateField()
{
	auto manager = EntityManager::GetInstance();
	auto& registry = EntityManager::GetInstance()->GetRegistry();

	auto field = manager->CreateEntity();

	//	サイズ
	const Math::Vector3 volume = { 100,10,100 };

	//	座標
	auto& trans = registry.emplace<Transform3D>(field);
	trans.Position = { 0.0f,-10.0f,0.0f };
	trans.Scale = volume;

	//	fbx
	auto res = Graphics::FbxResourceManager::GetInstance()->Load("Assets/Cube/Cube.fbx.bin");
	auto& fbx = registry.emplace<FbxComponent>(field, res,false);
	fbx.Mesh->SetColor(Graphics::Color::Cyan());

	//	collider
	auto& collider = registry.emplace<AABBColliderComponent>(field);
	collider.Collider.SetVolume(volume);
	collider.Offset = { 0.0f, 5.0f, 0.0f };
}

entt::entity Engine::System::ObjectsFactory::CreateSword(entt::entity Parent)
{
	auto manager = EntityManager::GetInstance();
	auto& registry = EntityManager::GetInstance()->GetRegistry();

	const float Scale = 1.0f;
	auto sword = manager->CreateEntity();

	//	座標
	auto& transform = registry.emplace<Transform3D>(sword);
	transform.Scale = { Scale ,Scale ,Scale };
	transform.Rotation = Math::Quaternion::Identity;

	//	fbxのリソース
	auto res = Graphics::FbxResourceManager::GetInstance()->Load("Assets/Sword/Sword.fbx.bin");


	//	fbxのモデル
	auto& fbx = registry.emplace<FbxComponent>(sword, res, false);
	fbx.CurrAnimation = "";
	//fbx.Mesh->SetColor(Graphics::Color::Green);

	//	当たり判定

	//	アタッチ
	auto& socket = registry.emplace<SocketComponent>(sword);
	socket.Parent = Parent;
	socket.BoneName = "hand_r";
	socket.OffsetPos = { 0.1,-0.64,-0.17 };
	socket.OffsetRot = { 0.0,0.848,0.0,0.531 };
	socket.PivotOffset = { -0.39,2.27,-0.22 };

	return sword;
}
