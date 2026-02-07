#include "pch.h"
#include "StageSelectObjFactory.hpp"

#include"System/Entity/Manager/EntityManager.hpp"
#include"Graphics/FbxMesh/Resource/Manager/FbxResourceManager.hpp"
#include"System/Conponent/Transform/TransformConponent.hpp"
#include"System/Conponent/Fbx/FbxMeshConponent.hpp"
#include"System/Conponent/Collider/ColliderComponent.hpp"
#include"Application/Components/Socket/SocketComponent.hpp"
#include "Application/Components/Enemy/State/EnemyStateComponent.hpp"

#include"Application/Components/Tag/TagComponent.hpp"
#include"Application/Scene/StageSelect/Component/Interact/InteractableRange.hpp"
#include"System/Conponent/Vfx/VfxMeshComponent.hpp"
#include "Graphics/Texture/Manager/TextureManager.hpp"

#include"System/Conponent/Effect/EffectComponent.hpp"
#include"Graphics/Effect/Manager/EffectManager.hpp"
#include"System/Conponent/Effect/Factory/EffectFactory.hpp"

#include"System/CSV/CSVManager.hpp"
#include"Application/Data/SaveData.hpp"


entt::entity Engine::System::StageSelectObjFactory::CreateEnemyWeapon(entt::entity Parent, const std::string& BoneName, const Graphics::Color& Color)
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
	fbx.Mesh->SetColor(Color);

	//	アタッチ
	auto& socket = registry.emplace<SocketComponent>(sword);
	socket.Parent = Parent;
	socket.BoneName = BoneName;
	socket.OffsetPos = { 1.82,0.51,-0.46 };
	socket.OffsetRot = { -0.7,-0.685,0.192,0.007 };
	socket.PivotOffset = { 0,1.98,0.0 };

	registry.emplace<EnemyWeaponTag>(sword);


	return sword;

}

void Engine::System::StageSelectObjFactory::CreateTalkTexture(entt::registry& Reg, entt::entity entity, bool IsTalkable,const std::string& FilePath)
{



}

/// <summary>
/// 最弱の敵
/// </summary>
void Engine::System::StageSelectObjFactory::CreateEnemy_Basic()
{
	auto manager = EntityManager::GetInstance();
	auto& registry = EntityManager::GetInstance()->GetRegistry();

	const float Scale = 0.05f;

	auto enemy = manager->CreateEntity();

	//	座標
	auto& transform = registry.emplace<Transform3D>(enemy);
	transform.Position = { 20.0f,0.0f,20.0f };
	transform.Scale = { Scale ,Scale ,Scale };
	transform.Rotation = Math::Quaternion::AngleAxis(3.14159265f, Math::Vector3::Forward);


	//	素材テスト
	auto res = Graphics::FbxResourceManager::GetInstance()->Load("Assets/Fbx/Ganfaul/Faul.fbx.bin");
	res->LoadAnimation("Idle", "Assets/Fbx/Ganfaul/Animation/Idle.fbx.anm");

	//	fbxのモデル
	auto& fbx = registry.emplace<FbxComponent>(enemy, res);
	fbx.CurrAnimation = "Idle";
	fbx.Mesh->SetColor(Graphics::Color::Cyan());

	auto col = ColliderComponent::Create<AABBCollider>();
	auto collider = col.GetPtr<AABBCollider>();
	collider->SetVolume({ 2.0f,8.0f,2.0f });
	col.Offset = { 0.0f, collider->GetVolume().y * 0.5f, 0.0f };
	registry.emplace<ColliderComponent>(enemy, std::move(col));

	//　状態管理
	auto& state = registry.emplace<EnemyStateComponent>(enemy);

	//	武器
	auto sword = CreateEnemyWeapon(enemy, "RightHand",Graphics::Color::Cyan());
	state.Weapon = sword;

	//	インタラクト
	auto& interact = registry.emplace<InteractableComponent>(enemy);
	interact.Rank = EnemyRank::Basic;
	interact.Talkable = true;

	//	話しかける用の画像
	std::string FilePath = "Assets/StageSelect/Texture/Blue_Challenge.png";

	auto vfxRes = Graphics::TextureManager::GetInstance()->Load(FilePath);
	auto& vfx = registry.emplace<VfxMeshComponent>(enemy, vfxRes);
	vfx.IsShow = true;
	vfx.Offset = { 0,11,0 };

	//	タグ
	registry.emplace<EnemyTag>(enemy);

}

