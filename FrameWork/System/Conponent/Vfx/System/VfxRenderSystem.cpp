#include "pch.h"
#include "VfxRenderSystem.hpp"

#include "../VfxMeshComponent.hpp"
#include "System/Conponent/Transform/TransformConponent.hpp"

/// <summary>
/// アニメーションの状態更新
/// </summary>
/// <param name="Reg"></param>
/// <param name="DeltaTime"></param>
void Engine::System::VfxRenderSystem::Update(entt::registry& Reg)
{
	auto view = Reg.view<VfxMeshComponent, Transform3D>();
	view.each([&](VfxMeshComponent& fbx, Transform3D& trans)
		{
			if (fbx.IsShow == true) return;

			fbx.Mesh->SetPosition(trans.Position);
			fbx.Mesh->SetRotation(trans.Rotation);
			fbx.Mesh->SetScale(trans.Scale);
		});

}

/// <summary>
/// 描画
/// </summary>
/// <param name="Reg">enttレジストリ</param>
/// <param name="Alpha">補間係数</param>
void Engine::System::VfxRenderSystem::Render(entt::registry& Reg)
{
	auto view = Reg.view<VfxMeshComponent>();
	view.each([&](VfxMeshComponent& vfx)
		{
			vfx.Mesh->Render();
		});
}
