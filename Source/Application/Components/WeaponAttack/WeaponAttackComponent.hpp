#pragma once
#include<vector>
#include<Plugin/entt/entt.hpp>
#include<set>

namespace Engine::System
{
	//	多段防止用のコンポーネント
	struct HitHistoryComponent
	{
		std::set<entt::entity>HitList;	//  衝突した敵にリスト（多段しないように）
		float HitStopMS = 0.0f;	//	ヒットストップで止める時間
	};
}