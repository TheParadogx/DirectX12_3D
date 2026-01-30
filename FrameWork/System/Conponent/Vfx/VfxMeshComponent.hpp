#pragma once
#include"Graphics/VFX/VfxSprite.hpp"

#include<memory>
#include<string>
namespace Engine::System
{

	struct VfxMeshComponent
	{
		//	Vfx本体
		std::unique_ptr<Graphics::VfxMesh> Mesh;

		//	今表示中かどうか
		bool IsShow = true;

		//	明示的に
		explicit VfxMeshComponent(Graphics::Texture* Texture)
		{
			Mesh = std::unique_ptr<Graphics::VfxMesh>();

			if (Texture != nullptr)
			{
				Mesh->Create(Texture);
			}
		}
		~VfxMeshComponent() = default;

		// unique_ptr を持っているならコピーは禁止すべき
		VfxMeshComponent(const VfxMeshComponent&) = delete;
		VfxMeshComponent& operator=(const VfxMeshComponent&) = delete;

		// ムーブは許可する
		VfxMeshComponent(VfxMeshComponent&&) noexcept = default;
		VfxMeshComponent& operator=(VfxMeshComponent&&) noexcept = default;
	};

}