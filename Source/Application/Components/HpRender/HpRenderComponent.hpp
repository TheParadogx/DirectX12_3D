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
		std::unique_ptr<Graphics::UISprite> BarSpriteYellow;	//	遅れて減る体力バー

		//	この情報１つで両方のサイズと座標を変えれるように。
		Math::Vector2 Position;
		Math::Vector2 Size;

		//	黄色のバー
		float VisualRatio = 1.0f;

		//	減少スピード
		float ShrinkSpeed = 1.5f;

		//	true:外部で色の制御をする　false:体力の残量に応じて緑から赤の自動的に色が変わる
		bool isColorOverridden = false;

		//HpRenderComponent(Graphics::Texture* BaseTexutre, Graphics::Texture* BarTexutre, Graphics::Texture* FrameTexture,const Math::Vector2& Pivot)
		HpRenderComponent(Graphics::Texture* BaseTexutre, Graphics::Texture* BarTexutre, const Math::Vector2& Pivot,const Graphics::Color& Color = Graphics::Color::Green())
		{
			//	下地
			BaseSprite = std::make_unique<Graphics::Sprite>();
			BaseSprite->Create(BaseTexutre);
			//	本体
			BarSprite = std::make_unique<Graphics::UISprite>();
			BarSprite->Create(BarTexutre);
			//	フレーム
			BarSpriteYellow = std::make_unique<Graphics::UISprite>();
			BarSpriteYellow->Create(BarTexutre);
			BarSpriteYellow->SetColor(Graphics::Color::Apricot());


			BaseSprite->SetPivot(Pivot);
			BarSprite->SetPivot(Pivot);
			BarSpriteYellow->SetPivot(Pivot);
			BarSprite->SetColor(Color);
		}
		// unique_ptr を持っているならコピーは禁止すべき
		HpRenderComponent(const HpRenderComponent&) = delete;
		HpRenderComponent& operator=(const HpRenderComponent&) = delete;

		// ムーブは許可する
		HpRenderComponent(HpRenderComponent&&) noexcept = default;
		HpRenderComponent& operator=(HpRenderComponent&&) noexcept = default;

		void SetPosition(const Math::Vector2& Position )
		{
			this->Position = Position;

			if (BaseSprite != nullptr)
			{
				BaseSprite->SetPosition(Position);
			}
			if (BarSprite != nullptr)
			{
				BarSprite->SetPosition(Position);
			}
			if (BarSpriteYellow != nullptr)
			{
				BarSpriteYellow->SetPosition(Position);
			}
		}

		void SetSize(const Math::Vector2& Size)
		{
			this->Size = Size;
			if (BaseSprite != nullptr)
			{
				BaseSprite->SetSize(Size);
			}
			if (BarSprite != nullptr)
			{
				BarSprite->SetSize(Size);
			}
			if (BarSpriteYellow != nullptr)
			{
				BarSpriteYellow->SetSize(Size);
			}
		}


	};
}