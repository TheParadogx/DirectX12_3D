#pragma once
#include"entt/entt.hpp"
#include"Effekseer.h"

#include<vector>
#include<array>

namespace Engine::System
{
	/// <summary>
	/// スキルコンポーネント
	/// インスタンスは破棄せずにタグでアクティブかどうかを切り替えます。
	/// </summary>
	struct SkillComponent
	{
		//	有効な時間
		float CooldownTimer = 0.0f;
		//	クールタイム
		float MaxCooldown = 5.0f;
		//	エフェクト
		std::vector<Effekseer::EffectRef> Effects;
	};

	/// <summary>
	/// アクティブなスキルのタグ
	/// </summary>
	struct ActiveSkillTag {
		float ElapsedTime = 0.0f; // 発動してから何秒経ったか
		float Duration = 2.0f;    // 何秒間アクティブでいるか
	};

	/// <summary>
	/// プレイヤーにアタッチするスキルスロットコンポーネント
	/// </summary>
	struct SkillSlotComponent
	{
		//	保持しているスキル
		std::array<entt::entity, 2> SkillSlots = { entt::null };
	};
}