/// <summary>
/// ちょっと強い敵
/// </summary>
void Engine::System::StageSelectObjFactory::CreateEnemy_Advanced()
{
	auto manager = EntityManager::GetInstance();
	auto& registry = EntityManager::GetInstance()->GetRegistry();

	const float Scale = 0.05f;

	auto enemy = manager->CreateEntity();

	//	座標
	auto& transform = registry.emplace<Transform3D>(enemy);
	transform.Position = { -20.0f,0.0f,20.0f };
	transform.Scale = { Scale ,Scale ,Scale };
	transform.Rotation = Math::Quaternion::AngleAxis(3.14159265f, Math::Vector3::Forward);

	//	素材テスト
	auto res = Graphics::FbxResourceManager::GetInstance()->Load("Assets/Fbx/Ganfaul/Faul.fbx.bin");
	res->LoadAnimation("Idle", "Assets/Fbx/Ganfaul/Animation/Idle.fbx.anm");

	//	fbxのモデル
	auto& fbx = registry.emplace<FbxComponent>(enemy, res);
	fbx.CurrAnimation = "Idle";
	fbx.Mesh->SetColor(Graphics::Color::Yellow());

	auto col = ColliderComponent::Create<AABBCollider>();
	auto collider = col.GetPtr<AABBCollider>();
	collider->SetVolume({ 2.0f,8.0f,2.0f });
	col.Offset = { 0.0f, collider->GetVolume().y * 0.5f, 0.0f };
	registry.emplace<ColliderComponent>(enemy, std::move(col));

	//　状態管理
	auto& state = registry.emplace<EnemyStateComponent>(enemy);

	//	武器
	auto sword = CreateEnemyWeapon(enemy, "RightHand", Graphics::Color::Yellow());
	state.Weapon = sword;

	//	インタラクト
	auto& interact = registry.emplace<InteractableComponent>(enemy);
	interact.Rank = EnemyRank::Advanced;
	interact.Talkable = false;
	interact.UnlockLevel = 1;

	auto data = System::CSV::Get<SaveData>().Find(1);

	if (data->ClearLevel >= interact.UnlockLevel)
	{
		interact.Talkable = true;
	}

	//	話しかける用の画像
	std::string FilePath;
	if (interact.Talkable == true)
	{
		FilePath = "Assets/StageSelect/Texture/Yellow_Challenge.png";
	}
	else
	{
		FilePath = "Assets/StageSelect/Texture/Yellow_Require.png";
	}

	auto vfxRes = Graphics::TextureManager::GetInstance()->Load(FilePath);
	auto& vfx = registry.emplace<VfxMeshComponent>(enemy, vfxRes);
	vfx.IsShow = true;
	vfx.Offset = { 0,11,0 };

	//	タグ
	registry.emplace<EnemyTag>(enemy);

}

