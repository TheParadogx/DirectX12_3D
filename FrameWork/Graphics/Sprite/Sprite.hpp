#pragma once
#include"Math/Vector2/Vector2.h"
#include"Utility/MacroHelpers.hpp"

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

	protected:
		/// <summary>
		/// 頂点座標変換
		/// </summary>
		virtual void UpdateVertices(Graphics::SpriteVertex* Vertices);

	protected:
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

	};
}


