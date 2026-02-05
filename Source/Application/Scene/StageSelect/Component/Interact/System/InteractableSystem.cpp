#include "pch.h"
#include "InteractableSystem.hpp"
#include "System/Input/Manager/InputManager.hpp"

#include "System/Conponent/Transform/TransformConponent.hpp"
#include  "System/Scene/Manager/SceneManager.hpp"
#include "Graphics/DebugRender/DebugRender.hpp"
#include "Math/Vector3/Vector3.h"

#include "Application/Components/Tag/TagComponent.hpp"
#include "Application/Scene/StageSelect/Component/Interact/InteractableRange.hpp"
#include "Application/Scene/InGame/InGameScene.hpp"

#include "System/Conponent/Vfx/VfxMeshComponent.hpp"

/// <summary>
/// 遷移判定
/// </summary>
/// <param name="Reg"></param>
/// <param name="DeltaTime"></param>
void Engine::System::InteractableSystem::MainUpdate(entt::registry& Reg, double DeltaTime)
{

	//	プレイヤー座標
	auto playerView = Reg.view<Transform3D, PlayerTag>(entt::exclude<DeadTag>);
	Math::Vector3 playerPos = {};
	playerPos = Reg.get<Transform3D>(playerView.front()).Position;

	auto view = Reg.view<Transform3D,InteractableComponent>();
	view.each([&](auto entity,Transform3D& trans, InteractableComponent&interract) 
		{

			// 範囲内なら表示フラグをON
			if (VfxMeshComponent* vfx = Reg.try_get<VfxMeshComponent>(entity)) {
				vfx->IsShow = true;
			}

			//	距離判定
			Math::Vector3 diff = trans.Position - playerPos;
			float sqrtDistance = Math::Vector3::SqrLength(diff);
			if (sqrtDistance <= interract.InteractRange * interract.InteractRange)
			{
				bool IsInput = Input::InputManager::GetInstance()->IsActionPressed("Interact");
				if (IsInput == true && interract.Talkable == true)
				{
					System::SceneManager::GetInstance()->ChangeSceneFade<Scene::InGame>(interract.Rank);
				}

			}
			else
			{
				//	文字コンポーネントがあるときは削除する
				if (VfxMeshComponent* vfx = Reg.try_get<VfxMeshComponent>(entity)) {
					vfx->IsShow = false;
				}

			}
		});


}

/// <summary>
/// 判定用の円の描画
/// </summary>
/// <param name="Reg"></param>
void Engine::System::InteractableSystem::Render(entt::registry& Reg)
{
#ifdef _DEBUG


	auto view = Reg.view<Transform3D, InteractableComponent>();
	view.each([&](Transform3D& trans, InteractableComponent& interract)
		{
			if (interract.Talkable == false) return;
			
			Graphics::DebugRender::DrawDebugCircle(
				trans.Position, interract.InteractRange, Graphics::Color::Yellow());
		});
#endif // _DEBUG
}
