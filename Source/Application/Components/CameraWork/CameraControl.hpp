#pragma once
#include<Plugin/entt/entt.hpp>
#include"Math/Vector3/Vector3.h"

namespace Engine::System
{
	struct CameraControlComponent
	{
		//	追従する対象
		entt::entity TargetEntity = entt::null;
		//	注視点の高さ
		Math::Vector3 offset = { 0.0f,0.0f,0.0f };

		//	現在の回転状況（ラジアン）
		//	左右
		float Yaw = 0.0f;
		//	上下
		float Pitch = 0.0f;

		//	距離
		//	キャラからカメラまでの基本距離
		float Distance = 10.0f;
		//	ズームイン限界
		float MinDistance = 2.0f;
		//	ズームアウト限界
		float MaxDistance = 18.0f;

		//	制限・感度
		//	回転速度
		float LookSensitivity = 2.0f;
		//	下を向く限界（-45度くらい）
		float PitchLimitMin = -0.8f;
		//	上を向く限界（70度くらい）
		float PitchLimitMax = 1.2f;

		//	物理・特殊

	};
}