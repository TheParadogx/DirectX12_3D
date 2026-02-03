#pragma once
namespace Engine::System
{
	/// <summary>
	/// どのエフェクトでも共通のコンポーネント
	/// </summary>
	struct GlowComponent
	{
		//	基準値
		float BaseIntensity = 1.0f;
		//	時間経過計測
		float Timer = 0.0f;
	};

	/// <summary>
	/// 鼓動：指定した時間で指定した範囲を揺らす
	/// </summary>
	struct PulseTag {
		float Duration = 2.0f; // 周期（秒）
		float Range = 0.5f;    // +-振幅
	};

	/// <summary>
	/// 花火のようにピカッと：指定した確率で瞬間的に発光する
	/// </summary>
	struct SparkTag {
		float Chance = 0.02f;      // 発生確率 (0.0～1.0)
		float BoostMultiplier = 2.0f; // 光った時の倍率
	};

	/// <summary>
	/// 直線変化：指定時間でAからBに変化させる
	/// </summary>
	struct LinearShiftTag {
		float StartIntensity = 0.0f;
		float EndIntensity = 1.0f;
		float Duration = 1.0f;     // 変化にかける時間
		bool Loop = false;         // ループするかどうか
	};

}