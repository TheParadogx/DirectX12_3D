#include "pch.h"
#include "EffectSystem.hpp"
#include "System/Conponent/Transform/TransformConponent.hpp"
#include "../EffectComponent.hpp"

namespace Engine::System
{
	/// <summary>
	/// トランスとの座標、回転同期
	/// </summary>
	void EffectSystem::PostUpdate(entt::registry& Reg)
	{
		auto view = Reg.view<EffectComponent>();
		view.each([&](auto entity, EffectComponent& effect) 
			{
				//	自動削除判定
				if (effect.Effect.IsPlaying() == false)
				{
					//	ループ設定かつアセットが有効なら再生
					if (effect.IsLoop == true && effect.Asset != nullptr)
					{
						effect.Effect.Play(effect.Asset, { 0,0,0 }, effect.Effect.ShouldDestroy());
					}
					else
					{
						// ループしないで終了したなら、自動削除設定を確認して削除
						if (effect.Effect.ShouldDestroy())
						{
							Reg.destroy(entity);
						}
						return;
					}
				}

				//	座標の同期
				// 同期すべきトランスフォームの取得
				Transform3D* targetTrans = nullptr;

				// 親がいれば親を優先、いなければ自分自身のTransform3Dを使う
				if (effect.Parent != entt::null && Reg.valid(effect.Parent)) {
					targetTrans = Reg.try_get<Transform3D>(effect.Parent);
				}
				else {
					targetTrans = Reg.try_get<Transform3D>(entity);
				}

				if (targetTrans)
				{
					// ^位置の同期^
					// 親がいる場合はOffsetを加算（親の回転を考慮しない簡易版）
					Math::Vector3 finalPos = targetTrans->Position + effect.Offset;
					effect.Effect.SetLocation(finalPos);

					// ^回転の同期^
					// Effekseer::SetRotation はオイラー角 (ラジアン) を期待する
					// Quaternion から Euler への変換 (エンジンの仕様に合わせてください)
					Math::Vector3 euler = targetTrans->Rotation.ToEuler(); // 仮のメソッド名
					effect.Effect.SetRotation(euler);

					// ^スケールの同期^
					effect.Effect.SetScale(targetTrans->Scale);
				}

			});

	}
}