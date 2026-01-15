#pragma once
#include<Plugin/entt/entt.hpp>
#include<string>

#include"Math/Vector3/Vector3.h"
#include"Math/Quaternion/Quaternion.h"

namespace Engine::System
{
	struct SocketComponent
	{
		std::string BoneName;		//	アタッチするボーン名
		Math::Quaternion OffsetRot = Math::Quaternion::Identity;;	//	向きの調整用
		Math::Vector3 OffsetPos;	//	座標の調整用
		Math::Vector3 PivotOffset;	//	このオブジェクトの基準点調節用
		entt::entity Parent = entt::null;		//	座標基準にする親
	};
}