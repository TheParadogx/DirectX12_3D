#pragma once
#include <d3d12.h>
#include <memory>
#include"Graphics/DirectX/DirectX12.hpp"




namespace Engine::Graphics
{
	class GraphicsDescriptorHeap;

	class ConstantBuffer
	{
	public:
		ConstantBuffer();
		virtual ~ConstantBuffer();

		/// <summary>
		/// 定数バッファの作成
		/// </summary>
		/// <param name="Size">定数バッファのサイズ</param>
		/// <returns>true:成功</returns>
		bool Create(size_t Size);

		/// <summary>
		/// バッファの解放
		/// </summary>
		void Release();

		/// <summary>
		/// バッファの更新
		/// </summary>
		/// <param name="SrcData"></param>
		void Update(const void* SrcData);

		/// <summary>
		/// バッファをパイプラインへセット
		/// </summary>
		/// <param name="RootParameterIndex">ルートシグネチャへセットしたパラメーターインデックス</param>
		void Set(int RootParameterIndex);

	private:
		/// <summary>
		/// 定数バッファリソース
		/// </summary>
		mw::ResourcePtr mBufferResource;

		/// <summary>
		/// リソースのディスクリプタ情報格納
		/// </summary>
		std::unique_ptr<GraphicsDescriptorHeap> mHeap;
		/// <summary>
		/// 送りたいバッファのサイズ
		/// </summary>
		UINT64 mBufferSize;
		/// <summary>
		/// GPUの規則に合わせてアライメントされたサイズ
		/// </summary>
		UINT64 mAlignedBufferSize;
	};




}
