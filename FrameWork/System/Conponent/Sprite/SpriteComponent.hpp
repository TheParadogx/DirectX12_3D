#pragma once
#include"Graphics/Sprite/Sprite.hpp"
#include"Graphics/Texture/Texture.hpp"

#include<memory>
#include<string>

namespace Engine::System
{
	struct SpriteComponent
	{
		//	画像
		Graphics::Sprite Sprite;

		//	画像を表示するかどうか
		bool IsShow = true;

		//	明示的に
		explicit SpriteComponent(Graphics::Texture* Texture)
		{
			if (Texture != nullptr)
			{
				Sprite.Create(Texture);
			}
		}
		~SpriteComponent() = default;

		// unique_ptr を持っているならコピーは禁止すべき
		SpriteComponent(const SpriteComponent&) = delete;
		SpriteComponent& operator=(const SpriteComponent&) = delete;

		// ムーブは許可する
		SpriteComponent(SpriteComponent&&) noexcept = default;
		SpriteComponent& operator=(SpriteComponent&&) noexcept = default;

	};
}