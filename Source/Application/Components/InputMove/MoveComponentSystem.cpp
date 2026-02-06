#include "pch.h"
#include "MoveComponentSystem.hpp"
#include "System/Input/Manager/InputManager.hpp"
#include"System/Camera/Camera.hpp"
#include"Math/Matrix3x3/Matrix3x3.h"

#include"Application/Components/InputMove/MoveComponent.hpp"
#include"System/Conponent/Rigidbody/RigidbodyComponent.hpp"
#include"System/Conponent/Transform/TransformConponent.hpp"
#include"Application/Components/Player/PlayerState/PlayerStateComponent.hpp"
#include"Application/Components/Player/Input/InputRequestComponent.hpp"
#include"Application/Components/StageComponent/StateComponent.hpp"
#include"Graphics/DebugRender/DebugRender.hpp"

/// <summary>
/// 移動量の計算処理をしてRigidの移動量に加算する
/// </summary>
void Engine::System::MoveComponentSystem::MainUpdate(entt::registry& Reg, double DeltaTime)
{
	//	ステージの情報を取得
	Math::Vector3 stageCenter = Math::Vector3::Zero;
	float stageRadius = 200;

	//	ある場合は最初の情報を取得する
	auto  stageView = Reg.view<Transform3D, StageComponent>();
	if (auto stageEntity = stageView.front(); stageEntity != entt::null)
	{
		stageCenter = stageCenter = stageView.get<Transform3D>(stageEntity).Position;
		stageRadius = stageView.get<StageComponent>(stageEntity).PlayableRadius;
	}


	auto view = Reg.view<MoveComponent, Rigidbody3D, Transform3D, PlayerStateComponent>();
	view.each([&](auto entity, MoveComponent& move, Rigidbody3D& rigid, Transform3D& trans, PlayerStateComponent& state) 
		{
			//	ワールド空間の方向ベクトル
			Math::Vector3 worldDir = {};

			//	カメラ基準で動くかつメインカメラが存在するとき
			const bool isCameraRelative = (move.UseCameraBasis == true) && System::Camera::Main != nullptr;
			if (isCameraRelative == true)
			{
				//	カメラの向きを取得
				auto camForward = Camera::Main->GetForward();
				auto camRight = Camera::Main->GetRight();

				//	今回はY軸はいらないので0.0にする。
				camForward.y = 0.0f;
				camRight.y = 0.0f;
				//	正規化
				camForward.Normalize();
				camRight.Normalize();

				worldDir = (camForward * move.InputDir.z) + (camRight * move.InputDir.x);

			}
			else
			{
				worldDir = move.InputDir;
			}

			//	移動速度計算（ダッシュなど）
			float speed = move.MoveSpeed;

			//	最終の移動量
			Math::Vector3 finalVelocity = worldDir * speed;

			/*
			* 移動しない状態なら移動量を消すのもここで行う
			*/
			if (state.State == ePlayerState::Dodge) 
			{
				finalVelocity = move.ForceVelocity * move.DodgeSpeed;
			}
			//	回避状態じゃないとき移動量を消す
			else
			{
				move.ForceVelocity = Math::Vector3::Zero;

				//	旋回
				if (worldDir.SqrLength() > 0.001f)
				{
					float targetAngle = atan2(worldDir.x, worldDir.z);
					Math::Quaternion targetRot = Math::Quaternion::AngleAxis(targetAngle, Math::Vector3(0, 0, 1));
					trans.Rotation = Math::Quaternion::Slerp(
						trans.Rotation,
						targetRot,
						move.TurnSpeed * (float)DeltaTime
					);
				}
			}

			//	次のフレーム位置を予測
			Math::Vector3 currentPos = trans.Position;
			Math::Vector3 nextPos = currentPos + (finalVelocity * DeltaTime);
			//Math::Vector3 nextPos = currentPos + finalVelocity;

			//	中心からの距離を計算
			Math::Vector3 toNext = nextPos - stageCenter;
			toNext.y = 0;	//	水平だけ
			float distSq = toNext.SqrLength();

			if (distSq > stageRadius * stageRadius)
			{
				//	外へ向かうベクトル
				toNext.Normalize();

				//	現在の速度が外向きかどうかチェック
				float dot = Math::Vector3::Dot(finalVelocity, toNext);
				if (dot > 0.0f)
				{
					//	外方向の移動量をカット
					finalVelocity -= toNext * dot;
				}

				//	万が一外に出たら位置を強制修正（スタック防止）
				float currDistSq = (currentPos - stageCenter).SqrLength();
				if (currDistSq > stageRadius * stageRadius)
				{
					Math::Vector3 pushDir = (currentPos - stageCenter);
					pushDir.Normalize();
					trans.Position = stageCenter + pushDir * stageRadius;
				}
			}

			//	移動速度を適応
			rigid.Velocity.x = finalVelocity.x;
			rigid.Velocity.z = finalVelocity.z;

			//	入力からの移動量のリセット
			move.InputDir = Math::Vector3::Zero;
			move.ForceVelocity *= 0.995f;
		});


}

void Engine::System::MoveComponentSystem::Render(entt::registry& Reg)
{
#if _DEBUG


	auto view = Reg.view<Transform3D, StageComponent>();
	view.each([&](Transform3D& trans, StageComponent& stage) 
		{
			Graphics::DebugRender::DrawDebugCircle(trans.Position, stage.PlayableRadius, Graphics::Color::Red());
		});

#endif // _DEBUG
}
