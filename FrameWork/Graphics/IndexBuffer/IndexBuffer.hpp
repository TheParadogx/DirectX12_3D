#pragma once

#include"Graphics/DirectX/TypeAlias/DirectX12TypeAlias.hpp"

namespace Engine::Graphics
{

	/// <summary>
	/// インデックスバッファのフォーマット
	/// </summary>
	enum class eIndexBufferFormat : uint8_t
	{
		Uint32,
		Uint16,
	};
	
	/// <summary>
	/// インデックスバッファ用クラス
	/// </summary>
	class IndexBuffer
	{
	public:
		IndexBuffer();
		virtual ~IndexBuffer();

		/// <summary>
		/// インデックスバッファの作成
		/// </summary>
		/// <param name="size">バッファサイズ</param>
		/// <returns></returns>
		bool Create(const size_t Size);

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
		/// <param name="SrcData">更新データ</param>
		/// <param name="offset">オフセット値</param>
		/// <param name="size">更新サイズ</param>
		void Update(const void* SrcData, const size_t Offset, const size_t Size);

		/// <summary>
		/// インデックスバッファをパイプラインへセット
		/// </summary>
		void Set()const;
		void Set(const eIndexBufferFormat Format, const size_t Offset, const size_t Size)const;

		/// <summary>
		/// バッファサイズの取得
		/// </summary>
		/// <returns></returns>
		size_t GetSize()const;
	private:
		/// <summary>
		/// インデックスバッファリソース
		/// </summary>
		mw::ResourcePtr mBufferResource;

		/// <summary>
		/// インデックスバッファの先頭アドレス
		/// </summary>
		D3D12_GPU_VIRTUAL_ADDRESS mBufferLocation;

		/// <summary>
		/// バッファのサイズ
		/// </summary>
		size_t mBufferSize;
	};
}

