#pragma once
#include"Graphics/Effect/EffectObject.hpp"
#include"entt/entt.hpp"
#include"Math/Vector3/Vector3.h"

namespace Engine::System
{
	/// <summary>
	/// エフェクトの再生インスタンスを管理するコンポーネント
	/// </summary>
	struct EffectComponent
	{
		//	親からの相対
		Math::Vector3 Offset = { 0.0f, 0.0f, 0.0f };

		//	エフェクト
		Graphics::EffectObject Effect;

		Effekseer::EffectRef Asset;

		//	親（追従用）
		entt::entity Parent = entt::null;

		//	ループするかどうか
		bool IsLoop = false;

		//	表示するかどうか
		bool IsShow = true;
		bool LoastIsShow = false;
	};
}