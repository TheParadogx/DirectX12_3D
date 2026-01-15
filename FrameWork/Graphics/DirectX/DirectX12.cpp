#include "pch.h"
#include "DirectX12.hpp"
#include "Debug/spdlog/Logger.hpp"

/// <summary>
/// 初期値
/// </summary>
void Engine::Graphics::DirectX::OnCreate()
{
	mDevice = nullptr;
	mFactory = nullptr;
	mSwapChain = nullptr;
	mCommandList = nullptr;
	mCommandQueue = nullptr;
	mCommandAllocator.fill(nullptr);
	mBackBaffers.fill(nullptr);
	mDepthBuffer = nullptr;
	mBackBufferHeap = nullptr;
	mDepthHeap = nullptr;
	mFence = nullptr;
	mDebugDevice = nullptr;
	mFenceValues.fill(0);
	mNextFenceValue = 0;
	mResourceBarrier = {};
	mWaitForGPUEventHandle = nullptr;
	mFrameIndex = 0;
}

/// <summary>
/// メモリの解放
/// </summary>
void Engine::Graphics::DirectX::OnDestory()
{
	this->WaitForGPU();

	CloseHandle(mWaitForGPUEventHandle);

	for (auto& buffer : mBackBaffers)
	{
		buffer.Reset();
	}
	mDepthBuffer.Reset();
	mBackBufferHeap.Reset();
	mDepthHeap.Reset();

	for (auto& allcator : mCommandAllocator)
	{
		allcator.Reset();
	}
	mCommandList.Reset();
	mCommandQueue.Reset();
	mSwapChain.Reset();

	mFence.Reset();
	mFactory.Reset();

	mDevice.Reset();
	if (mDebugDevice != nullptr)
	{
		mDebugDevice->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
		mDebugDevice.Reset();
	}

}

bool Engine::Graphics::DirectX::Initialize(HWND WindowHandle, UINT Width, UINT Height)
{
	bool ret = false;
#if _DEBUG
	DebugLayerOn();
#endif
	//	ファクトリー初期化
	ret = InitializeFactory();
	if (ret == false)
	{
		return false;
	}
	//	デバイス初期化
	ret = InitializeDevice();
	if (ret == false)
	{
		return false;
	}
	//	スワップチェイン初期化
	ret = InitializeSwapChain(WindowHandle, Width, Height);
	if (ret == false)
	{
		return false;
	}
	//	バックバッファ用ヒープ初期化
	ret = InitializeBackBufferHeap();
	if (ret == false)
	{
		return false;
	}
	//	深度バッファ用ヒープ初期化
	ret = InitializeDepthHeap();
	if (ret == false)
	{
		return false;
	}
	//	レンダーターゲット初期化
	ret = InitializeRenderTarget(Width, Height);
	if (ret == false)
	{
		return false;
	}
	//	フェンスの初期化
	ret = InitializeFence();
	if (ret == false)
	{
		return false;
	}
	return true;
}

