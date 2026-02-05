#pragma once
#include"Graphics/VFX/VfxSprite.hpp"
#include"Math/Vector3/Vector3.h"
#include<memory>
#include<string>
namespace Engine::System
{

	struct VfxMeshComponent
	{
		//	Vfx本体
		std::unique_ptr<Graphics::VfxMesh> Mesh;

		Math::Vector3 Offset;

		//	表示するかどうか
		bool IsShow = true;

		//	明示的に
		explicit VfxMeshComponent(Graphics::Texture* Texture)
		{
			Mesh = std::make_unique<Graphics::VfxMesh>();

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