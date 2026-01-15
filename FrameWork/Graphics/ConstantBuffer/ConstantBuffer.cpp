#include "pch.h"
#include "ConstantBuffer.hpp"
#include "Graphics/GraphicsDescriptorHeap/GraphicsDescriptorHeap.hpp"
#include"Graphics/DirectX/DirectX12.hpp"

Engine::Graphics::ConstantBuffer::ConstantBuffer()
	: mBufferResource(nullptr)
	, mHeap(nullptr)
	, mBufferSize(0)
	, mAlignedBufferSize(0)
{
}
Engine::Graphics::ConstantBuffer::~ConstantBuffer()
{
	Release();
}

/// <summary>
/// 定数バッファの作成
/// </summary>
/// <param name="Size">定数バッファのサイズ</param>
/// <returns>true:成功</returns>
bool Engine::Graphics::ConstantBuffer::Create(size_t Size)
{
	mBufferSize = static_cast<UINT64>(Size);
	//	256byte単位で作成されるよう算出
	mAlignedBufferSize = (mBufferSize + 0xff) & ~0xff;

	//	ヒープ設定
	D3D12_HEAP_PROPERTIES heapDesc = {};
	heapDesc.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapDesc.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapDesc.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapDesc.CreationNodeMask = 1;
	heapDesc.VisibleNodeMask = 1;

	//	バッファのリソース
	D3D12_RESOURCE_DESC resourceDesc = {};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Alignment = 0;
	resourceDesc.Width = mAlignedBufferSize;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.SampleDesc.Quality = 0;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	ID3D12Device* device = DirectX::GetInstance()->GetDevice();
	const HRESULT hr = device->CreateCommittedResource(
		&heapDesc,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&mBufferResource));

	if (FAILED(hr))
	{
		return false;
	}

	mHeap = std::make_unique<GraphicsDescriptorHeap>();
	const bool ret = mHeap->Create();
	if (ret == false)
	{
		mBufferResource.Reset();
		return false;
	}

	//	CBV作成
	D3D12_CONSTANT_BUFFER_VIEW_DESC CBV = {};
	CBV.BufferLocation = mBufferResource->GetGPUVirtualAddress();
	CBV.SizeInBytes = static_cast<UINT>(mAlignedBufferSize);
	const D3D12_CPU_DESCRIPTOR_HANDLE CpuHandle = mHeap->GetCpuHandle();
	device->CreateConstantBufferView(&CBV, CpuHandle);

	return true;
}

/// <summary>
/// バッファの解放
/// </summary>
void Engine::Graphics::ConstantBuffer::Release()
{
	mBufferResource.Reset();
	if (mHeap != nullptr)
	{
		mHeap.reset();
	}
}

/// <summary>
/// バッファの更新
/// </summary>
/// <param name="SrcData"></param>
void Engine::Graphics::ConstantBuffer::Update(const void* SrcData)
{
	D3D12_RANGE Range = {};
	Range.Begin = 0;
	Range.End = static_cast<SIZE_T>(mBufferSize);

	void* pData = nullptr;
	const HRESULT hr = mBufferResource->Map(0, &Range, &pData);
	if (SUCCEEDED(hr))
	{
		memcpy(pData, SrcData, Range.End);
		mBufferResource->Unmap(0, &Range);
	}
}

/// <summary>
/// バッファをパイプラインへセット
/// </summary>
/// <param name="RootParameterIndex">ルートシグネチャへセットしたパラメーターインデックス</param>
void Engine::Graphics::ConstantBuffer::Set(int RootParameterIndex)
{
	auto cmdList = DirectX::GetInstance()->GetCommandList();

	// Root Signature で TYPE_CBV にした場合は、こちらを呼ぶのがルール
	cmdList->SetGraphicsRootConstantBufferView(
		RootParameterIndex,
		mBufferResource->GetGPUVirtualAddress()
	);

	//mHeap->Set(RootParameterIndex);
}