#include"pch.h"
#include"Graphics/GraphicsDescriptorHeap/GraphicsDescriptorHeap.hpp"
#include"Debug/spdlog/Logger.hpp"


Engine::Graphics::GraphicsDescriptorHeap::GraphicsDescriptorHeap()
	:mHeapInfo(), mIsValid(false)
{
}

Engine::Graphics::GraphicsDescriptorHeap::~GraphicsDescriptorHeap()
{
	this->Release();
}

/// <summary>
/// ヒープの作成
/// </summary>
/// <returns>true:成功</returns>
bool Engine::Graphics::GraphicsDescriptorHeap::Create()
{
	GraphicsDescriptorHeapManager* HeapManager = GraphicsDescriptorHeapManager::GetInstance();
	if (HeapManager == nullptr)
	{
		LOG_ERROR("ヒープ管理の取得に失敗");
		return false;
	}

	mHeapInfo = HeapManager->Issuance(1);
	mIsValid = mHeapInfo.index >= 0;
	return mIsValid;
}

/// <summary>
/// ヒープの解放
/// </summary>
void Engine::Graphics::GraphicsDescriptorHeap::Release()
{
	if (mIsValid == false)
	{
		return;
	}

	GraphicsDescriptorHeapManager* HeapManager = GraphicsDescriptorHeapManager::GetInstance();
	if (HeapManager != nullptr)
	{
		HeapManager->Discard(mHeapInfo);
	}

	mIsValid = false;
}

/// <summary>
/// パイプラインへヒープのセット
/// </summary>
/// <param name="RootParameterIndex">ルートシグネチャへセットしたパラメーターインデックス</param>

void Engine::Graphics::GraphicsDescriptorHeap::Set(int RootParameterIndex)
{
	GraphicsDescriptorHeapManager* HeapManager = GraphicsDescriptorHeapManager::GetInstance();
	HeapManager->Set(RootParameterIndex, mHeapInfo);
}

D3D12_CPU_DESCRIPTOR_HANDLE Engine::Graphics::GraphicsDescriptorHeap::GetCpuHandle() const
{
	GraphicsDescriptorHeapManager* HeapManager = GraphicsDescriptorHeapManager::GetInstance();
	return HeapManager->GetCpuHandle(mHeapInfo);

}

D3D12_GPU_DESCRIPTOR_HANDLE Engine::Graphics::GraphicsDescriptorHeap::GetGpuHandle() const
{
	GraphicsDescriptorHeapManager* HeapManager = GraphicsDescriptorHeapManager::GetInstance();
	return HeapManager->GetGpuHandle(mHeapInfo);
}
