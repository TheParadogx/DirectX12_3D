#pragma once
#include"Graphics/DirectX/TypeAlias/DirectX12TypeAlias.hpp"


namespace Engine::Graphics
{
	/// <summary>
	/// Fbx用のパイプライン
	/// </summary>
	class FbxPipeline
	{		
		/// <summary>
		/// 深度ステンシルの初期設定の取得
		/// </summary>
		/// <returns></returns>
		D3D12_DEPTH_STENCIL_DESC GetDepthStencilDesc();

		/// <summary>
		/// ブレンドステートの初期設定の取得
		/// </summary>
		/// <returns></returns>
		D3D12_BLEND_DESC GetBlendStateDesc();

		/// <summary>
		/// ラスタライザーの初期設定の取得
		/// </summary>
		/// <returns></returns>
		D3D12_RASTERIZER_DESC GetRasterizerDesc();

		/// <summary>
		/// ルートシグネチャの作成
		/// </summary>
		/// <returns>true:成功</returns>
		bool CreateRootSignature();

		/// <summary>
		/// 描画パイプラインの作成
		/// </summary>
		/// <returns></returns>
		bool CreatePipeline();
	public:
		/// <summary>
		/// 生成
		/// </summary>
		/// <returns>true:成功</returns>
		bool Create();

		/// <summary>
		/// ルートシグネチャの取得
		/// </summary>
		/// <returns></returns>
		ID3D12RootSignature* GetRootSignature()const;

		/// <summary>
		/// パイプラインステートの取得
		/// </summary>
		/// <returns></returns>
		ID3D12PipelineState* GetPipelineState()const;

		/// <summary>
		/// 描画方法
		/// </summary>
		/// <returns>三角形</returns>
		D3D_PRIMITIVE_TOPOLOGY GetTopology()const;
	private:
		/// <summary>
		/// dx12ルートシグネチャ
		/// </summary>
		mw::RootSignaturePtr mRootSignature = nullptr;

		/// <summary>
		/// dx12パイプラインステート
		/// </summary>
		mw::PipelineStatePtr mPipelineState = nullptr;
	};
}


