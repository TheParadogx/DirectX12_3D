#pragma once

#include"Graphics/DirectX/TypeAlias/DirectX12TypeAlias.hpp"

#include<cstdint>
#include <memory>
#include<filesystem>

namespace Engine::Graphics
{
	class Texture;
	class SkyBoxResource
	{
		/// <summary>
		/// 立方体メッシュの作成
		/// </summary>
		/// <returns>true:成功</returns>
		bool CreateNesh();

	public:
		SkyBoxResource();
		virtual ~SkyBoxResource();

		/// <summary>
		/// 生成
		/// </summary>
		/// <param name="CubeMapPath">キューブマップテクスチャのパス</param>
		/// <returns>true:成功</returns>
		bool Create(const std::filesystem::path& CubeMapPath);

		/// <summary>
		/// リソースの解放
		/// </summary>
		void Release();

		//	ゲッター
		D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView() const;

	private:
		/// <summary>
		/// 頂点バッファ
		/// </summary>
		mw::ResourcePtr mVB = nullptr;
		D3D12_VERTEX_BUFFER_VIEW mVBView;

		/// <summary>
		/// キューブマップテクスチャ
		/// </summary>
		Texture* mSkyTexture = nullptr;

		/// <summary>
		/// 6面＊2枚＊3頂点＝３２頂点
		/// </summary>
		static constexpr uint32_t mVertexCount = 32;
	};
}


