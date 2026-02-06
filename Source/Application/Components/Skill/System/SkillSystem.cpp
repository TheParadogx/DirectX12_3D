#include "pch.h"
#include "SkillSystem.hpp"
#include"../SkillComponent.hpp"
#include"System/Conponent/Collider/ColliderComponent.hpp"
#include"Application/Components/WeaponAttack/WeaponAttackComponent.hpp"

/// <summary>
/// クールタイムや終了判定
/// </summary>
/// <param name="Reg"></param>
/// <param name="DeltaTime"></param>
void Engine::System::SkillSystem::MainUpdate(entt::registry& Reg, double DeltaTime)
{
	auto view = Reg.view<SkillComponent>();
	view.each([&](auto entity, SkillComponent& skill)
		{
			//	CT
			if (skill.CooldownTimer > 0.0f)
			{
				skill.CooldownTimer -= DeltaTime;
				if (skill.CooldownTimer < 0.0f)
				{
					skill.CooldownTimer = 0.0f;
				}
			}

			//	アクティブタグがついている時の処理
			if (ActiveSkillTag* active = Reg.try_get<ActiveSkillTag>(entity))
			{
				active->ElapsedTime += DeltaTime;
				if (active->ElapsedTime >= active->Duration)
				{
					if (Reg.all_of<ColliderComponent>(entity))
					{
						Reg.remove<ColliderComponent>(entity);
					}

					// ヒット履歴も忘れずにリセット（次の発動のため）
					if (Reg.all_of<HitHistoryComponent>(entity))
					{
						Reg.remove<HitHistoryComponent>(entity);
					}

					//	アクティブのデタッチ
					Reg.remove<ActiveSkillTag>(entity);
				}

			}

		});

}
