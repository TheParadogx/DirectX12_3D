#pragma once
#include"Math/Vector3/Vector3.h"
#include"Utility/FlagUtils.hpp"
#include<cstdint>

namespace Engine::System
{
	enum class MoveFlags : uint32_t
	{
		None = 0,

		//	状態フラグ
		IsGrounded = 1 << 0,
		IsJumping = 1 << 1,
		IsFalling = 1 << 2,
		IsRunning = 1 << 3,
		IsMoveing = 1 << 4,

		//	制限フラグ
		CanMove = 1 << 5,
		CanJump  = 1 << 6,
		CanTurn = 1 << 7,	//	向きの変更可能かどうか(実質視点)
		CanRunning = 1 << 8,

		//	特殊状態
		InputDisabled = 1 << 9,
		UseCameraBasis = 1 << 10,

		//	プリセット
		Default = CanMove | CanJump | CanTurn | CanRunning
	};
	DEFINE_ENUM_BITWISE_OPERATORS(MoveFlags);

	/// <summary>
	/// 入力で移動させるコンポーネント
	/// </summary>
	struct MoveComponent
	{
		//	入力での移動量
		Math::Vector3 TargetDir;                                                                               

		//	移動速度
		float MoveSpeed = 5.0f;
		//	ダッシュ時の加速倍率
		float SprintMulti = 1.8f;
		//	ジャンプの初速
		float JumpPower = 6.0f;
		//	回転速度
		float TurnSpeed = 20.0f;

		//	カメラを基準として動くかどうか
		bool UseCameraBasis = true;

	};
}