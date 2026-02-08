#include "pch.h"
#include "TimerComponentSystem.hpp"
#include"../TimerComponent.hpp"

namespace Engine::System
{
	/// <summary>
	/// ŽžŠÔŒo‰ß
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
}
