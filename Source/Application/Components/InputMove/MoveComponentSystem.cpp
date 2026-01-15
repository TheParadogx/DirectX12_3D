#include "pch.h"
#include "MoveComponentSystem.hpp"
#include "System/Input/Manager/InputManager.hpp"
#include"System/Camera/Camera.hpp"

#include"Application/Components/InputMove/MoveComponent.hpp"
#include"System/Conponent/Rigidbody/RigidbodyComponent.hpp"
#include"System/Conponent/Transform/TransformConponent.hpp"

void Engine::System::MoveComponentSystem::PreUpdate(entt::registry& Reg, double DeltaTime)
{
	auto view = Reg.view<MoveComponent, Rigidbody3D,Transform3D>();
	view.each([&](auto entity, auto& Move, auto& Rigid,auto& trans)
		{
			//	移動可能フラグの判定
			if ((Move.Flags & MoveFlags::CanMove) == MoveFlags::None)
			{
				return;
			}
			//	入力可能フラグ
			if ((Move.Flags & MoveFlags::InputDisabled) != MoveFlags::None)
			{
				return;
			}


			//	ワールド空間での方向ベクトル
			Math::Vector3 worldDir = {};

			//	カメラ基準で動くかつメインカメラが存在するとき
			if ((Rigid.Flags & RigidbodyFlags::UseCameraBasis) != RigidbodyFlags::None
				&& Camera::Main != nullptr)
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

				////	カメラ基準のベクトル作成
				worldDir = (camForward * Move.InputVec.z) + (camRight * Move.InputVec.x);
			}
			else
			{
				worldDir = Move.InputVec;
			}

			//	移動速度計算（ダッシュ等）
			float speed = Move.MoveSpeed;
			if ((Move.Flags & MoveFlags::IsRunning) != MoveFlags::None)
			{
				speed *= Move.SprintMulti;
			}

			//	移動速度を適応
			Rigid.Velocity.x = worldDir.x * speed;
			Rigid.Velocity.z = worldDir.z * speed;

			//	ジャンプ処理（連続ジャンプ可能なら着地判定入れなくていい）
			bool isGrounded = (Rigid.Flags & RigidbodyFlags::IsGrounded) != RigidbodyFlags::None;
			if ((Move.Flags & MoveFlags::IsJumping) != MoveFlags::None
				&& isGrounded == true)
			{
				//	連続ジャンプしないかつ地面に接しているときだけなので
				Rigid.Velocity.y = Move.JumpPower;

				//	一応のためのフラグ折りをいれてもいい
			}

			//	旋回
			if (worldDir.SqrLength() > 0.001f)
			{
				float targetAngle = atan2(worldDir.x, worldDir.z);
				Math::Quaternion targetRot = Math::Quaternion::AngleAxis(targetAngle, Math::Vector3(0, 0, 1));
				trans.Rotation = Math::Quaternion::Slerp(
					trans.Rotation,
					targetRot,
					Move.TurnSpeed * (float)DeltaTime
				);
			}
		});

}

void Engine::System::MoveComponentSystem::MainUpdate(entt::registry& Reg, double DeltaTime)
{

	//	移動量を取得
	auto vel = GET_INPUT_MANAGER->GetAxis();

	//	一旦直接キーから取得（将来的にはマッピング）
	auto isJump = GET_INPUT_MANAGER->GetKeyboard()->IsKeyPressed(Input::eKeyCode::Space);
	auto isSprint = GET_INPUT_MANAGER->GetKeyboard()->IsKeyPressed(Input::eKeyCode::LShift);

	//	コンポーネントの取得
	auto view = Reg.view<MoveComponent>();

	for(auto entity : view)
	{
		auto& move = view.get<MoveComponent>(entity);

		//	入力値をセット
		move.InputVec = { vel.x,0.0f,vel.y};

		//	ジャンプ可能かどうか
		if ((move.Flags & MoveFlags::CanJump) == MoveFlags::None)
		{
			isJump = false;
		}
		//	ジャンプ
		if (isJump == true)
		{
			move.Flags |= MoveFlags::IsJumping;
		}
		else
		{
			move.Flags &= ~MoveFlags::IsJumping;
		}

		//	ダッシュできる状態かどうか
		if ((move.Flags & MoveFlags::CanRunning) == MoveFlags::None)
		{
			isSprint = false;
		}
		//	ダッシュ
		if (isSprint == true)
		{
			move.Flags |= MoveFlags::IsRunning;
		}
		else
		{
			move.Flags &= ~MoveFlags::IsRunning;
		}

	}

}
