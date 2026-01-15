#include "pch.h"
#include "IndexBuffer.hpp"
#include "Graphics/DirectX/DirectX12.hpp"

#include "Debug/spdlog/Logger.hpp"

Engine::Graphics::IndexBuffer::IndexBuffer()
{

}

Engine::Graphics::IndexBuffer::~IndexBuffer()
{

}

/// <summary>
/// インデックスバッファの作成
/// </summary>
/// <param name="size">バッファサイズ</param>
/// <returns></returns>
bool Engine::Graphics::IndexBuffer::Create(const size_t Size)
{
	//	ヒープ設定
	D3D12_HEAP_PROPERTIES heapDesc = {};
	heapDesc.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapDesc.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapDesc.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapDesc.CreationNodeMask = 0;
	heapDesc.VisibleNodeMask = 0;

	//	バッファのリソース
	D3D12_RESOURCE_DESC resourceDesc = {};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Alignment = 0;
	resourceDesc.Width = static_cast<UINT64>(Size);
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	ID3D12Device* device = Graphics::DirectX::GetInstance()->GetDevice();
	const HRESULT hr = device->CreateCommittedResource(&heapDesc, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&mBufferResource));
	if (FAILED(hr))
	{
		return false;
	}

	mBufferLocation = mBufferResource->GetGPUVirtualAddress();
	mBufferSize = Size;

	return true;

}

/// <summary>
/// バッファの解放
/// </summary>
void Engine::Graphics::IndexBuffer::Release()
{
	mBufferResource.Reset();
}

/// <summary>
/// バッファの更新
/// </summary>
/// <param name="SrcData">更新データ</param>
void Engine::Graphics::IndexBuffer::Update(const void* SrcData)
{
	Update(SrcData, 0, GetSize());
}

/// <summary>
/// バッファの更新
/// </summary>
/// <param name="SrcData">更新データ</param>
/// <param name="offset">オフセット値</param>
/// <param name="size">更新サイズ</param>
void Engine::Graphics::IndexBuffer::Update(const void* SrcData, const size_t Offset, const size_t Size)
{
	D3D12_RANGE Range = {};
	Range.Begin = static_cast<SIZE_T>(Offset);
	Range.End = Range.Begin + static_cast<SIZE_T>(Size);

	uint8_t* pData = nullptr;
	const HRESULT hr = mBufferResource->Map(0, &Range, reinterpret_cast<void**>(&pData));
	if (SUCCEEDED(hr))
	{
		memcpy(pData + Offset, SrcData, Size);
		mBufferResource->Unmap(0, &Range);
	}
}

/// <summary>
/// インデックスバッファをパイプラインへセット
/// </summary>
void Engine::Graphics::IndexBuffer::Set()const
{
	D3D12_INDEX_BUFFER_VIEW ibv = {};
	ibv.BufferLocation = mBufferLocation;
	ibv.SizeInBytes = static_cast<UINT>(mBufferSize);
	ibv.Format = DXGI_FORMAT_R32_UINT;
	ID3D12GraphicsCommandList* comandlist = Graphics::DirectX::GetInstance()->GetCommandList();
	comandlist->IASetIndexBuffer(&ibv);
}
void Engine::Graphics::IndexBuffer::Set(const eIndexBufferFormat Format, const size_t Offset, const size_t Size) const
{
	D3D12_INDEX_BUFFER_VIEW ibv = {};
	ibv.BufferLocation = mBufferLocation + static_cast<D3D12_GPU_VIRTUAL_ADDRESS>(Offset);
	ibv.SizeInBytes = static_cast<UINT>(Size);
	switch (Format)
	{
	case eIndexBufferFormat::Uint32:
		ibv.Format = DXGI_FORMAT_R32_UINT;
		break;
	case eIndexBufferFormat::Uint16:
		ibv.Format = DXGI_FORMAT_R16_UINT;
		break;
	default:
		LOG_CRITICAL("Invalid IndexBufferFormat");
		break;
	}
	ID3D12GraphicsCommandList* comandlist = Graphics::DirectX::GetInstance()->GetCommandList();
	comandlist->IASetIndexBuffer(&ibv);

}

/// <summary>
/// バッファサイズの取得
/// </summary>
/// <returns></returns>
size_t Engine::Graphics::IndexBuffer::GetSize()const
{
	return mBufferSize;
}