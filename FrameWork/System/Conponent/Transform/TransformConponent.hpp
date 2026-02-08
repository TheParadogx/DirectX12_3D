#pragma once
#include"Math/Vector2/Vector2.h"
#include"Math/Vector3/Vector3.h"
#include"Math/Quaternion/Quaternion.h"
#include"Math/Matrix3x3/Matrix3x3.h"

namespace Engine::System
{
	/// <summary>
	/// 2D用の座標、角度、スケール
	/// </summary>
	struct Transform2D
	{
		Math::Vector2 Position = Math::Vector2::Zero;
		Math::Vector2 Scale = Math::Vector2(1,1);
		float Rotation = 0.0f;
	};

	/// <summary>
	/// 3D用の座標、回転、スケール
	/// </summary>
	struct Transform3D
	{
		Math::Quaternion Rotation = Math::Quaternion::Identity;;
		Math::Vector3 Position;
		Math::Vector3 Scale = {1.0,1.0,1.0};

		inline Math::Vector3 GetForward() const
		{
			float yaw = atan2f(2.0f * Rotation.z * Rotation.w,
				1.0f - 2.0f * (Rotation.z * Rotation.z));

			Math::Vector3 f;
			// カメラの計算式（x=sin, z=cos）に合わせる
			f.x = sinf(yaw);
			f.y = 0.0f;
			f.z = cosf(yaw);

			return f;
		}
	};


}