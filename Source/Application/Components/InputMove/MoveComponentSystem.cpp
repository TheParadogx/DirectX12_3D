#include "pch.h"
#include "MoveComponentSystem.hpp"
#include "System/Input/Manager/InputManager.hpp"
#include"System/Camera/Camera.hpp"

#include"Application/Components/InputMove/MoveComponent.hpp"
#include"System/Conponent/Rigidbody/RigidbodyComponent.hpp"
#include"System/Conponent/Transform/TransformConponent.hpp"
#include"Application/Components/Player/PlayerState/PlayerStateComponent.hpp"
#include"Application/Components/Player/Input/InputRequestComponent.hpp"

/// <summary>
/// 移動量の計算処理をしてRigidの移動量に加算する
/// </summary>
void Engine::System::MoveComponentSystem::MainUpdate(entt::registry& Reg, double DeltaTime)
{

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

				worldDir = (camForward * move.TargetDir.z) + (camRight * move.TargetDir.x);

			}
			else
			{
				worldDir = move.TargetDir;
			}

			//	移動速度計算（ダッシュなど）
			float speed = move.MoveSpeed;
			//	ダッシュ
			if (state.State == ePlayerState::Dash)
			{
				speed *= move.SprintMulti;
			}

			/*
			* 移動しない状態なら移動量を消すのもここで行う
			*/

			//	移動速度を適応
			rigid.Velocity.x = worldDir.x * speed;
			rigid.Velocity.z = worldDir.z * speed;

			/*
			* ジャンプ処理（必要になったらここに実装）
			*/

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

			//	入力からの移動量のリセット
			move.TargetDir = Math::Vector3::Zero;

		});


}
