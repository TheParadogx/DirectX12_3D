#pragma once

namespace Engine::System
{
    /// <summary>
    /// スケール操作の基準となるコンポーネント
    /// </summary>
    struct ScaleEffectComponent
    {
        // 基準となるスケール（通常は1.0f）
        float BaseScale = 1.0f;
        // 現在計算された最終的な表示倍率
        float CurrentScale = 1.0f;
        // 時間経過計測
        float Timer = 0.0f;

        //  true:有効 false:無効
        bool IsActive = true;
    };

    /// <summary>
    /// スケールの鼓動：指定した周期で大きさを揺らす
    /// </summary>
    struct ScalePulseTag {
        float Duration = 1.5f; // 1周期の時間
        float Range = 0.1f;   // 振幅（1.0 +- 0.1 のように動く）
    };

    /// <summary>
    /// スケールの直線変化：出現時や消滅時のズーム演出などに
    /// </summary>
    struct ScaleLinearTag {
        float StartScale = 0.0f;
        float EndScale = 1.0f;
        float Duration = 0.5f;
        bool Loop = false;
        bool PingPong = false; // 往復させるなら便利
    };

    /// <summary>
    /// 選択中のみスケールを適用するための識別用
    /// </summary>
    struct SelectedUIEffectTag {};
}