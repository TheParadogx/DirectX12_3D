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
		Skill,		//	スキル
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

		//	回避
		struct DodgeData
		{
			int DodgeCount = 0;
			int DodgeCountMax = 2;
			float RecoveryTimer = 0.0f;
			float CoolDowmMax = 5.0f;
			float DodgeInputWindow = 0.8f;
			float MoveSpeed = 0.0f;
			bool IsCoolDown = false;
		};

		//	攻撃
		struct AttackData
		{
			int AttackCount = 0;	//	連続攻撃回数
			int AttackCountMax = 4;	//	連続攻撃の最大数
			float AttackInputTimer = 0.0f;	//	入力受けつけ時間
			float AttackInputWindow = 0.1f;	//	連続攻撃入力受付制限時間
			bool AttackQueued = false;	
		};

		DodgeData Dodge;
		AttackData Attack;

	};
}