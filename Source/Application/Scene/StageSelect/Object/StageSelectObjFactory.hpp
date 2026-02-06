#pragma once
#include"Graphics/Color/Color.hpp"

namespace Engine::System
{
	class StageSelectObjFactory
	{
		static entt::entity CreateEnemyWeapon(entt::entity Parent, const std::string& BoneName,const Graphics::Color& Color);
		static void CreateTalkTexture(entt::registry& Reg, entt::entity entity,bool IsTalkable, const std::string& FilePath);
	public:

		static void CreateEnemy_Basic();     // 青（最初）
		static void CreateEnemy_Advanced();  // 黄（中盤）
		static void CreateEnemy_Boss();      // 赤（最強）

		static void TestEffect();	//	エフェクト
	};
}