void Engine::Graphics::DirectX::BegineRendering()
{
	mFrameIndex = mSwapChain->GetCurrentBackBufferIndex();
	const UINT64 fenceToWait = mFenceValues[mFrameIndex];

	//	GPUが前フレーム処理中なら待機
	if (mFence->GetCompletedValue() < fenceToWait)
	{
		mFence->SetEventOnCompletion(fenceToWait, mWaitForGPUEventHandle);
		WaitForSingleObject(mWaitForGPUEventHandle, INFINITE);
	}

	//	フレーム専用アロケーターのリセット
	mCommandAllocator[mFrameIndex]->Reset();
	mCommandList->Reset(mCommandAllocator[mFrameIndex].Get(), nullptr);

	//	バックバッファをRENDER_TARGETに遷移
	mResourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	mResourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	mResourceBarrier.Transition.pResource = mBackBaffers[mFrameIndex].Get();
	mResourceBarrier.Transition.Subresource = 0;
	mResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	mResourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	mCommandList->ResourceBarrier(1, &mResourceBarrier);

	D3D12_CPU_DESCRIPTOR_HANDLE heap = mBackBufferHeap->GetCPUDescriptorHandleForHeapStart();
	heap.ptr += static_cast<SIZE_T>(mFrameIndex) * static_cast<SIZE_T>(mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	D3D12_CPU_DESCRIPTOR_HANDLE dsvheap = mDepthHeap->GetCPUDescriptorHandleForHeapStart();
	//	レンダーターゲット設定
	mCommandList->OMSetRenderTargets(1, &heap, true, &dsvheap);

	//	レンダーターゲットクリア
	mCommandList->ClearRenderTargetView(heap, mBackColor.value, 0, nullptr);
	mCommandList->ClearDepthStencilView(dsvheap, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

/// <summary>
/// 画面のフリップ
/// </summary>
void Engine::Graphics::DirectX::Flip()
{
	//	バックバッファをPRESENTに遷移
	mResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	mResourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	mCommandList->ResourceBarrier(1, &mResourceBarrier);

	//	コマンドリストを閉じる
	mCommandList->Close();

	//	GPUの送信
	ID3D12CommandList* cmdLists[] = { mCommandList.Get()};
	mCommandQueue->ExecuteCommandLists(1, cmdLists);

	//	画面表示
	mSwapChain->Present(1, 0);

	//	フェンスにシグナル
	mCommandQueue->Signal(mFence.Get(), ++mNextFenceValue);

	//	次フレームのフェンス値を保存
	const UINT nextIndex = mSwapChain->GetCurrentBackBufferIndex();
	mFenceValues[nextIndex] = mNextFenceValue;

}

/// <summary>
/// GPUの処理待ち
/// </summary>
void Engine::Graphics::DirectX::WaitForGPU()
{
	mCommandQueue->Signal(mFence.Get(), ++mNextFenceValue);

	if (mFence->GetCompletedValue() < mNextFenceValue)
	{
		mFence->SetEventOnCompletion(mNextFenceValue, mWaitForGPUEventHandle);
		WaitForSingleObject(mWaitForGPUEventHandle, INFINITE);
	}
}

/// <summary>
/// ビューポートの設定
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="width"></param>
/// <param name="height"></param>
void Engine::Graphics::DirectX::SetViewPort(float x, float y, float Width, float Height)
{
	D3D12_VIEWPORT viewport = {};
	viewport.TopLeftX = x;
	viewport.TopLeftY = y;
	viewport.Width = Width;
	viewport.Height = Height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	mCommandList->RSSetViewports(1, &viewport);

	//	シザー矩形設定
	D3D12_RECT rect = {};
	rect.top = 0;
	rect.left = 0;
	rect.right = static_cast<LONG>(Width);
	rect.bottom = static_cast<LONG>(Height);
	mCommandList->RSSetScissorRects(1, &rect);

}

/// <summary>
/// Dx12コマンドアロケーターの取得
/// </summary>
/// <returns></returns>
ID3D12CommandAllocator* Engine::Graphics::DirectX::GetCommandAllocator()
{
	// 配列範囲チェック
	if (mFrameIndex >= NUM_BACK_BAFFER)
	{
		return nullptr;
	}

	// このフレームのフェンス値
	const UINT64 fenceValueToWaitFor = mFenceValues[mFrameIndex];

	// GPUがまだコマンドを処理中なら待機
	if (mFence->GetCompletedValue() < fenceValueToWaitFor)
	{
		mFence->SetEventOnCompletion(fenceValueToWaitFor, mWaitForGPUEventHandle);
		WaitForSingleObject(mWaitForGPUEventHandle, INFINITE);
	}

	//	使用前にリセット
	mCommandAllocator[mFrameIndex]->Reset();

	// 安全にアロケータ取得
	return mCommandAllocator[mFrameIndex].Get();
}

/// <summary>
/// 背景色のセット
/// </summary>
/// <param name="BackColor"></param>
void Engine::Graphics::DirectX::SetBackColor(const Color& BackColor)
{
	mBackColor = BackColor;
}

/// <summary>
/// デバッグレイヤーの起動
/// </summary>
void Engine::Graphics::DirectX::DebugLayerOn()
{
	ID3D12Debug* debugLater = nullptr;
	const HRESULT hr = D3D12GetDebugInterface(IID_PPV_ARGS(&debugLater));
	if (SUCCEEDED(hr))
	{
		debugLater->EnableDebugLayer();
		debugLater->Release();
	}
}

/// <summary>
/// デバイスの初期化
/// </summary>
/// <returns>true:成功</returns>
bool Engine::Graphics::DirectX::InitializeDevice()
{
	bool success = false;
	//	アダプター列挙
	for (UINT i = 0;; i++)
	{
		mw::AdapterPtr adapter;
		if (mFactory->EnumAdapters(i, adapter.GetAddressOf()) == DXGI_ERROR_NOT_FOUND)
		{
			break;
		}

		DXGI_ADAPTER_DESC adapterDecs = {};
		adapter->GetDesc(&adapterDecs);

		const D3D_FEATURE_LEVEL level = D3D_FEATURE_LEVEL_12_0;
		const HRESULT ret = D3D12CreateDevice(adapter.Get(), level, IID_PPV_ARGS(&mDevice));

		if (SUCCEEDED(ret))
		{
			success = true;
			break;
		}
	}
#if _DEBUG
	if (mDevice != nullptr)
	{
		mDevice->QueryInterface(IID_PPV_ARGS(&mDebugDevice));
	}
#endif // _DEBUG
	return success;
}

/// <summary>
/// ファクトリーの初期化
/// </summary>
/// <returns>true:成功</returns>
bool Engine::Graphics::DirectX::InitializeFactory()
{
#if _DEBUG
	const HRESULT ret = CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&mFactory));
#else
	const HRESULT ret = CreateDXGIFactory1(IID_PPV_ARGS(&mFactory));
#endif
	if (FAILED(ret))
	{
		LOG_CRITICAL("Dx12ファクトリの作成に失敗");
		return false;
	}
	return true;
}

/// <summary>
/// スワップチェインの初期化
/// </summary>
/// <param name="WindowHandle"></param>
/// <param name="Width"></param>
/// <param name="Height"></param>
/// <returns>true:成功</returns>
bool Engine::Graphics::DirectX::InitializeSwapChain(HWND WindowHandle, UINT Width, UINT Height)
{
	//	アロケーターの初期化
	HRESULT hr;
	for (auto& allocator : mCommandAllocator)
	{
		hr = mDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&allocator));
		if (FAILED(hr))
		{
			LOG_CRITICAL("コマンドアロケータの作成に失敗");
			return false;
		}
	}

	//	コマンドリストの初期化
	hr = mDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mCommandAllocator[0].Get(), nullptr, IID_PPV_ARGS(&mCommandList));
	if (FAILED(hr))
	{
		LOG_CRITICAL("コマンドリストの作成に失敗");
		return false;
	}

	//	コマンドキュー初期化
	D3D12_COMMAND_QUEUE_DESC cmdQueue = {};
	cmdQueue.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	cmdQueue.NodeMask = 0;
	cmdQueue.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	cmdQueue.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	hr = mDevice->CreateCommandQueue(&cmdQueue, IID_PPV_ARGS(&mCommandQueue));
	if (FAILED(hr))
	{
		LOG_CRITICAL("コマンドキューの作成に失敗");
		return false;
	}

	//	スワップチェイン初期化
	DXGI_SWAP_CHAIN_DESC1 scDesc = {};
	scDesc.Width = Width;
	scDesc.Height = Height;
	scDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scDesc.Stereo = false;
	scDesc.SampleDesc.Count = 1;
	scDesc.SampleDesc.Quality = 0;
	scDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	scDesc.BufferCount = 2;
	scDesc.Scaling = DXGI_SCALING_STRETCH;
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	scDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	scDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//	生成	
	hr = mFactory->CreateSwapChainForHwnd(mCommandQueue.Get(), WindowHandle, &scDesc, nullptr, nullptr, (IDXGISwapChain1**)mSwapChain.GetAddressOf());
	if (FAILED(hr))
	{
		LOG_CRITICAL("スワップチェインの作成に失敗");
		return false;
	}

	//mCommandList->Close();

	return true;
}

