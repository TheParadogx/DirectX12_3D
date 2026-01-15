#pragma once
#include"Math/Vector2/Vector2.h"
#include"Math/Vector3/Vector3.h"
#include"Math/Quaternion/Quaternion.h"
#include"Utility/FlagUtils.hpp"

#include<cstdint>

namespace Engine::System
{

	/// <summary>
	/// フラグ一覧
	/// </summary>
	enum class RigidbodyFlags : uint32_t
	{
		None = 0,
		UseGravity = 1 << 0,	//	重力計算を行うか
		IsStatic = 1 << 1,		//	絶対に動かない物体
		IsSleeping = 1 << 2,	//	特定条件まで動かない物体(計算スキップ)
		FreezeRotX = 1 << 3,	//	X回転の固定
		FreezeRotY = 1 << 4,	//	Y回転の固定
		FreezeRotZ = 1 << 5,	//	Z回転の固定
		Continuous = 1 << 6,	// 高速移動物の突き抜け防止(CCD)を有効にするか
		UseCameraBasis = 1 << 7,//	カメラ基準で動くかどうか
		IsGrounded = 1 << 8		//	着地判定

	};
	//	演算子
	DEFINE_ENUM_BITWISE_OPERATORS(RigidbodyFlags);

	/// <summary>
	/// 2D用の物理
	/// </summary>
	struct Rigidbody2D
	{

	};

	/// <summary>
	/// 3D用の物理
	/// </summary>
	struct Rigidbody3D
	{

		//	移動量
		Math::Vector3 Velocity;


		//	質量
		float Mass = 1.0f;
		//	重力のスケール
		float GravityScale = 1.0f;

		//	ビットフラグ
		RigidbodyFlags Flags = RigidbodyFlags::UseGravity | RigidbodyFlags::IsGrounded;

		//	物理計算を行うかどうか
		bool IsDynamic()const
		{
			return !(Flags & (RigidbodyFlags::IsStatic | RigidbodyFlags::IsSleeping));
		}
	};

}