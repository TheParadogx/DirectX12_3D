#pragma once
#include<Plugin/entt/entt.hpp>
#include<string>

namespace Engine::System
{
	class ObjectsFactory
	{
		static entt::entity CreateSword(entt::entity Parent,const std::string& BoneName);
	public:
		static entt::entity CreatePlayer();
		static void CreateEnemy();
		static void CreateField();
		static void CreateTest();
	};
}