/// <summary>
/// バックバッファ用ディスクリプタヒープの初期化
/// </summary>
/// <returns>true:成功</returns>
bool Engine::Graphics::DirectX::InitializeBackBufferHeap()
{
	//	ヒープの設定情報を入れる構造体
	//	レンダーターゲット用
	D3D12_DESCRIPTOR_HEAP_DESC dchDesc = {};
	dchDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;	//	シェーダーリソース、定数バッファ用。
	dchDesc.NodeMask = 0;	//	マルチGPU環境用。普通はシングルだから０
	dchDesc.NumDescriptors =  NUM_BACK_BAFFER;	//	ヒープ内に作るRTVの数(バックバッファの枚数分)
	dchDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;	//	シェーダーからアクセスできるようにするか（D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLEなら見える）

	//	ディスクリプタヒープの作成
	const HRESULT hr = mDevice->CreateDescriptorHeap(&dchDesc, IID_PPV_ARGS(&mBackBufferHeap));

	if (FAILED(hr))
	{
		LOG_CRITICAL("バックバッファ用のヒープの作成に失敗");
		return false;
	}

	return true;
}

/// <summary>
/// 深度バッファ用ディスクリプタヒープの初期化
/// </summary>
/// <returns>true:成功</returns>
bool Engine::Graphics::DirectX::InitializeDepthHeap()
{
	//	深度用ディスクリプターヒープ
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	desc.NodeMask = 0;
	desc.NumDescriptors = 1;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	const HRESULT hr = mDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&mDepthHeap));
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

