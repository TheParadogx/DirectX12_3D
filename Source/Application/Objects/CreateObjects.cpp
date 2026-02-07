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
#include"Application/Components/Damage/AttackPowerComponent.hpp"

#include"Application/Components/Player/PlayerState/PlayerStateComponent.hpp"
#include"Application/Components/Player/Input/InputRequestComponent.hpp"

#include "Application/Components/Enemy/State/EnemyStateComponent.hpp"
#include "Application/Components/Enemy/AI/EnemyAIComponent.hpp"
#include "Application/Components/Enemy/Parameter/EnemyParametersComponent.hpp"


#include"Application/Components/Tag/TagComponent.hpp"
#include"Application/Components/StageComponent/StateComponent.hpp"

#include"System/Conponent/Effect/EffectComponent.hpp"
#include"Graphics/Effect/Manager/EffectManager.hpp"
#include"Application/Components/Skill/SkillComponent.hpp"


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

	auto res = Graphics::FbxResourceManager::GetInstance()->Load("Assets/Fbx/Ganfaul/Faul.fbx.bin");
	res->LoadAnimation("Idle", "Assets/Fbx/Ganfaul/Animation/Idle.fbx.anm");
	res->LoadAnimation("Jog", "Assets/Fbx/Ganfaul/Animation/Jog.fbx.anm");
	res->LoadAnimation("Dodge", "Assets/Fbx/Ganfaul/Animation/Dodge.fbx.anm");
	res->LoadAnimation("Attack_0", "Assets/Fbx/Ganfaul/Animation/Attack_A.fbx.anm");
	res->LoadAnimation("Attack_1", "Assets/Fbx/Ganfaul/Animation/Attack_B.fbx.anm");
	res->LoadAnimation("Attack_2", "Assets/Fbx/Ganfaul/Animation/Attack_C.fbx.anm");
	res->LoadAnimation("Attack_3", "Assets/Fbx/Ganfaul/Animation/Attack_D.fbx.anm");


	//	fbxのモデル
	auto& fbx = registry.emplace<FbxComponent>(player, res);
	fbx.CurrAnimation = "Idle";

	//	当たり判定
	auto col = ColliderComponent::Create<AABBCollider>();
	auto collider = col.GetPtr<AABBCollider>();
	collider->SetVolume({ 2.0f,8.0f,2.0f });
	col.Offset = { 0.0f, collider->GetVolume().y * 0.5f, 0.0f };
	col.BoneName = "Head";
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
	auto& hpBar = registry.emplace<System::HpRenderComponent>
		(player, baseRes, barRes, Math::Vector2(0.0,0.0), Graphics::Color::Lime());
	hpBar.SetPosition({730,978});
	hpBar.SetSize({ 480,12 });

	//	入力
	registry.emplace<System::InputRequestComponent>(player);
	//	状態
	auto& state = registry.emplace<System::PlayerStateComponent>(player);

	//	移動
	auto& move = registry.emplace<System::MoveComponent>(player);
	move.MoveSpeed = 20.0f;

	//	武器
	auto sword = CreatePlayerWeapon(player,"RightHand");
	state.Weapon = sword;

	//	タグ
	registry.emplace<PlayerTag>(player);

	//	スキル保持
	auto& skillHolder = registry.emplace<SkillSlotComponent>(player);
	skillHolder.SkillSlots[0] = CreateSkill1();
	skillHolder.SkillSlots[1] = CreateSkill2();

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

	//	移動量（物理）
	auto& rigidbody = registry.emplace<Rigidbody3D>(enemy);


	//	素材テスト
	auto res = Graphics::FbxResourceManager::GetInstance()->Load("Assets/Fbx/Ganfaul/Faul.fbx.bin");
	res->LoadAnimation("Idle", "Assets/Fbx/Ganfaul/Animation/Idle.fbx.anm");
	res->LoadAnimation("Jog", "Assets/Fbx/Ganfaul/Animation/Jog.fbx.anm");
	res->LoadAnimation("Dodge", "Assets/Fbx/Ganfaul/Animation/Dodge.fbx.anm");
	res->LoadAnimation("Attack_0", "Assets/Fbx/Ganfaul/Animation/Attack_A.fbx.anm");
	res->LoadAnimation("Attack_1", "Assets/Fbx/Ganfaul/Animation/Attack_B.fbx.anm");
	res->LoadAnimation("Attack_2", "Assets/Fbx/Ganfaul/Animation/Attack_C.fbx.anm");
	res->LoadAnimation("Attack_3", "Assets/Fbx/Ganfaul/Animation/Attack_D.fbx.anm");

	//	fbxのモデル
	auto& fbx = registry.emplace<FbxComponent>(enemy, res);
	fbx.CurrAnimation = "Idle";
	fbx.Mesh->SetColor(Graphics::Color::Red());

	//	ステータス
	auto& status = registry.emplace<System::StatusComponet>(enemy);
	status.MaxHp.Base = 1000;
	status.Hp = 1000;

	//	画像
	auto baseRes = Graphics::TextureManager::GetInstance()->Load("Assets/Texture/HPBar/BarBase.png");
	auto barRes = Graphics::TextureManager::GetInstance()->Load("Assets/Texture/HPBar/Bar.png");

	//	体力バー
	auto& hpBar = registry.emplace<System::HpRenderComponent>(enemy, baseRes, barRes, Math::Vector2(0.0, 0.0));
	hpBar.SetPosition({ 100,100 });

	auto col = ColliderComponent::Create<AABBCollider>();
	auto collider = col.GetPtr<AABBCollider>();
	collider->SetVolume({ 2.0f,8.0f,2.0f });
	col.Offset = { 0.0f, collider->GetVolume().y * 0.5f, 0.0f };
	registry.emplace<ColliderComponent>(enemy, std::move(col));

	//　状態管理
	auto& state = registry.emplace<EnemyStateComponent>(enemy);

	//	武器
	auto sword = CreateEnemyWeapon(enemy, "RightHand");
	state.Weapon = sword;

	//	タグ
	registry.emplace<EnemyTag>(enemy);

}

