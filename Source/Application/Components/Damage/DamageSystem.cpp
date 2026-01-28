#include "pch.h"
#include "DamageSystem.hpp"
#include "AttackPowerComponent.hpp"
#include"Application/Components/Status/StatusComponent.hpp"
#include"System/Collider/CollisionEvents/CollisionEvent.hpp"
#include"Application/Components/Tag/TagComponent.hpp"
#include"Application/Components/WeaponAttack/WeaponAttackComponent.hpp"
#include"Application/Components/Invincible/InvincibleTag.hpp"

/// <summary>
/// ダメージ処理
/// </summary>
/// <param name="Reg"></param>
/// <param name="DeltaTime"></param>
void Engine::System::DamageSystem::PostUpdate(entt::registry& Reg, double DeltaTime)
{
	//	自分が攻撃力を持っていて誰かと当たった時
	auto view = Reg.view<AttackPowerComponent, CollisionEvent>();
	view.each([&](entt::entity entity, AttackPowerComponent& attack, CollisionEvent& collision)
		{
			HitHistoryComponent* history = Reg.try_get<HitHistoryComponent>(entity);

			//	被害者
			for (auto victim : collision.HitEntitys)
			{
				//	既に衝突 or 履歴保持がないときは終了
				if (history != nullptr && history->HitList.contains(victim))
				{
					continue;
				}

				//	無敵状態なら終了
				if (Reg.all_of<InvincibleComponet>(victim)) 
				{
					continue;
				}

				//	相手が体力を持っているかどうか
				if (auto* status = Reg.try_get<StatusComponet>(victim))
				{
					status->ApplyDamage(attack.DamageValue);
					history->HitList.insert(victim);

					if (status->IsDead())
					{
						Reg.emplace_or_replace<DeadTag>(victim);
					}
				}
			}
		});


}
