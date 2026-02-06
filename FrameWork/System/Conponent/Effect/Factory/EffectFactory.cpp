#include "pch.h"
#include "EffectFactory.hpp"
#include"System/Conponent/Transform/TransformConponent.hpp"
#include"System/Conponent/Effect/EffectComponent.hpp"

#include"System/Entity/Manager/EntityManager.hpp"


entt::entity Engine::System::EffectFactory::CreateAtLocation(Effekseer::EffectRef Asset, const Math::Vector3& Position, const Math::Vector3& Scale, bool IsLoop)
{
	auto manager = EntityManager::GetInstance();
	auto& registry = EntityManager::GetInstance()->GetRegistry();

	auto entity = manager->CreateEntity();
	auto& transform = registry.emplace<Transform3D>(entity);
	transform.Position = Position;
	transform.Scale = Scale;
	transform.Rotation = Math::Quaternion::Identity;

	auto& effect = registry.emplace<EffectComponent>(entity);
	effect.Asset = Asset;
	effect.IsLoop = IsLoop;
	effect.Effect.SetScale(Scale);
	effect.Effect.Play(effect.Asset, transform.Position);


	return entity;
}

entt::entity Engine::System::EffectFactory::CreateAttached(Effekseer::EffectRef Asset, entt::entity Parent, const Math::Vector3& Offset, bool IsLoop)
{
	auto manager = EntityManager::GetInstance();
	auto& registry = EntityManager::GetInstance()->GetRegistry();
	auto entity = manager->CreateEntity();

	auto& effect = registry.emplace<EffectComponent>(entity);
	effect.Asset = Asset;
	effect.Offset = Offset;
	effect.IsLoop = IsLoop;
	effect.Parent = Parent;

	return entity;
}
