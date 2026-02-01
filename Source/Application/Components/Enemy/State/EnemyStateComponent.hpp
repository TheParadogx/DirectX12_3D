#pragma once
#include<entt/entt.hpp>

namespace Engine::System
{
	//	敵の状態
	enum class eEnemyState {
		Idle,
		Chasing,
		Attacking,
		Evade,
		CancelEvade,	//	全ての行動をキャンセルして回避する。
	};
	
	/// <summary>
	/// 敵の状態管理
	/// </summary>
	struct EnemyStateComponent
	{
		//	状態
		eEnemyState State = eEnemyState::Idle;

		//	武器
		entt::entity Weapon = entt::null;
	};
}