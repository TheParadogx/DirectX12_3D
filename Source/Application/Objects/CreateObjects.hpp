#pragma once
#include<Plugin/entt/entt.hpp>
#include<string>

namespace Engine::System
{
	class ObjectsFactory
	{
		static entt::entity CreatePlayerWeapon(entt::entity Parent,const std::string& BoneName);
		static entt::entity CreateEnemyWeapon(entt::entity Parent, const std::string& BoneName);

		static entt::entity CreateSkill1();
	public:
		static entt::entity CreatePlayer();
		static void CreateEnemy();
		static void CreateEnemy_Basic();     // 青（最初）
		static void CreateEnemy_Advanced();  // 黄（中盤）
		static void CreateEnemy_Boss();      // 赤（最強）
		static void CreateField();
		static void CreateTest();
	};
}