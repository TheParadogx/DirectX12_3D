#pragma once
#include"System/Singleton/Singleton.hpp"
#include"Graphics/VertexBuffer/VertexBuffer.hpp"
#include"Graphics/IndexBuffer/IndexBuffer.hpp"
#include"Graphics/ConstantBuffer/ConstantBuffer.hpp"
#include"Graphics/Graphics.hpp"
#include"Math/Matrix/Matrix.h"

#include"Graphics/Sprite/Pipeline/SpritePipeline.hpp"
#include"Graphics/FbxMesh/Pipeline/FbxPipeline.hpp"
#include"Graphics/Line/Pipeline/LinePipeline.hpp"
#include"Graphics/UI/Pipeline/UISpritePipeline.hpp"
#include"Graphics/SkyBox/Pipeline/SkyBoxPipeline.hpp"
#include"Graphics/VFX/Pipeline/VFXPipeline.hpp"

#include<cstdint>
#include<memory>

namespace Engine::Graphics
{
	/// <summary>
	/// 描画管理
	/// </summary>
	class Renderer : public Singleton<Renderer>
	{
		GENERATE_SINGLETON_BODY(Renderer);

	public:

		/// <summary>
		/// インスタンス生成時の処理
		/// </summary>
		virtual void OnCreate()override;

		/// <summary>
		/// インスタンスの解放時の処理
		/// </summary>
		virtual void OnDestory()override;

		/// <summary>
		/// 新規描画フレームの開始
		/// </summary>
		void NewRenderingFrame();

		/// <summary>
		/// 2D用スプライトの描画
		/// </summary>
		/// <param name="spriteVertices">頂点配列の先頭アドレス</param>
		/// <param name="vertexCount">頂点配列数</param>
		/// <param name="spriteIndices">インデックス配列の先頭アドレス</param>
		/// <param name="indexCount">インデックス配列数</param>
		void DrawSprite(
			const SpriteVertex* spriteVertices, const size_t vertexCount,
			const uint16_t* spriteIndices, const size_t indexCount);

		/// <summary>
		/// FbxMeshの描画
		/// </summary>
		/// <param name="VBv"></param>
		/// <param name="IBv"></param>
		/// <param name="IndexCount"></param>
		/// <param name="SrvHandle"></param>
		void DrawMesh(
			const D3D12_VERTEX_BUFFER_VIEW& VBv, const D3D12_INDEX_BUFFER_VIEW& IBv, 
			UINT IndexCount, UINT StartIndex);

		/// <summary>
		/// SkyBoxの描画
		/// </summary>
		/// <param name="VBv">スカイボックスの頂点バッファビュー</param>
		/// <param name="vertexCount">頂点数（通常36）</param>
		void DrawSkyBox(const D3D12_VERTEX_BUFFER_VIEW& VBv, UINT vertexCount);

		/// <summary>
		/// Vfxの描画
		/// </summary>
		/// <param name="v"></param>
		/// <param name="vNum"></param>
		/// <param name="i"></param>
		/// <param name="iNum"></param>
		void DrawVfx(const VfxVertex* v, size_t vNum, const uint16_t* i, size_t iNum);

		/// <summary>
		/// ワールド空間へのライン描画
		/// </summary>
		/// <param name="lineVertices">頂点配列の先頭アドレス</param>
		/// <param name="vertexCount">頂点配列数</param>
		/// <param name="lineIndices">インデックス配列の先頭アドレス</param>
		/// <param name="indexCount">インデックス配列の先頭アドレス</param>
		void DrawLines(
			const LineVertex* LineVertices,
			const size_t VertexCount,
			const uint16_t* LineIndices,
			const size_t IndexCount);

		/// <summary>
		/// デバック用の点と回転の表示
		/// </summary>
		/// <param name="pos"></param>
		/// <param name="rot"></param>
		/// <param name="length"></param>
		void DrawGizmo(Math::Vector3 pos, Math::Quaternion rot, float length = 0.2f);

		/// <summary>
		/// スプライトのパイプラインのセット
		/// </summary>
		void SetSpritePipeline();

		/// <summary>
		/// Fbxのパイプラインのセット
		/// </summary>
		void SetFbxPipeline();

		/// <summary>
		/// 線描画用のパイプラインのセット
		/// </summary>
		void SetLinePipeline();

		/// <summary>
		/// UI用のspriteパイプラインのセット
		/// </summary>
		void SetUISpritePipeline();

		/// <summary>
		/// skybox pipeline
		/// </summary>
		void SetSkyBoxPipeline();

		/// <summary>
		/// VFX用のパイプライン
		/// </summary>
		void SetVfxPipeline();

		/// <summary>
		/// VFX用の定数バッファをセットする
		/// </summary>
		void SetVfxConstantBuffer(uint32_t rootIndex, const void* data, size_t size);
	private:
		/// <summary>
		/// 基本描画関数
		/// </summary>
		void Draw(
			const void* VertexData, const size_t VertexStride, const size_t VertexCount,
			const void* IndexData, const eIndexBufferFormat IndexFormat, const size_t IndexCount);

		//	パイプラインのセット
		void SetRootSignature(ID3D12RootSignature* RootSignature);
		void SetPipelineState(ID3D12PipelineState* PipelineState);
		void SetTopology(D3D_PRIMITIVE_TOPOLOGY Topology);

	private:
		/// <summary>
		/// 今のパイプラインの状態
		/// </summary>
		struct CurrentPipeline
		{
			ID3D12RootSignature* RootSignature;
			ID3D12PipelineState* PipelineState;
			D3D_PRIMITIVE_TOPOLOGY Topology;
		};

		struct CameraBuffer
		{
			Math::Matrix View;
			Math::Matrix Projection;
		};

		std::unique_ptr<VertexBuffer> mVertexBuffer;
		std::unique_ptr<IndexBuffer>  mIndexBuffer;
		std::unique_ptr<ConstantBuffer> mConstantBuffer;
		std::unique_ptr<ConstantBuffer> mVfxConstantBuffer;

		size_t mVertexBufferOffset = 0;
		size_t mIndexBufferOffset = 0;

		size_t mUsingVertexBufferSize = 0;
		size_t mUsingIndexBufferSize = 0;

		/// <summary>
		/// 今のパイプラインの状態
		/// </summary>
		CurrentPipeline mCurrentPipeline;

		/// <summary>
		/// Sprite用パイプライン
		/// </summary>
		SpritePipeline mSpritePipeline;

		/// <summary>
		/// Fbx用パイプライン
		/// </summary>
		FbxPipeline mFbxPipeline;

		/// <summary>
		/// 線描画用のパイプライン（今回はデバックの可視化で使用します）
		/// </summary>
		LinePipeline mLinePipeline;

		/// <summary>
		/// UIスプライト用のパイプラインのセット
		/// </summary>
		UISpritePipeline mUISpritePipeline;

		/// <summary>
		/// スカイボックス用のパイプライン
		/// </summary>
		SkyBoxPipeline mSkyBoxPipeline;

		/// <summary>
		/// VFX用のパイプライン
		/// </summary>
		VFXPipeline mVfxPipeline;

		/// <summary>
		/// dx12コマンドリスト
		/// </summary>
		ID3D12GraphicsCommandList* mCmdList = nullptr;
	};
}

