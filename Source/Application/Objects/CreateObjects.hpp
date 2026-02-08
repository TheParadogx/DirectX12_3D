#pragma once
#include<Plugin/entt/entt.hpp>
#include<string>

#include"Graphics/Color/Color.hpp"

namespace Engine::System
{
	class ObjectsFactory
	{
		static entt::entity CreatePlayerWeapon(entt::entity Parent,const std::string& BoneName,int Damage);
		static entt::entity CreateEnemyWeapon(entt::entity Parent, const std::string& BoneName,int Damage,const Graphics::Color& Color);

		static entt::entity CreateSkill1();
		static entt::entity CreateSkill2();
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