/// <summary>
/// 最弱の青色の敵
/// </summary>
void Engine::System::ObjectsFactory::CreateEnemy_Basic()
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

	//	移動量（物理）
	auto& rigidbody = registry.emplace<Rigidbody3D>(enemy);


	//	素材テスト
	auto res = Graphics::FbxResourceManager::GetInstance()->Load("Assets/Fbx/Ganfaul/Faul.fbx.bin");
	res->LoadAnimation("Idle", "Assets/Fbx/Ganfaul/Animation/Idle.fbx.anm");
	res->LoadAnimation("Jog", "Assets/Fbx/Ganfaul/Animation/Jog.fbx.anm");
	res->LoadAnimation("Dodge", "Assets/Fbx/Ganfaul/Animation/Dodge.fbx.anm");
	res->LoadAnimation("Attack_0", "Assets/Fbx/Ganfaul/Animation/Attack_A.fbx.anm");
	res->LoadAnimation("Attack_1", "Assets/Fbx/Ganfaul/Animation/Attack_B.fbx.anm");
	res->LoadAnimation("Attack_2", "Assets/Fbx/Ganfaul/Animation/Attack_C.fbx.anm");
	res->LoadAnimation("Attack_3", "Assets/Fbx/Ganfaul/Animation/Attack_D.fbx.anm");

	//	fbxのモデル
	auto& fbx = registry.emplace<FbxComponent>(enemy, res);
	fbx.CurrAnimation = "Idle";
	fbx.Mesh->SetColor(Graphics::Color::Cyan());

	//	ステータス
	auto& status = registry.emplace<System::StatusComponet>(enemy);
	status.MaxHp.Base = 1000;
	status.Hp = 1000;

	//	画像
	auto baseRes = Graphics::TextureManager::GetInstance()->Load("Assets/Texture/HPBar/BarBase.png");
	auto barRes = Graphics::TextureManager::GetInstance()->Load("Assets/Texture/HPBar/Bar.png");

	//	体力バー
	auto& hpBar = registry.emplace<System::HpRenderComponent>
		(enemy, baseRes, barRes, Math::Vector2(0.0, 0.0), Graphics::Color::CoralRed());
	hpBar.SetPosition({ 622,126 });
	hpBar.SetSize({ 629,12 });
	hpBar.isColorOverridden = true;

	auto col = ColliderComponent::Create<AABBCollider>();
	auto collider = col.GetPtr<AABBCollider>();
	collider->SetVolume({ 2.0f,8.0f,2.0f });
	col.Offset = { 0.0f, collider->GetVolume().y * 0.5f, 0.0f };
	col.BoneName = "Head";
	registry.emplace<ColliderComponent>(enemy, std::move(col));

	//　状態管理
	auto& state = registry.emplace<EnemyStateComponent>(enemy);

	//	AI
	auto& ai = registry.emplace<EnemyAIComponent>(enemy);
	//	パラメーター
	auto& param = registry.emplace<EnemyParameters>(enemy);
	param.Rank = EnemyRank::Basic;

	//	武器
	auto sword = CreateEnemyWeapon(enemy, "RightHand");
	param.Weapon = sword;



	//	タグ
	registry.emplace<EnemyTag>(enemy);
	registry.emplace<BasicTag>(enemy);


}

