#pragma once
#include<Plugin/entt/entt.hpp>
namespace Engine::System
{
	class ObjectsFactory
	{
		static entt::entity CreateSword(entt::entity Parent);
	public:
		static entt::entity CreatePlayer();
		static void CreateEnemy();
		static void CreateField();
	};
}