#include "pch.h"
#include "FbxRenderSystem.h"

#include"System/Conponent/Fbx/FbxMeshConponent.hpp"
#include"System/Conponent/Transform/TransformConponent.hpp"
#include"System/Conponent/Rigidbody/RigidbodyComponent.hpp"

namespace Engine::System
{

	/// <summary>
	/// アニメーションの状態更新
	/// </summary>
	/// <param name="Reg"></param>
	/// <param name="DeltaTime"></param>
	void FbxRenderSystem::Update(entt::registry& Reg, double DeltaTime)
	{
		//	Fbxと座標系を持っているオブジェクトの取得
		auto view = Reg.view<FbxComponent>();

		view.each([&](auto entity, auto& fbx)
		{
				float speed = fbx.AnimationScale * static_cast<float>(DeltaTime);
				fbx.Mesh->Animate(fbx.CurrAnimation, speed, fbx.IsLoop);
		});

	}

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="Reg">enttレジストリ</param>
	/// <param name="Alpha">補間係数</param>
	void FbxRenderSystem::Render(entt::registry& Reg)
	{
		//	Fbxと座標系を持っているオブジェクトの取得
		auto view = Reg.view<FbxComponent,Transform3D>();

		view.each([&](auto entity, auto& fbx, auto& trans)
		{
			fbx.Mesh->SetPosition(trans.Position);
			fbx.Mesh->SetScale(trans.Scale);
			fbx.Mesh->SetRotation(trans.Rotation * fbx.OffsetRotation);
			fbx.Mesh->Render();
		});
	}
}