/// <summary>
/// ちょっと強い黄色の敵
/// </summary>
void Engine::System::ObjectsFactory::CreateEnemy_Advanced()
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

	//	移動量（物理）
	auto& rigidbody = registry.emplace<Rigidbody3D>(enemy);


	//	素材テスト
	auto res = Graphics::FbxResourceManager::GetInstance()->Load("Assets/Fbx/Ganfaul/Faul.fbx.bin");
	res->LoadAnimation("Idle", "Assets/Fbx/Ganfaul/Animation/Idle.fbx.anm");
	res->LoadAnimation("Jog", "Assets/Fbx/Ganfaul/Animation/Jog.fbx.anm");
	res->LoadAnimation("Dodge", "Assets/Fbx/Ganfaul/Animation/Dodge.fbx.anm");
	res->LoadAnimation("Attack_0", "Assets/Fbx/Ganfaul/Animation/Attack_A.fbx.anm");
	res->LoadAnimation("Attack_1", "Assets/Fbx/Ganfaul/Animation/Attack_B.fbx.anm");
	res->LoadAnimation("Attack_2", "Assets/Fbx/Ganfaul/Animation/Attack_C.fbx.anm");
	res->LoadAnimation("Attack_3", "Assets/Fbx/Ganfaul/Animation/Attack_D.fbx.anm");

	//	fbxのモデル
	auto& fbx = registry.emplace<FbxComponent>(enemy, res);
	fbx.CurrAnimation = "Idle";
	fbx.Mesh->SetColor(Graphics::Color::Yellow());

	//	ステータス
	auto& status = registry.emplace<System::StatusComponet>(enemy);
	status.MaxHp.Base = 1000;
	status.Hp = 1000;

	//	画像
	auto baseRes = Graphics::TextureManager::GetInstance()->Load("Assets/Texture/HPBar/BarBase.png");
	auto barRes = Graphics::TextureManager::GetInstance()->Load("Assets/Texture/HPBar/Bar.png");

	//	体力バー
	auto& hpBar = registry.emplace<System::HpRenderComponent>
		(enemy, baseRes, barRes, Math::Vector2(0.0, 0.0), Graphics::Color::CoralRed());
	hpBar.SetPosition({ 622,126 });
	hpBar.SetSize({ 629,12 });
	hpBar.isColorOverridden = true;

	auto col = ColliderComponent::Create<AABBCollider>();
	auto collider = col.GetPtr<AABBCollider>();
	collider->SetVolume({ 2.0f,8.0f,2.0f });
	col.Offset = { 0.0f, collider->GetVolume().y * 0.5f, 0.0f };
	col.BoneName = "Head";
	registry.emplace<ColliderComponent>(enemy, std::move(col));

	//　状態管理
	auto& state = registry.emplace<EnemyStateComponent>(enemy);

	//	AI
	auto& ai = registry.emplace<EnemyAIComponent>(enemy);
	//	パラメーター
	auto& param = registry.emplace<EnemyParameters>(enemy);
	param.Rank = EnemyRank::Advanced;

	//	武器
	auto sword = CreateEnemyWeapon(enemy, "RightHand");
	param.Weapon = sword;



	//	タグ
	registry.emplace<EnemyTag>(enemy);
	registry.emplace<AdvancedTag>(enemy);



}