/// <summary>
/// 最強
/// </summary>
void Engine::System::StageSelectObjFactory::CreateEnemy_Boss()
{
	auto manager = EntityManager::GetInstance();
	auto& registry = EntityManager::GetInstance()->GetRegistry();

	const float Scale = 0.05f;

	auto enemy = manager->CreateEntity();

	//	座標
	auto& transform = registry.emplace<Transform3D>(enemy);
	transform.Position = { 0.0f,0.0f,20.0f };
	transform.Scale = { Scale ,Scale ,Scale };
	transform.Rotation = Math::Quaternion::AngleAxis(3.14159265f, Math::Vector3::Forward);


	//	素材テスト
	auto res = Graphics::FbxResourceManager::GetInstance()->Load("Assets/Fbx/Ganfaul/Faul.fbx.bin");
	res->LoadAnimation("Idle", "Assets/Fbx/Ganfaul/Animation/Idle.fbx.anm");

	//	fbxのモデル
	auto& fbx = registry.emplace<FbxComponent>(enemy, res);
	fbx.CurrAnimation = "Idle";
	fbx.Mesh->SetColor(Graphics::Color::Red());

	auto col = ColliderComponent::Create<AABBCollider>();
	auto collider = col.GetPtr<AABBCollider>();
	collider->SetVolume({ 2.0f,8.0f,2.0f });
	col.Offset = { 0.0f, collider->GetVolume().y * 0.5f, 0.0f };
	registry.emplace<ColliderComponent>(enemy, std::move(col));

	//　状態管理
	auto& state = registry.emplace<EnemyStateComponent>(enemy);

	//	武器
	auto sword = CreateEnemyWeapon(enemy, "RightHand", Graphics::Color::Red());
	state.Weapon = sword;

	auto& interact = registry.emplace<InteractableComponent>(enemy);
	interact.Rank = EnemyRank::Boss;
	interact.Talkable = false;
	interact.UnlockLevel = 2;

	auto data = System::CSV::Get<SaveData>().Find(1);

	if (data->ClearLevel >= interact.UnlockLevel)
	{
		interact.Talkable = true;
	}

	//	話しかける用の画像
	std::string FilePath;
	if (interact.Talkable == true)
	{
		FilePath = "Assets/StageSelect/Texture/Red_Challenge.png";
	}
	else
	{
		FilePath = "Assets/StageSelect/Texture/Red_Require.png";
	}


	auto vfxRes = Graphics::TextureManager::GetInstance()->Load(FilePath);
	auto& vfx = registry.emplace<VfxMeshComponent>(enemy, vfxRes);
	vfx.IsShow = true;
	vfx.Offset = { 0,11,0 };

	//	タグ
	registry.emplace<EnemyTag>(enemy);
}

void Engine::System::StageSelectObjFactory::TestEffect()
{
	auto manager = EntityManager::GetInstance();
	auto& registry = EntityManager::GetInstance()->GetRegistry();

	const float Scale = 1.0f;

	auto entity = manager->CreateEntity();

	//	座標
	auto& transform = registry.emplace<Transform3D>(entity);
	transform.Position = { 0.0f,0.0f,20.0f };
	transform.Scale = { Scale ,Scale ,Scale };
	transform.Rotation = Math::Quaternion::AngleAxis(-3.14159265f / 2, Math::Vector3::Up);

	//	リソース
	//auto res = Graphics::EffectManager::GetInstance()->GetEffect("Assets/Effect/AttackHit.efk");
	//auto& effect = registry.emplace<EffectComponent>(entity);
	//effect.Asset = res;
	//effect.IsLoop = true;
	//effect.Effect.Play(effect.Asset, transform.Position);

	std::vector<Effekseer::EffectRef> Res;
	//int Count = 0;
	//int CountMax = 10;
	//while (Count < CountMax)
	//{
	//	Count++;
	//	auto path = "Assets/Effect/Sylph/" + std::to_string(Count) + ".efk";
	//	auto res = Graphics::EffectManager::GetInstance()->GetEffect(path);
	//	Res.push_back(res);
	//}

	Res.push_back(Graphics::EffectManager::GetInstance()->GetEffect("Assets/Effect/PhantasmMeteor.efk"));
	Res.push_back(Graphics::EffectManager::GetInstance()->GetEffect("Assets/Effect/Fire3.efk"));
	Res.push_back(Graphics::EffectManager::GetInstance()->GetEffect("Assets/Effect/Fire7.efk"));
	Res.push_back(Graphics::EffectManager::GetInstance()->GetEffect("Assets/Effect/Flame.efk"));
	Res.push_back(Graphics::EffectManager::GetInstance()->GetEffect("Assets/Effect/LightningStrike.efk"));
	Res.push_back(Graphics::EffectManager::GetInstance()->GetEffect("Assets/Effect/Light4.efk"));
	Res.push_back(Graphics::EffectManager::GetInstance()->GetEffect("Assets/Effect/Light3.efk"));
	Res.push_back(Graphics::EffectManager::GetInstance()->GetEffect("Assets/Effect/Blow2.efk"));
	//Res.push_back(Graphics::EffectManager::GetInstance()->GetEffect("Assets/Effect/Herald.efk"));

	float scale = 3.0f;

	for (auto& r : Res)
	{
		EffectFactory::CreateAtLocation(r,{0,1,0},{ scale ,scale ,scale },true);
	}
}
