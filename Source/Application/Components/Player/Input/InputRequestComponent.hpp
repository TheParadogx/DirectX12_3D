#pragma once
#include"Math/Vector3/Vector3.h"
#include"Utility/FlagUtils.hpp"
#include<cstdint>

namespace Engine::System
{
	enum class ActionInputFlags :uint32_t
	{
		None = 0,

		//	判定フラグ
		RunRequested = 1 << 0,		//	走り
		DashRequested = 1 << 1,		//	ダッシュ
		AttackRequested = 1 << 2,	//	攻撃
	};
	DEFINE_ENUM_BITWISE_OPERATORS(ActionInputFlags);

	struct InputRequestComponent
	{
		//	入力での移動量
		Math::Vector3 InputVec;

		//	判定用フラグ
		ActionInputFlags Flags = ActionInputFlags::None;
	};
}