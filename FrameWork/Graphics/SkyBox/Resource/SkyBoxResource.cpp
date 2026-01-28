#include "pch.h"
#include "SkyBoxResource.hpp"
#include "Graphics/Texture/Texture.hpp"

namespace Engine::Graphics
{
	/// <summary>
	/// 立方体メッシュの作成
	/// </summary>
	/// <returns>true:成功</returns>
	bool SkyBoxResource::CreateNesh()
	{
		return false;
	}

	SkyBoxResource::SkyBoxResource()
	{

	}
	SkyBoxResource::~SkyBoxResource()
	{
		Release();
	}

	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="CubeMapPath">キューブマップテクスチャのパス</param>
	/// <returns>true:成功</returns>
	bool SkyBoxResource::Create(const std::filesystem::path& CubeMapPath)
	{

	}

	/// <summary>
	/// リソースの解放
	/// </summary>
	void SkyBoxResource::Release()
	{

	}

	D3D12_VERTEX_BUFFER_VIEW SkyBoxResource::GetVertexBufferView()const
	{

	}
}