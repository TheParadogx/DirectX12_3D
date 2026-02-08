#include "pch.h"
#include "TimerComponentSystem.hpp"
#include"../TimerComponent.hpp"

#include"System/Entity/Manager/EntityManager.hpp"

namespace Engine::System
{
	/// <summary>
	/// 時間経過
	/// </summary>
	/// <param name="DeltaTime"></param>
	void TimerComponentSystem::PreUpdate(entt::registry& Reg,double DeltaTime)
	{
		auto view = Reg.view<TimerComponent>();
		view.each([&](TimerComponent& timer) 
			{
				timer.Timer += DeltaTime;
			});
	}

	/// <summary>
	/// タイマーとなるエンティティの作成
	/// </summary>
	/// <returns></returns>
	entt::entity TimerComponentSystem::CreateTimerObject()
	{
		entt::registry& Reg = EntityManager::GetInstance()->GetRegistry();
		entt::entity entity = Reg.create();

		Reg.emplace<TimerComponent>(entity);
		return entity;
	}
}