/// <summary>
/// 最強のボス（赤）
/// </summary>
void Engine::System::ObjectsFactory::CreateEnemy_Boss()
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

	//	移動量（物理）
	auto& rigidbody = registry.emplace<Rigidbody3D>(enemy);


	//	素材テスト
	auto res = Graphics::FbxResourceManager::GetInstance()->Load("Assets/Fbx/Ganfaul/Faul.fbx.bin");
	res->LoadAnimation("Idle", "Assets/Fbx/Ganfaul/Animation/Idle.fbx.anm");
	res->LoadAnimation("Jog", "Assets/Fbx/Ganfaul/Animation/Jog.fbx.anm");
	res->LoadAnimation("Dodge", "Assets/Fbx/Ganfaul/Animation/Dodge.fbx.anm");
	res->LoadAnimation("Attack_0", "Assets/Fbx/Ganfaul/Animation/Attack_A.fbx.anm");
	res->LoadAnimation("Attack_1", "Assets/Fbx/Ganfaul/Animation/Attack_B.fbx.anm");
	res->LoadAnimation("Attack_2", "Assets/Fbx/Ganfaul/Animation/Attack_C.fbx.anm");
	res->LoadAnimation("Attack_3", "Assets/Fbx/Ganfaul/Animation/Attack_D.fbx.anm");

	//	fbxのモデル
	auto& fbx = registry.emplace<FbxComponent>(enemy, res);
	fbx.CurrAnimation = "Idle";
	fbx.Mesh->SetColor(Graphics::Color::Red());

	//	ステータス
	auto& status = registry.emplace<System::StatusComponet>(enemy);
	status.MaxHp.Base = 1000;
	status.Hp = 1000;

	//	画像
	auto baseRes = Graphics::TextureManager::GetInstance()->Load("Assets/Texture/HPBar/BarBase.png");
	auto barRes = Graphics::TextureManager::GetInstance()->Load("Assets/Texture/HPBar/Bar.png");

	//	体力バー
	auto& hpBar = registry.emplace<System::HpRenderComponent>
		(enemy, baseRes, barRes, Math::Vector2(0.0, 0.0), Graphics::Color::CoralRed());
	hpBar.SetPosition({ 622,126 });
	hpBar.SetSize({ 629,12 });
	hpBar.isColorOverridden = true;

	auto col = ColliderComponent::Create<AABBCollider>();
	auto collider = col.GetPtr<AABBCollider>();
	collider->SetVolume({ 2.0f,8.0f,2.0f });
	col.Offset = { 0.0f, collider->GetVolume().y * 0.5f, 0.0f };	
	col.BoneName = "Head";
	registry.emplace<ColliderComponent>(enemy, std::move(col));

	//　状態管理
	auto& state = registry.emplace<EnemyStateComponent>(enemy);

	//	AI
	auto& ai = registry.emplace<EnemyAIComponent>(enemy);
	//	パラメーター
	auto& param = registry.emplace<EnemyParameters>(enemy);
	param.AttackComboMax = 4;
	param.IdleTime = 0.7f;
	param.Rank = EnemyRank::Boss;
	param.CanCancelEvade = true;


	//	武器
	auto sword = CreateEnemyWeapon(enemy, "RightHand");
	param.Weapon = sword;



	//	タグ
	registry.emplace<EnemyTag>(enemy);
	registry.emplace<BossTag>(enemy);

}


void Engine::System::ObjectsFactory::CreateField()
{
	auto manager = EntityManager::GetInstance();
	auto& registry = EntityManager::GetInstance()->GetRegistry();

	auto field = manager->CreateEntity();

	//	サイズ
	const float volume = 1.8f;

	//	座標
	auto& trans = registry.emplace<Transform3D>(field);
	trans.Position = { 0.0f,0.0f,0.0f };
	trans.Scale = { volume,volume,volume };

	//	fbx
	auto res = Graphics::FbxResourceManager::GetInstance()->Load("Assets/Fbx/Field/Field.fbx.bin");
	auto& fbx = registry.emplace<FbxComponent>(field, res,false);

	auto& stageConfig = registry.emplace<StageComponent>(field);
	stageConfig.PlayableRadius = 80.f;

	//	stage
	registry.emplace<StageTag>(field);

}

