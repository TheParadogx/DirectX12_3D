#pragma once
#include"Effekseer.h"
#include"Math/Vector3/Vector3.h"

#include<vector>

namespace Engine::System
{
	/// <summary>
	/// 最終的なダメージのコンポーネント
	/// </summary>
	struct AttackPowerComponent
	{
		float DamageValue = 10;
		std::vector<Effekseer::EffectRef> HitEffectAsset;	//	攻撃が当たった時のエフェクト
		Math::Vector3 Offset = {};	//	オフセット
		Math::Vector3 Scale = {1.0f,1.0f,1.0f};
	};
}