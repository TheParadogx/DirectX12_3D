#include "pch.h"
#include "GraphicsDescriptorHeapManager.hpp"
#include "Graphics/DirectX/DirectX12.hpp" 
#include "Debug/spdlog/Logger.hpp"

/// <summary>
/// 初期値
/// </summary>
void Engine::Graphics::GraphicsDescriptorHeapManager::OnCreate()
{
	mGraphicsHeap = nullptr;
	mInfo.fill({});
	mInUse.fill(false);
	mSearchOffset = 0;
}

/// <summary>
/// リソース解放
/// </summary>
void Engine::Graphics::GraphicsDescriptorHeapManager::OnDestory()
{
	mGraphicsHeap.Reset();
}

/// <summary>
/// 初期化
/// </summary>
/// <returns>true:成功</returns>
bool Engine::Graphics::GraphicsDescriptorHeapManager::Initialize()
{
	ID3D12Device* device = DirectX::GetInstance()->GetDevice();
	if (device == nullptr)
	{
		LOG_CRITICAL("Dx12デバイスの取得に失敗");
		return false;
	}

	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = MAX_DESCRIPTOR;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask = 0;
	const HRESULT hr = device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&mGraphicsHeap));
	if (FAILED(hr))
	{
		return false;
	}

	const D3D12_CPU_DESCRIPTOR_HANDLE cpuStart = mGraphicsHeap->GetCPUDescriptorHandleForHeapStart();
	const D3D12_GPU_DESCRIPTOR_HANDLE gpuStart = mGraphicsHeap->GetGPUDescriptorHandleForHeapStart();
	const SIZE_T incrementSize = static_cast<SIZE_T>(device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
	SIZE_T off = 0;
	for (auto& itr : mInfo)
	{
		itr.Cpu = cpuStart;
		itr.Cpu.ptr += off;

		itr.Gpu = gpuStart;
		itr.Gpu.ptr += off;

		off += incrementSize;
	}
	mInUse.fill(false);

	return true;
}

/// <summary>
/// ヒープから使用領域の発行
/// </summary>
/// <param name="size"></param>
/// <returns></returns>
Engine::Graphics::DescriptorHeapInfo Engine::Graphics::GraphicsDescriptorHeapManager::Issuance(uint32_t Size)
{
	int searchStart = mSearchOffset;
	for (int count = 0; count < MAX_DESCRIPTOR;)
	{
		//	配列の範囲内に収めるための剰余演算
		int currentOffset = (searchStart + count) % MAX_DESCRIPTOR;

		//	指定サイズがヒープの末尾をまたぐ場合は確保不可能
		if (currentOffset + Size > MAX_DESCRIPTOR)
		{
			//	次のチェックは先頭から
			count += (MAX_DESCRIPTOR - currentOffset);
			continue;
		}

		//	連続した空き領域があるかどうか
		bool bCheck = true;
		int conflictIndex = -1;
		for (int s = 0; s < Size; s++)
		{
			if (mInUse[currentOffset + s] == true)
			{
				bCheck = false;
				conflictIndex = s;
				break;
			}
		}

		if (bCheck == true)
		{
			//	確保処理
			for (int i = 0; i < Size; i++)
			{
				mInUse[currentOffset + i] = true;
			}

			//	次回の検索開始位置を更新
			mSearchOffset = (currentOffset + Size) % MAX_DESCRIPTOR;

			DescriptorHeapInfo info = { currentOffset ,Size};
			return info;
		}

		//	見つからなかったら衝突した要素の次までスキップ
		count += (conflictIndex + 1);

	}
	return DescriptorHeapInfo(-1.-1);
}

/// <summary>
/// 使用領域の破棄
/// </summary>
/// <param name="Info"></param>
void Engine::Graphics::GraphicsDescriptorHeapManager::Discard(DescriptorHeapInfo& Info)
{
	if (Info.index < 0)
	{
		return;
	}

	const int lastIndex = Info.index+ Info.size;
	for (int index = Info.index; index < lastIndex; index++)
	{
		mInUse[index] = false;
	}

	Info.index = -1;
	Info.size = -1;
}

void Engine::Graphics::GraphicsDescriptorHeapManager::Set(int RootParameterIndex, const DescriptorHeapInfo& Info)
{
	if (mGraphicsHeap == nullptr)
	{
		return;
	}
	if (Info.index < 0)
	{
		return;
	}
	ID3D12GraphicsCommandList* CommandList = Graphics::DirectX::GetInstance()->GetCommandList();

	CommandList->SetDescriptorHeaps(1, mGraphicsHeap.GetAddressOf());
	// 【修正】現在のヒープの先頭から、Info.index 分だけ進めたハンドルを計算する
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = mGraphicsHeap->GetGPUDescriptorHandleForHeapStart();
	UINT handleSize = Graphics::DirectX::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// インデックス分オフセットさせる
	gpuHandle.ptr += (UINT64)Info.index * handleSize;

	CommandList->SetGraphicsRootDescriptorTable(RootParameterIndex, gpuHandle);
}

D3D12_CPU_DESCRIPTOR_HANDLE Engine::Graphics::GraphicsDescriptorHeapManager::GetCpuHandle(const DescriptorHeapInfo& Info) const noexcept
{
	if (Info.index < 0)
	{
		return D3D12_CPU_DESCRIPTOR_HANDLE();
	}
	return mInfo[Info.index].Cpu;
}

D3D12_GPU_DESCRIPTOR_HANDLE Engine::Graphics::GraphicsDescriptorHeapManager::GetGpuHandle(const DescriptorHeapInfo& Info) const noexcept
{
	if (Info.index < 0)
	{
		return D3D12_GPU_DESCRIPTOR_HANDLE();
	}
	return mInfo[Info.index].Gpu;
}

/// <summary>
/// グラフィックス用ディスクリプタヒープ
/// </summary>
/// <returns></returns>
ID3D12DescriptorHeap* Engine::Graphics::GraphicsDescriptorHeapManager::GetGraphicsHeap()
{
	return mGraphicsHeap.Get();
}
