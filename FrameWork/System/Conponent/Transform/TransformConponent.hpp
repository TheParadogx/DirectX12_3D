#pragma once
#include"Math/Vector2/Vector2.h"
#include"Math/Vector3/Vector3.h"
#include"Math/Quaternion/Quaternion.h"

namespace Engine::System
{
	/// <summary>
	/// 2D用の座標、角度、スケール
	/// </summary>
	struct Transform2D
	{
		Math::Vector2 Position;
		Math::Vector2 Scale;
		float Rotation;
	};

	/// <summary>
	/// 3D用の座標、回転、スケール
	/// </summary>
	struct Transform3D
	{
		Math::Quaternion Rotation = Math::Quaternion::Identity;;
		Math::Vector3 Position;
		Math::Vector3 Scale = {1.0,1.0,1.0};
	};


}