#pragma once
#include"Graphics/UI/UISprite.hpp"
#include"Graphics/Sprite/Sprite.hpp"

#include"Math/Vector2/Vector2.h"

#include<memory>

namespace Engine::System
{

	/// <summary>
	/// ステータスコンポーネントがないと何も表示されない
	/// </summary>
	struct HpRenderComponent
	{
		std::unique_ptr<Graphics::Sprite> BaseSprite;	//	下地
		std::unique_ptr<Graphics::UISprite> BarSprite;	//	バー本体

		//	この情報１つで両方のサイズと座標を変えれるように。
		Math::Vector2 Position;
		Math::Vector2 Size;

		//	true:外部で色の制御をする　false:体力の残量に応じて緑から赤の自動的に色が変わる
		bool isColorOverridden = false;

		HpRenderComponent(Graphics::Texture* BaseTexutre, Graphics::Texture* BarTexutre,const Math::Vector2& Pivot)
		{
			BaseSprite = std::make_unique<Graphics::Sprite>();
			BaseSprite->Create(BaseTexutre);
			BarSprite = std::make_unique<Graphics::UISprite>();
			BarSprite->Create(BarTexutre);

			BaseSprite->SetPivot(Pivot);
			BarSprite->SetPivot(Pivot);
			BarSprite->SetColor(Graphics::Color::Green());
		}
		// unique_ptr を持っているならコピーは禁止すべき
		HpRenderComponent(const HpRenderComponent&) = delete;
		HpRenderComponent& operator=(const HpRenderComponent&) = delete;

		// ムーブは許可する
		HpRenderComponent(HpRenderComponent&&) noexcept = default;
		HpRenderComponent& operator=(HpRenderComponent&&) noexcept = default;

		void SetPosition(const Math::Vector2& Position )
		{
			if (BaseSprite != nullptr)
			{
				BaseSprite->SetPosition(Position);
			}
			if (BarSprite != nullptr)
			{
				BarSprite->SetPosition(Position);
			}
		}

		void SetSize(const Math::Vector2& Size)
		{
			if (BaseSprite != nullptr)
			{
				BaseSprite->SetSize(Size);
			}
			if (BarSprite != nullptr)
			{
				BarSprite->SetSize(Size);
			}
		}


	};
}