void Engine::System::ObjectsFactory::CreateTest()
{
	auto manager = EntityManager::GetInstance();
	auto& registry = EntityManager::GetInstance()->GetRegistry();

	const float Scale = 0.05f;

	auto test = manager->CreateEntity();

	//	座標
	auto& transform = registry.emplace<Transform3D>(test);
	transform.Position = { 0.0f,0.0f,-20.0f };
	transform.Scale = { Scale ,Scale ,Scale };
	transform.Rotation = Math::Quaternion::Identity;

	//	素材テスト
	auto res = Graphics::FbxResourceManager::GetInstance()->Load("Assets/Fbx/Ganfaul/Faul.fbx.bin");
	res->LoadAnimation("Idle", "Assets/Fbx/Ganfaul/Animation/Idle.fbx.anm");
	res->LoadAnimation("Jog", "Assets/Fbx/Ganfaul/Animation/Jog.fbx.anm");
	res->LoadAnimation("Dodge", "Assets/Fbx/Ganfaul/Animation/Dodge.fbx.anm");
	res->LoadAnimation("Attack_0", "Assets/Fbx/Ganfaul/Animation/Attack_A.fbx.anm");
	res->LoadAnimation("Attack_1", "Assets/Fbx/Ganfaul/Animation/Attack_B.fbx.anm");
	res->LoadAnimation("Attack_2", "Assets/Fbx/Ganfaul/Animation/Attack_C.fbx.anm");
	res->LoadAnimation("Attack_3", "Assets/Fbx/Ganfaul/Animation/Attack_D.fbx.anm");

	//	fbxのリソース
	//auto res = Graphics::FbxResourceManager::GetInstance()->Load("Assets/Mannequin/SKM_Manny_Simple.FBX.bin");
	//res->LoadAnimation("Idle", "Assets/Mannequin/Animation/MM_Idle.FBX.anm");
	//res->LoadAnimation("Run", "Assets/Mannequin/Animation/MM_Run_Fwd.FBX.anm");
	//res->LoadAnimation("Jump", "Assets/Mannequin/Animation/MM_Jump.FBX.anm");
	//res->LoadAnimation("Attack", "Assets/Mannequin/Animation/SwordSlash.fbx.anm");



	//	fbxのモデル
	auto& fbx = registry.emplace<FbxComponent>(test, res);
	fbx.CurrAnimation = "Attack_3";
	//fbx.Mesh->SetColor(Graphics::Color::Red());

	auto col = ColliderComponent::Create<AABBCollider>();
	auto collider = col.GetPtr<AABBCollider>();
	collider->SetVolume({ 2.0f,8.0f,2.0f });
	col.Offset = { 0.0f, collider->GetVolume().y * 0.5f, 0.0f };
	registry.emplace<ColliderComponent>(test, std::move(col));

	//	タグ
	//registry.emplace<EnemyTag>(test);

}

entt::entity Engine::System::ObjectsFactory::CreatePlayerWeapon(entt::entity Parent, const std::string& BoneName)
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
	//auto res = Graphics::FbxResourceManager::GetInstance()->Load("Assets/Sword/MM_Sword.fbx.bin");


	//	fbxのモデル
	auto& fbx = registry.emplace<FbxComponent>(sword, res, false);
	fbx.CurrAnimation = "";
	//fbx.Mesh->SetColor(Graphics::Color::Green);

	//	当たり判定
	//	いったん正常に動作するかどうか

	//	アタッチ
	auto& socket = registry.emplace<SocketComponent>(sword);
	socket.Parent = Parent;
	socket.BoneName = BoneName;
	socket.OffsetPos = {1.82,0.51,-0.46 };
	socket.OffsetRot = { -0.7,-0.685,0.192,0.007 };
	socket.PivotOffset = { 0,1.98,0.0 };

	auto effectRes = Graphics::EffectManager::GetInstance()->GetEffect("Assets/Effect/AttackHit.efk");
	auto windEffect = Graphics::EffectManager::GetInstance()->GetEffect("Assets/Effect/Sword1.efk");
	auto windEffect2 = Graphics::EffectManager::GetInstance()->GetEffect("Assets/Effect/Sword6.efk");

	auto& damage = registry.emplace<AttackPowerComponent>(sword);
	damage.DamageValue = 1;
	damage.HitEffectAsset.push_back(effectRes);
	damage.HitEffectAsset.push_back(windEffect);
	damage.HitEffectAsset.push_back(windEffect2);
	damage.Offset = { 0,5,0 };
	damage.Scale = { 2,2,2 };

	registry.emplace<PlayerWeaponTag>(sword);

	return sword;
}

