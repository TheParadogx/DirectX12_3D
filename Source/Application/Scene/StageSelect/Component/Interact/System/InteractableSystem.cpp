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


/// <summary>
/// ‘JˆÚ”»’è
/// </summary>
/// <param name="Reg"></param>
/// <param name="DeltaTime"></param>
void Engine::System::InteractableSystem::MainUpdate(entt::registry& Reg, double DeltaTime)
{
	//	“ü—Í”»’è
	if (Input::InputManager::GetInstance()->IsActionPressed("Interact") == false)
	{
		return;
	}

	//	ƒvƒŒƒCƒ„[À•W
	auto playerView = Reg.view<Transform3D, PlayerTag>(entt::exclude<DeadTag>);
	Math::Vector3 playerPos = {};
	playerPos = Reg.get<Transform3D>(playerView.front()).Position;

	auto view = Reg.view<Transform3D,InteractableComponent>();
	view.each([&](Transform3D& trans, InteractableComponent&interract) 
		{
			//	ó‘Ô”»’è
			if (interract.Talkable == false) return;

			//	‹——£”»’è
			Math::Vector3 diff = trans.Position - playerPos;
			float sqrtDistance = Math::Vector3::SqrLength(diff);
			if (sqrtDistance <= interract.InteractRange * interract.InteractRange)
			{
				System::SceneManager::GetInstance()->ChangeScene<Scene::InGame>(interract.Rank);
			}
		});


}

/// <summary>
/// ”»’è—p‚Ì‰~‚Ì•`‰æ
/// </summary>
/// <param name="Reg"></param>
void Engine::System::InteractableSystem::Render(entt::registry& Reg)
{
#ifdef _DEBUG


	auto view = Reg.view<Transform3D, InteractableComponent>();
	view.each([&](Transform3D& trans, InteractableComponent& interract)
		{
			Graphics::DebugRender::DrawDebugCircle(
				trans.Position, interract.InteractRange, Graphics::Color::Yellow());
		});
#endif // _DEBUG
}
