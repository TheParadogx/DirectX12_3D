#pragma once
#include"entt/entt.hpp"
#include"Application/Objects/EnemyRank.hpp"

namespace Engine::System
{
	/// <summary>
	/// 敵の状態管理用の固有パラメーター
	/// </summary>
	struct EnemyParameters
	{
		float MoveSpeed = 10.0f;	//	走る速度
		float AttackRange = 10.0f;	//	攻撃に入る距離
		float IdleTime = 1.0f;	//	攻撃後の硬直時間

		float IdleEvadeProbability = 0.3f;	//	待機中に回避する確率
		float CancelEvadeProbability = 0.3f; // キャンセル回避が発生する確率
		int AttackComboMax = 1;
		entt::entity Weapon = entt::null;	//	武器
		EnemyRank Rank = EnemyRank::Basic;	//	敵のランク
		bool  CanCancelEvade = false;		//	キャンセル回避を行えるかどうか
	};
}