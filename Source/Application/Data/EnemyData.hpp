#pragma once

#include"System/CSV/CsvLoader.hpp"

#include<string>

namespace Engine::System
{
	/// <summary>
	/// 敵のパラメーター用データ
	/// </summary>
	struct EnemyData
	{
		int id = 0;
		float MaxHP = 0;
		float Attack = 0;
		float MoveSpeed = 0;
		float EvadeSpeed = 0;
		float AttackRange = 0;
		float IdleTime = 0;

		float IdleEvadeProbability = 0;
		float CancelEvadeProbability = 0.3f; // キャンセル回避が発生する確率
		int AttackComboMax = 1;
		int  CanCancelEvade = 0;		//	キャンセル回避を行えるかどうか

		CSV_BINDABLE(EnemyData, 
			obj.id, obj.MaxHP, obj.Attack,
			obj.MoveSpeed, obj.EvadeSpeed, obj.AttackRange,
			obj.IdleTime, obj.IdleEvadeProbability, obj.CancelEvadeProbability,
			obj.AttackComboMax, obj.CanCancelEvade);

	};
}