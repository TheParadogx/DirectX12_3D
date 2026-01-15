#include "pch.h"
#include "Texture.hpp"
#include "Debug/spdlog/Logger.hpp"
#include"Graphics/DirectX/DirectX12.hpp"
#include"Graphics/GraphicsDescriptorHeap/GraphicsDescriptorHeapManager.hpp"

namespace fs = std::filesystem;
namespace DXTex = DirectX;

D3D12_RESOURCE_DESC Engine::Graphics::Texture::GetTextureResourceDesc(const DXTex::TexMetadata& MetaData)
{
	D3D12_RESOURCE_DESC Desc = {};
	Desc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(MetaData.dimension);
	Desc.Alignment = 0;
	Desc.Width = static_cast<UINT64>(MetaData.width);
	Desc.Height = static_cast<UINT>(MetaData.height);
	Desc.DepthOrArraySize = static_cast<UINT16>(MetaData.arraySize);
	Desc.MipLevels = static_cast<UINT16>(MetaData.mipLevels);
	Desc.Format = MetaData.format;
	Desc.SampleDesc.Count = 1;
	Desc.SampleDesc.Quality = 0;
	Desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	Desc.Flags = D3D12_RESOURCE_FLAG_NONE;

	return Desc;
}

/// <summary>
/// 作成
/// </summary>
/// <param name="FilePath">フォルダパス</param>
/// <returns>true:成功</returns>
bool Engine::Graphics::Texture::Create(const std::filesystem::path& FilePath)
{
	if (fs::exists(FilePath) == false)
	{
		LOG_ERROR("ファイルが見つかりませんでした" + FilePath.string());
		return false;
	}

	const fs::path extension = FilePath.extension();
	DXTex::TexMetadata texMetaData = {};
	DXTex::ScratchImage scratchImage = {};
	HRESULT hr = S_FALSE;
	std::wstring path = FilePath.wstring();
	if (extension.compare(".dds") == 0)
	{
		hr = DXTex::LoadFromDDSFile(
			path.c_str(),
			DXTex::DDS_FLAGS_NONE,
			&texMetaData,
			scratchImage);
	}
	else if (extension.compare(".tga") == 0)
	{
		hr = DXTex::LoadFromTGAFile(
			path.c_str(),
			DXTex::TGA_FLAGS_NONE,
			&texMetaData,
			scratchImage);
	}
	else if (extension.compare(".hdr") == 0)
	{
		hr = DXTex::LoadFromHDRFile(
			path.c_str(),
			&texMetaData,
			scratchImage);
	}
	else
	{
		hr = DXTex::LoadFromWICFile(
			path.c_str(),
			DXTex::WIC_FLAGS_NONE,
			&texMetaData,
			scratchImage);
	}
	if (FAILED(hr))
	{
		LOG_ERROR("Failed Load Texture");
		return false;
	}

	const DXTex::Image* image = scratchImage.GetImage(0, 0, 0);
	Graphics::DirectX* directX = Graphics::DirectX::GetInstance();
	ID3D12Device* device = directX->GetDevice();

	//	読み込んだテクスチャからコピー用情報の取得
	const D3D12_RESOURCE_DESC TextureResourceDesc = GetTextureResourceDesc(texMetaData);
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT footpoint = {};
	UINT row = 0;
	UINT64 rowsizeinbyte = 0;
	UINT64 totalbyte = 0;
	device->GetCopyableFootprints(
		&TextureResourceDesc, 0, 1, 0,
		&footpoint, &row, &rowsizeinbyte, &totalbyte);

	mw::CmdAllocatorPtr tempAlloc;
	hr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&tempAlloc));
	if (FAILED(hr))
	{
		LOG_ERROR("Failed CreateCommandAllocator");
		return false;
	}
	mw::ComPtr<ID3D12GraphicsCommandList> tempCmdList;
	hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, tempAlloc.Get(), nullptr, IID_PPV_ARGS(&tempCmdList));
	if (FAILED(hr))
	{
		LOG_ERROR("Failed CreateCommandList");
		return false;
	}

	//	アップロード用中間バッファ作成
	D3D12_HEAP_PROPERTIES UploadProperty = {};
	UploadProperty.Type = D3D12_HEAP_TYPE_UPLOAD;
	UploadProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	UploadProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	UploadProperty.CreationNodeMask = 0;
	UploadProperty.VisibleNodeMask = 0;

	D3D12_RESOURCE_DESC UploadResourceDesc = {};
	UploadResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	UploadResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	UploadResourceDesc.Width = totalbyte;
	UploadResourceDesc.Height = 1;
	UploadResourceDesc.DepthOrArraySize = 1;
	UploadResourceDesc.MipLevels = 1;
	UploadResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	UploadResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	UploadResourceDesc.SampleDesc.Count = 1;
	UploadResourceDesc.SampleDesc.Quality = 0;

	mw::ResourcePtr UploadBuffer = nullptr;
	hr = device->CreateCommittedResource(
		&UploadProperty, D3D12_HEAP_FLAG_NONE, &UploadResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&UploadBuffer));
	if (FAILED(hr))
	{
		return false;
	}

	//	ヒープ設定
	D3D12_HEAP_PROPERTIES HeapProperty = {};
	HeapProperty.Type = D3D12_HEAP_TYPE_DEFAULT;
	HeapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	HeapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	HeapProperty.CreationNodeMask = 0;
	HeapProperty.VisibleNodeMask = 0;

	//	テクスチャ用バッファ作成
	hr = device->CreateCommittedResource(
		&HeapProperty,
		D3D12_HEAP_FLAG_NONE,
		&TextureResourceDesc,
		D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&mResource));
	if (FAILED(hr))
	{
		return false;
	}

	void* Data = nullptr;
	hr = UploadBuffer->Map(0, nullptr, &Data);
	if (SUCCEEDED(hr))
	{
		// footpoint は GetCopyableFootprints で取得済み
		const BYTE* srcPixels = scratchImage.GetPixels();
		const UINT srcRowPitch = image->rowPitch;         // CPU側の1行バイト数
		const UINT numRows = row;                       // GetCopyableFootprints で得た行数
		const UINT64 dstRowPitch = footpoint.Footprint.RowPitch; // GPU 側の RowPitch (aligned)
		const UINT64 dstOffset = footpoint.Offset;     // 通常は 0 だが使うのが安全

		BYTE* dstBase = reinterpret_cast<BYTE*>(Data) + dstOffset;

		for (UINT r = 0; r < numRows; ++r)
		{
			// コピー先行先頭アドレス + r * dstRowPitch
			BYTE* dstRow = dstBase + static_cast<size_t>(r) * static_cast<size_t>(dstRowPitch);
			const BYTE* srcRow = srcPixels + static_cast<size_t>(r) * static_cast<size_t>(srcRowPitch);

			// CPU側の1行分だけコピーする（RowPitch が >= srcRowPitch）
			memcpy(dstRow, srcRow, srcRowPitch);
			// もし dstRowPitch > srcRowPitch でゼロクリアが必要なら以下を有効に
			// if (dstRowPitch > srcRowPitch) memset(dstRow + srcRowPitch, 0, dstRowPitch - srcRowPitch);
		}
		UploadBuffer->Unmap(0, nullptr);
	}

	//	コピー元バッファ設定(アップロード側)
	D3D12_TEXTURE_COPY_LOCATION Src = {};
	Src.pResource = UploadBuffer.Get();
	Src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	Src.PlacedFootprint = footpoint;

	//	コピー先バッファ設定(テクスチャとして参照する側)
	D3D12_TEXTURE_COPY_LOCATION Dest = {};
	Dest.pResource = mResource.Get();
	Dest.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	Dest.SubresourceIndex = 0;

	//	バッファのコピー
	tempCmdList->CopyTextureRegion(&Dest, 0, 0, 0, &Src, nullptr);
	
	//	バリアー処理
	D3D12_RESOURCE_BARRIER Barrier = {};
	Barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	Barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	Barrier.Transition.pResource = mResource.Get();
	Barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	Barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	Barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	tempCmdList->ResourceBarrier(1, &Barrier);

	//	命令の実行
	tempCmdList->Close();
	//	コマンドリスト実行
	ID3D12CommandList* cmdLists[] = { tempCmdList.Get() };
	ID3D12CommandQueue* CommandQueue = directX->GetCommandQueue();
	CommandQueue->ExecuteCommandLists(1, cmdLists);
	//	実行中なのでフェンス待ち
	directX->WaitForGPU();

	//	シェーダーリソース作成
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = texMetaData.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texMetaData.mipLevels;

	auto GDHManager = Graphics::GraphicsDescriptorHeapManager::GetInstance();
	mHeapInfo = GDHManager->Issuance(1);
	const D3D12_CPU_DESCRIPTOR_HANDLE CpuHandle = GDHManager->GetCpuHandle(mHeapInfo);
	device->CreateShaderResourceView(mResource.Get(), &srvDesc, CpuHandle);

	mWidth = static_cast<float>(texMetaData.width);
	mHeight = static_cast<float>(texMetaData.height);

	UploadBuffer.Reset();
	std::cout << "Loading Texture:" << FilePath.string() << std::endl;

	return true;
}

/// <summary>
/// リソースの解放
/// </summary>
void Engine::Graphics::Texture::Release()
{
	mResource.Reset();
}

/// <summary>
/// パイプラインへのセット
/// </summary>
/// <param name="RootParameterIndex"></param>
void Engine::Graphics::Texture::Set(int RootParameterIndex)
{
	GraphicsDescriptorHeapManager* GDHManager = GraphicsDescriptorHeapManager::GetInstance();
	GDHManager->Set(RootParameterIndex, mHeapInfo);
}

/// <summary>
/// テクスチャの幅
/// </summary>
/// <returns></returns>
float Engine::Graphics::Texture::GetWidth()const
{
	return mWidth;
}
/// <summary>
/// テクスチャの高さ
/// </summary>
/// <returns></returns>
float Engine::Graphics::Texture::GetHeight()const
{
	return mHeight;
}
