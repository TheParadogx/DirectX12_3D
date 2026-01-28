#include "pch.h"
#include "CameraControlSystem.hpp"

#include"Application/Components/CameraWork/CameraControl.hpp"
#include"System/Input/Manager/InputManager.hpp"
#include"System/Conponent/Transform/TransformConponent.hpp"

#include"System/Camera/Camera.hpp"
#include"Math/Math.h"


void Engine::System::CameraControlSystem::PostUpdate(entt::registry& Reg, double DeltaTime)
{
	//	コンポーネントを持つオブジェクトの取得
//	入力の取得
	auto view = Reg.view<CameraControlComponent>();
	auto input = Input::InputManager::GetInstance();

	view.each([&](auto entity, auto& ctrl)
		{
			//	ターゲットその存在確認
			if (ctrl.TargetEntity == entt::null || Reg.valid(ctrl.TargetEntity) == false)
			{
				return;
			}

			//	入力の取得
			Math::Vector2 look = input->GetLookAxis();

			//	角度の更新
			ctrl.Yaw += look.x * ctrl.LookSensitivity * DeltaTime;
			ctrl.Pitch += look.y * ctrl.LookSensitivity * DeltaTime;

			ctrl.Yaw = fmodf(ctrl.Yaw, Math::PI * 2.0f);

			//	回転の制限
			ctrl.Pitch = std::clamp(ctrl.Pitch, ctrl.PitchLimitMin, ctrl.PitchLimitMax);

			//	注視点の計算
			auto& TargetTransform = Reg.get<Transform3D>(ctrl.TargetEntity);
			Math::Vector3 LookAtPos = TargetTransform.Position + ctrl.offset;

			//	球面座標からカメラの相対座標を算出
			// 公式: （間違っていたらどんまい）
			// x = r * cos(pitch) * sin(yaw)
			// y = r * sin(pitch)
			// z = r * cos(pitch) * cos(yaw)
			Math::Vector3 relativePos;
			float cosPitch = cosf(ctrl.Pitch);

			relativePos.x = ctrl.Distance * cosPitch * sinf(ctrl.Yaw);
			relativePos.y = -ctrl.Distance * sinf(ctrl.Pitch);
			relativePos.z = ctrl.Distance * cosPitch * cosf(ctrl.Yaw);

			//	カメラの最終的な座標
			Math::Vector3 cameraPos = LookAtPos + relativePos;

			//	カメラ本体に反映
			if (Camera::Main != nullptr)
			{
				Camera::Main->Update(cameraPos, LookAtPos, Math::Vector3::Up);
			}

		});
}
