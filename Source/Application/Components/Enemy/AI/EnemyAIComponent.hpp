#pragma once
#include"Application/Components/Player/PlayerState/PlayerStateComponent.hpp"

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
	/// 体力に応じた状態の変化
	/// </summary>
	enum class eEnemyPhase
	{
		Healthy,	//	通常
		Caution,	//	警戒
		Desperate,	//	瀕死
	};

	/// <summary>
	/// 敵のAI用
	/// </summary>
	struct EnemyAIComponent
	{
		//	敵の状態
		//	行動ルーチン変更用状態
		eEnemyState CurrState = eEnemyState::Idle;
		eEnemyPhase CurrPhase = eEnemyPhase::Healthy;

		//	状態待機などの時間を計る用
		float StateTimer = 0.0f;	
		//	今の攻撃の段数
		int CurrAttackCombo = 0;

		//	プレイヤーの前の状態
		ePlayerState PrevPlayerState = ePlayerState::Idle;
		//	プレイヤーの前の攻撃の段数
		int PrevPlayerAttackCombo = 0;

		//	アクションの終了した瞬間を見れるように
		bool IsActionFinished = false;

		bool IsIdle = false;

	};
}