#pragma once
#include<vector>
#include<Plugin/entt/entt.hpp>

namespace Engine::System
{
	//	武器にアタッチする攻撃用コンポーネント
	struct WeaponAttackComponent
	{
		std::vector<entt::entity> HitList;	//  衝突した敵にリスト（多段しないように）
		float HitStopMS = 0.1f;	//	ヒットストップで止める時間
	};
}