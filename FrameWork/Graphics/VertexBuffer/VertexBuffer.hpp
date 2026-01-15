#pragma once
#include <d3d12.h>
#include <stdint.h>

#include"Graphics/DirectX/TypeAlias/DirectX12TypeAlias.hpp"

namespace Engine::Graphics
{
	class VertexBuffer
	{
	public:
		VertexBuffer();
		virtual ~VertexBuffer();

		/// <summary>
		/// 頂点バッファの作成
		/// </summary>
		/// <param name="size">バッファのサイズ</param>
		/// <param name="stride">1頂点のデータサイズ</param>
		/// <returns>true:成功</returns>
		bool Create(const size_t Size, const size_t Stride);

		/// <summary>
		/// バッファの解放
		/// </summary>
		void Release();

		/// <summary>
		/// バッファの更新
		/// </summary>
		/// <param name="SrcData">更新データ</param>
		void Update(const void* SrcData);

		/// <summary>
		/// バッファの更新
		/// </summary>
		/// <param name="SrcData">更新データ<</param>
		/// <param name="offset">オフセット値</param>
		/// <param name="size">更新サイズ</param>
		void Update(const void* SrcData, const size_t Offset, const size_t Size);

		/// <summary>
		/// 頂点バッファをパイプラインへセット
		/// </summary>
		void Set()const;

		void Set(const size_t Offset, const size_t Stride, const size_t Size)const;

		/// <summary>
		/// 頂点バッファのサイズの取得
		/// </summary>
		/// <returns></returns>
		size_t GetSize()const;

	private:
		/// <summary>
		/// 頂点バッファリソース
		/// </summary>
		mw::ResourcePtr mBufferResource;

		/// <summary>
		/// 頂点バッファの先頭アドレス
		/// </summary>
		D3D12_GPU_VIRTUAL_ADDRESS mBufferLocation;

		/// <summary>
		/// 1頂点のサイズ
		/// </summary>
		size_t mStride;

		/// <summary>
		/// 頂点バッファのサイズ
		/// </summary>
		size_t mBufferSize;

	};
}