entt::entity Engine::System::ObjectsFactory::CreateEnemyWeapon(entt::entity Parent, const std::string& BoneName)
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
	fbx.Mesh->SetColor(Graphics::Color::Red());

	//	アタッチ
	auto& socket = registry.emplace<SocketComponent>(sword);
	socket.Parent = Parent;
	socket.BoneName = BoneName;
	socket.OffsetPos = { 1.82,0.51,-0.46 };
	socket.OffsetRot = { -0.7,-0.685,0.192,0.007 };
	socket.PivotOffset = { 0,1.98,0.0 };

	auto& damage = registry.emplace<AttackPowerComponent>(sword);
	damage.DamageValue = 1;

	registry.emplace<EnemyWeaponTag>(sword);
	

	return sword;
}

/// <summary>
/// スキル１
/// </summary>
/// <returns></returns>
entt::entity Engine::System::ObjectsFactory::CreateSkill1()
{
	auto manager = EntityManager::GetInstance();
	auto& registry = EntityManager::GetInstance()->GetRegistry();

	auto entity = manager->CreateEntity();

	//	スキル
	auto& skill = registry.emplace<SkillComponent>(entity);

	//	エフェクトの読み込み
	skill.Effects.push_back(Graphics::EffectManager::GetInstance()->GetEffect("Assets/Effect/Sylph/1.efk"));
	skill.Effects.push_back(Graphics::EffectManager::GetInstance()->GetEffect("Assets/Effect/Sylph/2.efk"));
	skill.Effects.push_back(Graphics::EffectManager::GetInstance()->GetEffect("Assets/Effect/Sylph/3.efk"));
	skill.Effects.push_back(Graphics::EffectManager::GetInstance()->GetEffect("Assets/Effect/Sylph/4.efk"));
	skill.Effects.push_back(Graphics::EffectManager::GetInstance()->GetEffect("Assets/Effect/Sylph/6.efk"));
	skill.Effects.push_back(Graphics::EffectManager::GetInstance()->GetEffect("Assets/Effect/Sylph/7.efk"));
	skill.Effects.push_back(Graphics::EffectManager::GetInstance()->GetEffect("Assets/Effect/Sylph/8.efk"));
	skill.Effects.push_back(Graphics::EffectManager::GetInstance()->GetEffect("Assets/Effect/Sylph/9.efk"));
	skill.Effects.push_back(Graphics::EffectManager::GetInstance()->GetEffect("Assets/Effect/Sylph/10.efk"));
	skill.Effects.push_back(Graphics::EffectManager::GetInstance()->GetEffect("Assets/Effect/Tornade.efk"));


	auto& trans = registry.emplace<Transform3D>(entity);

	auto& damage = registry.emplace<AttackPowerComponent>(entity);
	damage.DamageValue = 250.0f;
	damage.HitEffectAsset.push_back(Graphics::EffectManager::GetInstance()->GetEffect("Assets/Effect/Skill1Hit.efk"));

	registry.emplace<PlayerWeaponTag>(entity);

	return entity;
}

entt::entity Engine::System::ObjectsFactory::CreateSkill2()
{
	auto manager = EntityManager::GetInstance();
	auto& registry = EntityManager::GetInstance()->GetRegistry();

	auto entity = manager->CreateEntity();

	//	スキル
	auto& skill = registry.emplace<SkillComponent>(entity);

	//	エフェクトの読み込み
	skill.Effects.push_back(Graphics::EffectManager::GetInstance()->GetEffect("Assets/Effect/Fire3.efk"));
	skill.Effects.push_back(Graphics::EffectManager::GetInstance()->GetEffect("Assets/Effect/Fire7.efk"));
	skill.Effects.push_back(Graphics::EffectManager::GetInstance()->GetEffect("Assets/Effect/Flame.efk"));

	auto& trans = registry.emplace<Transform3D>(entity);

	auto& damage = registry.emplace<AttackPowerComponent>(entity);
	damage.DamageValue = 250.0f;
	damage.HitEffectAsset.push_back(Graphics::EffectManager::GetInstance()->GetEffect("Assets/Effect/Skill1Hit.efk"));

	registry.emplace<PlayerWeaponTag>(entity);

	return entity;
}
