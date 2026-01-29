#pragma once

#include"Graphics/DirectX/TypeAlias/DirectX12TypeAlias.hpp"

#include"Math/Vector2/Vector2.h"
#include"Math/Vector3/Vector3.h"
#include"Graphics/Color/Color.hpp"
#include"Math/Matrix/Matrix.h"
#include"Math/Quaternion/Quaternion.h"
#include<vector>
#include<array>
#include<memory>

#include"Graphics/Graphics.hpp"

namespace Engine::Graphics
{
	class Texture;
	class ConstantBuffer;
	class VfxMesh
	{
	public:
		VfxMesh();
		virtual ~VfxMesh() = default;

		/// <summary>
		/// 作成
		/// </summary>
		/// <returns>true:成功</returns>
		bool Create(Texture* texture);

		/// <summary>
		/// 行列の計算やビューボード制御
		/// </summary>
		void Update(float DeltaTime );

		/// <summary>
		/// 描画
		/// </summary>
		void Render();

		//	座業
		const Math::Vector3& GetPosition()const { return mPosition; }
		void SetPosition(const Math::Vector3& position) { mPosition = position; }
		//	回転
		const Math::Quaternion& GetRotation()const { return mRotation; }
		void SetRotation(const Math::Quaternion& rotaion) { mRotation = rotaion; };
		//	スケール
		const Math::Vector3& GetScale()const { return mScale; }
		void SetScale(const Math::Vector3& scale) { mScale = scale; }
		//	サイズ


		//	色
		void SetColor(const Color& color) { mColor = color; }

	private:
		/// <summary>
		/// 板ポリゴンの生成
		/// </summary>
		/// <returns></returns>
		bool CreateMesh();

	protected:
		struct Vertex {
			Math::Vector3 pos;
			Math::Vector2 uv;
		};

		// VFX用定数バッファ構造体（HLSLのb0と一致させる）
		struct VfxConstantBufferInfo {
			Math::Matrix World;
			Math::Matrix View;
			Math::Matrix Projection;
			Color MeshColor;
			float Timer;      // アニメーション用経過時間
			float Padding[3];
		};

	protected:
		/// <summary>
		/// 255のアライメントにする
		/// </summary>
		static constexpr auto ConstantBufferSize = (sizeof(VfxConstantBufferInfo) + 255) & ~255;

	protected:
		/// <summary>
		/// 行列用GPUの行列を送る用のバッファ
		/// </summary>
		std::unique_ptr<ConstantBuffer> mTransformBuffer;

		/// <summary>
		/// 頂点バッファ
		/// </summary>
		mw::ResourcePtr mVertexBuffer;

		/// <summary>
		/// 頂点情報
		/// </summary>
		D3D12_VERTEX_BUFFER_VIEW mVBView = {};

		/// <summary>
		/// テクスチャの参照用ポインタ
		/// </summary>
		Texture* mTexture;

		/// <summary>
		/// ワールド行列
		/// </summary>
		Math::Matrix mWorldMatrix;
		/// <summary>
		/// 座標
		/// </summary>
		Math::Vector3    mPosition;
		/// <summary>
		/// 回転
		/// </summary>
		Math::Quaternion mRotation;
		/// <summary>
		/// スケール
		/// </summary>
		Math::Vector3    mScale;
		/// <summary>
		/// サイズ
		/// </summary>
		Math::Vector2    mSize;
		/// <summary>
		/// 回転
		/// </summary>
		Math::Vector2    mPivot;
		/// <summary>
		/// 色
		/// </summary>
		Color            mColor;

		/// <summary>
		/// Animationの時間管理用
		/// </summary>
		float mTimer;

		/// <summary>
		/// ビューボードの設定
		/// true:カメラの方を向く false:自身基準
		/// </summary>
		bool mIsBillBoard;

	};
}


