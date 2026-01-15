#pragma once
#include"Graphics/DirectX/TypeAlias/DirectX12TypeAlias.hpp"
#include"Graphics/GraphicsDescriptorHeap/GraphicsDescriptorHeapInfo.hpp"

#include<Plugin/DirectXTex/DirectXTex.h>
#include<filesystem>

namespace DXTex = DirectX;

namespace Engine::Graphics
{
	class Texture
	{

		/// <summary>
		/// 読み込んだテクスチャの情報取得
		/// </summary>
		/// <param name="MetaData"></param>
		/// <returns></returns>
		D3D12_RESOURCE_DESC GetTextureResourceDesc(const DXTex::TexMetadata& MetaData);

	public:
		/// <summary>
		/// 作成
		/// </summary>
		/// <param name="FilePath">フォルダパス</param>
		/// <returns>true:成功</returns>
		bool Create(const std::filesystem::path& FilePath);

		/// <summary>
		/// リソースの解放
		/// </summary>
		void Release();

		/// <summary>
		/// パイプラインへのセット
		/// </summary>
		/// <param name="RootParameterIndex"></param>
		void Set(int RootParameterIndex);

		/// <summary>
		/// テクスチャの幅
		/// </summary>
		/// <returns></returns>
		float GetWidth()const;
		/// <summary>
		/// テクスチャの高さ
		/// </summary>
		/// <returns></returns>
		float GetHeight()const;
	private:
		/// <summary>
		/// テクスチャ用リソース
		/// </summary>
		mw::ResourcePtr mResource;

		/// <summary>
		/// ヒープ情報
		/// </summary>
		DescriptorHeapInfo mHeapInfo;

		/// <summary>
		/// テクスチャ横幅
		/// </summary>
		float mWidth;
		/// <summary>
		/// テクスチャ縦幅
		/// </summary>
		float mHeight;

	};
}


