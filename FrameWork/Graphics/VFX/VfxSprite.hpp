#pragma once

#include"Math/Vector2/Vector2.h"
#include"Math/Vector3/Vector3.h"
#include"Graphics/Color/Color.hpp"
#include"Math/Matrix/Matrix.h"
#include<vector>
#include<array>

#include"Graphics/Graphics.hpp"


namespace Engine::Graphics
{
	/// <summary>
	/// 方向などの設定
	/// </summary>
	enum class eVfxBillboardType {
		None,     // 空間に固定（地面の魔法陣など）
		AllAxis,  // 常にカメラを向く（爆発、火花など）
		YAxis,    // Y軸だけカメラを向く（木のモデルなど）
	};

	// 定数バッファ：HLSLの cbuffer VfxBuffer : register(b0) と一致させる
	// 16バイト境界を守るため alignas を指定
	struct alignas(16) VfxConstantBuffer
	{
		Math::Matrix matWVP;    // 行列
		Math::Vector2 uvOffset; // UVスクロール
		float intensity;        // 輝度
		float padding;          // 境界調整用
	};

	class Texture;
	class VfxSprite
	{
	public:
		VfxSprite();
		virtual ~VfxSprite() = default;

		bool Create(Texture* texture);
		void Update(float deltaTime);
		void Render();

		// アクセサ
		void SetPosition(const Math::Vector3& pos) { mPosition = pos; }
		void SetScale(const Math::Vector3& scale) { mScale = scale; }
		void SetAngle(const Math::Vector3& angle) { mAngle = angle; }
		void SetColor(const Color& color) { mBaseColor = color; }
		void SetIntensity(float intensity) { mIntensity = intensity; }
		void SetScrollSpeed(const Math::Vector2& speed) { mScrollSpeed = speed; }
		void SetBillboardType(eVfxBillboardType type) { mBillboardType = type; }
	private:
		Texture* mTexture;

		Math::Vector3 mPosition;
		Math::Vector3 mScale;
		Math::Vector3 mAngle; // (Pitch, Yaw, Roll)

		Color         mBaseColor;
		Math::Vector2 mUVOffset;
		Math::Vector2 mScrollSpeed;
		float         mIntensity;

		eVfxBillboardType mBillboardType = eVfxBillboardType::None;

		std::array<VfxVertex,4> mVertices;
	};
}


