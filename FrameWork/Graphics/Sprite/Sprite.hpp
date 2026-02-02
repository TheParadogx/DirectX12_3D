#pragma once
#include"Math/Vector2/Vector2.h"
#include"Utility/MacroHelpers.hpp"
#include"Math/Matrix/Matrix.h"
#include"Graphics/Color/Color.hpp"
#include"Graphics/ConstantBuffer/ConstantBuffer.hpp"

#include<memory>

namespace Engine::Graphics
{
	class Texture;
	struct SpriteVertex;

	class Sprite
	{
	public:
		Sprite();
		virtual ~Sprite() = default;

		/// <summary>
		/// 生成
		/// </summary>
		/// <param name="Texture">テクスチャのポインタ</param>
		/// <returns>true:成功</returns>
		bool Create(Graphics::Texture* Texture);

		/// <summary>
		/// テクスチャ描画
		/// </summary>
		virtual void Render();

		//	アクセサ
		DEFINE_PROPERTY(Math::Vector2, Position, mPosition);
		DEFINE_PROPERTY(Math::Vector2, Pivot, mPivot);
		DEFINE_PROPERTY(Math::Vector2, Scale, mScale);
		DEFINE_PROPERTY(Math::Vector2, Size, mSize);
		DEFINE_PROPERTY(float, Angle, mAngle);
		DEFINE_PROPERTY(Graphics::Color, Color, mColor);
	protected:
		/// <summary>
		/// 頂点座標変換
		/// </summary>
		virtual void UpdateVertices(Graphics::SpriteVertex* Vertices);

	protected:
		struct SpriteConstantBuffer {
			Math::Matrix WVP;      // 座標変換用
			Graphics::Color Color;   // RGBA (Aが透明度)
			float Intensity = 1.0f;		//　光度
			float Padding[3];
		};



		/// <summary>
		/// テクスチャ
		/// </summary>
		Texture* mTexture;

		/// <summary>
		/// 座標
		/// </summary>
		Math::Vector2 mPosition;

		/// <summary>
		/// 中心となる軸
		/// </summary>
		Math::Vector2 mPivot;

		/// <summary>
		/// 拡縮
		/// </summary>
		Math::Vector2 mScale;

		/// <summary>
		/// サイズ(幅,高さ)
		/// </summary>
		Math::Vector2 mSize;

		/// <summary>
		/// 回転の角度
		/// </summary>
		float mAngle;

		/// <summary>
		/// 色
		/// </summary>
		Graphics::Color mColor;

		/// <summary>
		/// 定数バッファ
		/// </summary>
		std::unique_ptr<Graphics::ConstantBuffer> mConstantBuffer;
	};
}


