#pragma once

#include"Graphics/Sprite/Sprite.hpp"
#include"Graphics/Color/Color.hpp"

namespace Engine::Graphics
{
	/// <summary>
	///	画像表示の機能に加えて表示範囲の
	/// </summary>
	class UISprite : public Sprite
	{

		void UpdateVertices(Graphics::SpriteVertex* Vertices)override;
	public:
		UISprite()
			:mFillRatio(1.0f), mColor(Color::White())
		{ }

		/// <summary>
		/// 表示
		/// </summary>
		void Render()override;


		DEFINE_PROPERTY(float, FillRatio, mFillRatio);
		DEFINE_PROPERTY(Color, Color, mColor);
	private:
		/// <summary>
		/// 色
		/// </summary>
		Color mColor = Color::White();

		/// <summary>
		/// 左から表示割合（0.0 ～ 1.0）
		/// </summary>
		float mFillRatio;
	};
}


