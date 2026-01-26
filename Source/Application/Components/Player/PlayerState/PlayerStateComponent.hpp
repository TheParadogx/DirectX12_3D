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
		Dodge,		//	ダッシュ（ダッシュキー長押し）
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

		struct SprintData
		{
			int DodgeCount = 0;
			int DodgeCountMax = 2;
			float RecoveryTimer = 0.0f;
			float CoolDowmMax = 5.0f;
			float DodgeInputWindow = 0.8f;
			bool IsCoolDown = false;
		} Sprint;

	};
}