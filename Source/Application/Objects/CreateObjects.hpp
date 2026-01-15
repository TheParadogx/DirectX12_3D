#pragma once
#include<Plugin/entt/entt.hpp>
namespace Engine::System
{
	class ObjectsFactory
	{
	public:
		static entt::entity CreatePlayer();
		static void CreateEnemy();
		static void CreateField();
		static void CreateSword(entt::entity Parent);
	};
}