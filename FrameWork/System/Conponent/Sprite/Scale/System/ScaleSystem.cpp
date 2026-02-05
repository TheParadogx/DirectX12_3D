#include "pch.h"
#include"ScaleSystem.hpp"
#include"../ScaleComponent.hpp"
#include "System/Conponent/Sprite/SpriteComponent.hpp"
#include"System/Conponent/Transform/TransformConponent.hpp"

void Engine::System::ScaleSystem::Update(entt::registry& Reg, float DeltaTime)
{
    Reg.view<ScaleEffectComponent, SpriteComponent,Transform2D>()
        .each([&](auto entity, ScaleEffectComponent& scale, SpriteComponent& sprite, Transform2D& trans)
            {
                float nextScale = scale.BaseScale;

                if (scale.IsActive == false)
                {
                    // 計算したスケールを適用
                    scale.CurrentScale = nextScale;
                    trans.Scale = { nextScale ,nextScale };
                    return;
                }

                // 直線変化
                if (ScaleLinearTag* linear = Reg.try_get<ScaleLinearTag>(entity))
                {
                    scale.Timer += DeltaTime;
                    float t = std::clamp(scale.Timer / linear->Duration, 0.0f, 1.0f);
                    nextScale = std::lerp(linear->StartScale, linear->EndScale, t);

                    // 完了時の処理
                    if (t >= 1.0f) {
                        if (linear->Loop) {
                            scale.Timer = 0.0f;
                        }
                        else {
                            // 必要ならタグ削除やエンティティ破棄などのロジック
                        }
                    }
                }
                else if (ScalePulseTag* pulse = Reg.try_get<ScalePulseTag>(entity))
                {
                    scale.Timer += DeltaTime;
                    // sin波で 1.0f を中心に ±Range 動かす
                    float wave = std::sin(scale.Timer * (2.0f * 3.14159f / pulse->Duration));
                    nextScale = scale.BaseScale + (wave * pulse->Range);
                }

                // 計算したスケールを適用
                scale.CurrentScale = nextScale;
                trans.Scale = { nextScale ,nextScale };
            });
}