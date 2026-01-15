#include "pch.h"
#include "VertexBuffer.hpp"

#include "Graphics/DirectX/DirectX12.hpp"

Engine::Graphics::VertexBuffer::VertexBuffer()
	:mBufferResource(nullptr)
	, mBufferLocation()
	, mStride()
	, mBufferSize()
{
}

Engine::Graphics::VertexBuffer::~VertexBuffer()
{
	Release();
}

/// <summary>
/// 頂点バッファの作成
/// </summary>
/// <param name="size">バッファのサイズ</param>
/// <param name="stride">1頂点のデータサイズ</param>
/// <returns>true:成功</returns>
bool Engine::Graphics::VertexBuffer::Create(const size_t Size, const size_t Stride)
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
	mStride = Stride;

	return true;
}

/// <summary>
/// バッファの解放
/// </summary>
void Engine::Graphics::VertexBuffer::Release()
{
	mBufferResource.Reset();
	mBufferLocation = 0;
	mBufferSize = 0;
	mStride = 0;
}

/// <summary>
/// バッファの更新
/// </summary>
/// <param name="SrcData">更新データ</param>
void Engine::Graphics::VertexBuffer::Update(const void* SrcData)
{
	Update(SrcData, 0, GetSize());
}

/// <summary>
/// バッファの更新
/// </summary>
/// <param name="SrcData">更新データ<</param>
/// <param name="offset">オフセット値</param>
/// <param name="size">更新サイズ</param>
void Engine::Graphics::VertexBuffer::Update(const void* SrcData, const size_t Offset, const size_t Size)
{
	D3D12_RANGE Range = {};
	Range.Begin = static_cast<SIZE_T>(Offset);
	Range.End = Range.Begin + Size;

	uint8_t* pData = nullptr;
	const HRESULT hr = mBufferResource->Map(0, &Range, reinterpret_cast<void**>(&pData));
	if (SUCCEEDED(hr))
	{
		memcpy(pData + Offset, SrcData, Size);
		mBufferResource->Unmap(0, &Range);
	}

}

/// <summary>
/// 頂点バッファをパイプラインへセット
/// </summary>
void Engine::Graphics::VertexBuffer::Set() const
{
	D3D12_VERTEX_BUFFER_VIEW vbv = {};
	vbv.BufferLocation = mBufferLocation;
	vbv.SizeInBytes = static_cast<UINT>(mBufferSize);
	vbv.StrideInBytes = static_cast<UINT>(mStride);
	ID3D12GraphicsCommandList* comandlist = Graphics::DirectX::GetInstance()->GetCommandList();
	comandlist->IASetVertexBuffers(0, 1, &vbv);

}
void Engine::Graphics::VertexBuffer::Set(const size_t Offset, const size_t Stride, const size_t Size) const
{
	D3D12_VERTEX_BUFFER_VIEW vbv = {};
	vbv.BufferLocation = mBufferLocation + static_cast<D3D12_GPU_VIRTUAL_ADDRESS>(Offset);
	vbv.SizeInBytes = static_cast<UINT>(Size);
	vbv.StrideInBytes = static_cast<UINT>(Stride);
	ID3D12GraphicsCommandList* comandlist = Graphics::DirectX::GetInstance()->GetCommandList();
	comandlist->IASetVertexBuffers(0, 1, &vbv);

}

/// <summary>
/// 頂点バッファのサイズの取得
/// </summary>
/// <returns></returns>
size_t Engine::Graphics::VertexBuffer::GetSize()const
{
	return mBufferSize;
}