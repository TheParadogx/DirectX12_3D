#pragma once
#include<cstdint>
#include<Plugin/entt/entt.hpp>

namespace Engine::System
{

	/// <summary>
	/// プレイヤーの状態
	/// </summary>
	enum class ePlayerState
	{
		Idle,		//	待機
		Run,		//	走り
		Dash,		//	ダッシュ（ダッシュキー長押し）
		Attack,		//	攻撃
		Dead,		//	死亡
	};

	/// <summary>
	/// プレイヤーの状態用コンポーネント
	/// </summary>
	struct PlayerStateComponent
	{
		//	今の状態
		ePlayerState State = ePlayerState::Idle;
		entt::entity Weapon = entt::null;
	};
}