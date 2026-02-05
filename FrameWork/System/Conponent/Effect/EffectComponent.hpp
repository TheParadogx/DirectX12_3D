#pragma once
#include"Graphics/Effect/EffectObject.hpp"
#include"entt/entt.hpp"
#include"Math/Vector3/Vector3.h"

namespace Engine::System
{
	struct EffectComponent
	{
		//	親からの相対
		Math::Vector3 Offset = { 0.0f, 0.0f, 0.0f };

		//	エフェクト
		Graphics::EffectObject Effect;

		//	親（追従用）
		entt::entity Parent = entt::null;

		//	アセット名等を保持しているとデバックが楽になる。
		// リリース時にはコメント化
		//std::string EffectName;
	};
}