/// <summary>
/// レンダーターゲットの初期化
/// </summary>
/// <param name="width"></param>
/// <param name="height"></param>
/// <returns>true:成功</returns>
bool Engine::Graphics::DirectX::InitializeRenderTarget(UINT Width, UINT Height)
{
	HRESULT ret = 0;

	D3D12_CPU_DESCRIPTOR_HANDLE handle = mBackBufferHeap->GetCPUDescriptorHandleForHeapStart();
	for (UINT index = 0; index < NUM_BACK_BAFFER; index++)
	{
		//	スワップチェインから実際のバックバッファのリソース取得
		ret = mSwapChain->GetBuffer(index, IID_PPV_ARGS(&mBackBaffers[index]));
		if (FAILED(ret))
		{
			LOG_CRITICAL("スワップチェインからバックバッファリソースの取得に失敗");
			return false;
		}
		//	取得したバッファを描画可能なターゲットRTVとして登録
		mDevice->CreateRenderTargetView(mBackBaffers[index].Get(), nullptr, handle);
		//	1つ分RTVを進める
		handle.ptr += mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}

	//	深度バッファのリソース生成
	D3D12_RESOURCE_DESC depthResource = {};
	depthResource.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResource.Width = static_cast<UINT64>(Width);
	depthResource.Height = Height;
	depthResource.DepthOrArraySize = 1;
	depthResource.Format = DXGI_FORMAT_D32_FLOAT;
	depthResource.SampleDesc.Count = 1;
	depthResource.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	//	深度値用。GPU専用のVRAM上に確保
	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	//	クリアバリュー
	//	クリア用の初期値
	D3D12_CLEAR_VALUE clearvalue = {};
	clearvalue.DepthStencil.Depth = 1.0f;
	clearvalue.Format = DXGI_FORMAT_D32_FLOAT;

	//	深度リソースの確保
	ret = mDevice->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResource,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&clearvalue,
		IID_PPV_ARGS(&mDepthBuffer));

	if (FAILED(ret))
	{
		LOG_CRITICAL("深度リソース作成に失敗");
		return false;
	}

	//	dsvの作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
	mDevice->CreateDepthStencilView(mDepthBuffer.Get(), &dsvDesc, mDepthHeap->GetCPUDescriptorHandleForHeapStart());

	return true;
}

/// <summary>
/// フェンスの初期化
/// </summary>
/// <returns>true:成功</returns>
bool Engine::Graphics::DirectX::InitializeFence()
{
	const HRESULT ret = mDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence));
	if (FAILED(ret))
	{
		return false;
	}

	mNextFenceValue = 1;
	//	イベントハンドルの作成
	mWaitForGPUEventHandle = CreateEvent(nullptr, false, false, nullptr);
	if (mWaitForGPUEventHandle == nullptr)
	{
		return false;
	}

	return true;
}
