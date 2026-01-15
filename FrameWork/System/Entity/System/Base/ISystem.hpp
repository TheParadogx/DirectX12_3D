#pragma once
#include"System/Time/Time.hpp"
#include<Plugin/entt/entt.hpp>

namespace Engine::System
{
	class ISystem
	{ 
	public:
		virtual ~ISystem() = default;
		virtual void PreUpdate(entt::registry& Reg, double DeltaTime) {}
		virtual void MainUpdate(entt::registry& Reg, double DeltaTime) {}
		virtual void PostUpdate(entt::registry& Reg, double DeltaTime) {}
		virtual void Render(entt::registry& Reg) {}
		virtual void Release(){}
	};
}